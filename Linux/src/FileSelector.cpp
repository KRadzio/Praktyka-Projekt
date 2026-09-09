#include "FileSelector.hpp"

FileSelector::FileSelector()
{
    // set vector and map
    currDirectoryPath = std::filesystem::current_path();
    for (const auto &entry : std::filesystem::directory_iterator(currDirectoryPath))
    {
        currDir.emplace(currDir.end(), entry);
        dirMaped.emplace(std::pair<std::filesystem::path, bool>(entry.path(), false));
    }
}

FileSelector::~FileSelector() {}

FileSelector &FileSelector::GetInstance()
{
    static FileSelector *instance = new FileSelector();
    return *instance;
}

void FileSelector::DeselectCurrEntry()
{
    // set to none
    if (currEntrySelected != "")
    {
        dirMaped[currEntrySelected] = false;
        currEntrySelected = "";
    }
}

int FileSelector::SelectEntry(std::filesystem::path entryname)
{
    // empty select the new one
    if (currEntrySelected == "")
    {
        currEntrySelected = entryname;
        dirMaped[currEntrySelected] = true;
        return Ignore;
    }

    // change selection to the new one
    if (entryname != currEntrySelected)
    {
        dirMaped[currEntrySelected] = false;
        currEntrySelected = entryname;
        dirMaped[currEntrySelected] = true;
        return Ignore;
    }

    // confirm selection
    else
    {
        dirMaped[currEntrySelected] = false;
        // dir
        if (std::filesystem::is_directory(entryname))
        {
            currDirectoryPath = entryname;
            RefreshCurrDir();
            return DirEntry;
        }
        // file
        else if (std::filesystem::is_regular_file(entryname))
            return FileEntry;
        return Ignore;
    }
}

int FileSelector::SelectCurrEntry()
{
    // empty do nothing
    if (currEntrySelected == "")
        return Ignore;
    else
    {
        // dir
        if (std::filesystem::is_directory(currEntrySelected))
        {
            currDirectoryPath = currEntrySelected;
            RefreshCurrDir();
            return DirEntry;
        }
        // file
        else if (std::filesystem::is_regular_file(currEntrySelected))
            return FileEntry;
        return Ignore;
    }
}

void FileSelector::GoUpADirectory()
{
    currDirectoryPath = currDirectoryPath.parent_path();
    RefreshCurrDir();
}

void FileSelector::RefreshCurrDir()
{
    // set entry to none
    currEntrySelected = "";
    currDir.clear();
    dirMaped.clear();
    // refresh the vector and map
    for (const auto &entry : std::filesystem::directory_iterator(currDirectoryPath))
    {
        currDir.emplace(currDir.end(), entry);
        dirMaped.emplace(std::pair<std::filesystem::path, bool>(entry.path(), false));
    }
}

void FileSelector::ActivatePopupMenu()
{
    loadPopupActive = true;
    RefreshCurrDir();
}

int32_t FileSelector::LoadMenu(Image *imageToLoad, bool noTexture)
{
    // can not just return when loaded the gui need to know when the popup ends
    // the return code is set and then returned at the end of func
    int32_t returnCode = 2;
    // can not be opend if thread is running
    ImGui::OpenPopup("WczytajPlik", ImGuiPopupFlags_NoReopen);
    ImGui::SetNextWindowSize(ImVec2(FILE_POPUP_WIDTH, FILE_POPUP_HEIGHT));
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("WczytajPlik", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        auto dir = GetCurrDir();
        auto map = GetDirMaped();
        // curr dir path
        ImGui::BeginChild("Dir", ImVec2(DIR_LIST_WIDTH, DIR_LIST_HEIGHT), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::Text("%s", GetCurrDirectoryPath().c_str());
        ImGui::Separator();
        // display entries as selectebles
        for (auto entry : dir)
            if (ImGui::Selectable(entry.path().filename().c_str(), map[entry.path()], ImGuiSelectableFlags_NoAutoClosePopups))
                if (SelectEntry(entry.path()) == FileEntry)
                {
                    if (!noTexture)
                    {
                        // could not be loaded
                        if (imageToLoad->SetSourceImage(GetFullPathToEntry()) == -1)
                        {
                            errorPopupActive = true;
                        }
                        else
                        {
                            loadPopupActive = false;
                            ImGui::CloseCurrentPopup();
                            returnCode = 0;
                        }
                    }
                    else
                    {
                        // could not be loaded
                        if (imageToLoad->SetSourceImageNoTEXTURE(GetFullPathToEntry()) == -1)
                        {
                            errorPopupActive = true;
                        }
                        else
                        {
                            loadPopupActive = false;
                            ImGui::CloseCurrentPopup();
                            returnCode = 0;
                        }
                    }
                }
        ImGui::EndChild();
        ImGui::Separator();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W_FS / 2);
        if (ImGui::Button("Otwórz", ImVec2(CANCEL_BUTTON_W_FS, 0)))
        {
            if (SelectCurrEntry() == FileEntry)
            {
                if (!noTexture)
                {
                    // could not be loaded
                    if (imageToLoad->SetSourceImage(GetFullPathToEntry()) == -1)
                    {
                        errorPopupActive = true;
                    }
                    else
                    {
                        loadPopupActive = false;
                        ImGui::CloseCurrentPopup();
                        returnCode = 0;
                    }
                }
                else
                {
                    // could not be loaded
                    if (imageToLoad->SetSourceImageNoTEXTURE(GetFullPathToEntry()) == -1)
                    {
                        errorPopupActive = true;
                    }
                    else
                    {
                        loadPopupActive = false;
                        ImGui::CloseCurrentPopup();
                        returnCode = 0;
                    }
                }
            }
        }
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W_FS / 2);
        if (ImGui::Button("Folder wyżej", ImVec2(CANCEL_BUTTON_W_FS, 0)))
        {
            GoUpADirectory();
        }
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W_FS / 2);
        if (ImGui::Button("Anuluj", ImVec2(CANCEL_BUTTON_W_FS, 0)))
        {
            loadPopupActive = false;
            ImGui::CloseCurrentPopup();
            returnCode = 1;
        }

        if (errorPopupActive)
        {
            ImGui::OpenPopup("BLĄD", ImGuiPopupFlags_NoReopen);
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowSize(ImVec2(0, ERROR_SPACE_HEIGHT));
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            if (ImGui::BeginPopupModal("BLĄD", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
            {
                ImGui::Text("Nie udało sie wczytać pliku:");
                ImGui::Text("%s", imageToLoad->GetError().c_str());
                ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W_FS / 2);
                if (ImGui::Button("OK", ImVec2(CANCEL_BUTTON_W_FS, 0)))
                {
                    errorPopupActive = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
        ImGui::EndPopup();
    }
    return returnCode;
}