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
    for (int i = 0; i < 64; i++)
        fileNameBuff[i] = 0;
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
                            errorPopupActive = true;
                        else
                        {
                            ImGui::CloseCurrentPopup();
                            returnCode = 0;
                        }
                    }
                    else
                    {
                        // could not be loaded
                        if (imageToLoad->SetSourceImageNoTEXTURE(GetFullPathToEntry()) == -1)
                            errorPopupActive = true;
                        else
                        {
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
                        errorPopupActive = true;
                    else
                    {
                        ImGui::CloseCurrentPopup();
                        returnCode = 0;
                    }
                }
                else
                {
                    // could not be loaded
                    if (imageToLoad->SetSourceImageNoTEXTURE(GetFullPathToEntry()) == -1)
                        errorPopupActive = true;
                    else
                    {
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

int32_t FileSelector::SaveAsMenu(Image *imageToSave)
{

    // can not just return when loaded the gui need to know when the popup ends
    // the return code is set and then returned at the end of func
    int32_t returnCode = 2;
    // can not be opend if thread is running
    ImGui::OpenPopup("ZapiszPlik", ImGuiPopupFlags_NoReopen);
    ImGui::SetNextWindowSize(ImVec2(FILE_POPUP_WIDTH, SAVE_POPUP_HEIGHT));
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("ZapiszPlik", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        auto dir = GetCurrDir();
        auto map = GetDirMaped();
        // curr dir path
        ImGui::BeginChild("Dir", ImVec2(DIR_LIST_WIDTH, DIR_LIST_HEIGHT), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::Text("%s", GetCurrDirectoryPath().c_str());
        ImGui::Separator();
        // display dir as selectables
        for (auto entry : dir)
            if (ImGui::Selectable(entry.path().filename().c_str(), map[entry.path()], ImGuiSelectableFlags_NoAutoClosePopups))
                if (SelectEntry(entry.path()) == FileSelector::FileEntry)
                    warningPopupActive = true;
        ImGui::EndChild();

        ImGui::Separator();

        ImGui::Text("Nazwa pliku");
        ImGui::InputText("wpisz", fileNameBuff, 64);

        const char *ext[] = {".png", ".jpg", ".bmp"};
        ImGui::Text("Rozszerzenie");
        ImGui::Combo("wybierz", &currExtension, ext, IM_ARRAYSIZE(ext));

        ImGui::Text("Zapisz - jeżeli chcemy nadać nazwę");
        ImGui::Text("Wybierz - jeżeli chcemy wybrać \n istniejacy plik lub folder");
        ImGui::Separator();

        int offset = (FILE_POPUP_WIDTH - 2 * CANCEL_BUTTON_W_FS - BUTTON_OFFSET) / 2;

        ImGui::SetCursorPosX(offset);
        if (ImGui::Button("Zapisz", ImVec2(CANCEL_BUTTON_W_FS, 0)))
        {
            // can not be empty
            std::string buffStr = fileNameBuff;
            if (buffStr == "")
                errorPopupActive = true;
            // already exists
            else if (FileExists(GetCurrDirectoryPath().string() + '/' + fileNameBuff + ext[currExtension]))
            {
                warningPopupActive = true;
                customName = true;
            }
            else
            {
                returnCode = 0;
                imageToSave->SaveImageAs(GetCurrDirectoryPath(), fileNameBuff, currExtension);
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::SameLine(offset + CANCEL_BUTTON_W_FS + BUTTON_OFFSET);
        if (ImGui::Button("Wybierz", ImVec2(CANCEL_BUTTON_W_FS, 0)))
        {
            // save as existing
            if (SelectCurrEntry() == FileSelector::FileEntry)
                warningPopupActive = true;
        }
        ImGui::SetCursorPosX(offset);
        if (ImGui::Button("Folder wyżej", ImVec2(CANCEL_BUTTON_W_FS, 0)))
        {
            GoUpADirectory();
        }
        ImGui::SameLine(offset + CANCEL_BUTTON_W_FS + BUTTON_OFFSET);
        if (ImGui::Button("Anuluj", ImVec2(CANCEL_BUTTON_W_FS, 0)))
        {
            returnCode = 1;
            ImGui::CloseCurrentPopup();
        }
        WarningAndErrorPopUp(imageToSave, &returnCode);
        ImGui::EndPopup();
    }
    return returnCode;
}

int32_t FileSelector::SaveWarningPopup(Image *imageToSave)
{
    int32_t returnCode = 2;
    // can not be opend if thread is running
    ImGui::OpenPopup("OSTRZEŻENIE", ImGuiPopupFlags_NoReopen);
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("OSTRZEŻENIE", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::Text("Plik o takie nazwie już istnieje czy chcesz go nadpisać?");
        ImGui::Separator();

        int offset = (ImGui::GetWindowWidth() - 2 * CANCEL_BUTTON_W_FS - BUTTON_OFFSET) / 2;

        ImGui::SetCursorPosX(offset);
        // override
        if (ImGui::Button("Zapisz", ImVec2(CANCEL_BUTTON_W_FS, 0)))
        {
            returnCode = 0;
            imageToSave->SaveImage();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine(offset + CANCEL_BUTTON_W_FS + BUTTON_OFFSET);
        if (ImGui::Button("Anuluj", ImVec2(CANCEL_BUTTON_W_FS, 0)))
        {
            returnCode = 1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    return returnCode;
}

void FileSelector::WarningAndErrorPopUp(Image *imageToSave, int32_t *returnCode)
{
    if (warningPopupActive)
    {
        ImGui::OpenPopup("OSTRZEŻENIE", ImGuiPopupFlags_NoReopen);
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("OSTRZEŻENIE", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
        {
            ImGui::Text("Plik o takie nazwie już istnieje czy chcesz go nadpisać?");
            ImGui::Separator();

            int offset = (ImGui::GetWindowWidth() - 2 * CANCEL_BUTTON_W_FS - BUTTON_OFFSET) / 2;

            ImGui::SetCursorPosX(offset);
            // override
            if (ImGui::Button("Zapisz", ImVec2(CANCEL_BUTTON_W_FS, 0)))
            {
                if (customName)
                {
                    *returnCode = 0;
                    imageToSave->SaveImageAs(FileSelector::GetInstance().GetCurrDirectoryPath(), fileNameBuff, currExtension);
                    customName = false;
                }
                else
                {
                    *returnCode = 0;
                    imageToSave->SaveImageAs(FileSelector::GetInstance().GetFullPathToEntry());
                }
                warningPopupActive = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine(offset + CANCEL_BUTTON_W_FS + BUTTON_OFFSET);
            if (ImGui::Button("Anuluj", ImVec2(CANCEL_BUTTON_W_FS, 0)))
            {
                warningPopupActive = false;
                FileSelector::GetInstance().DeselectCurrEntry();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    // can not be empty
    if (errorPopupActive)
    {
        ImGui::OpenPopup("BLĄD", ImGuiPopupFlags_NoReopen);
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowSize(ImVec2(0, POPUP_HEIGHT));
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("BLĄD", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
        {
            ImGui::Text("Nazwa pliku nie może być pusta");
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W_FS / 2);
            if (ImGui::Button("OK", ImVec2(CANCEL_BUTTON_W_FS, 0)))
            {
                errorPopupActive = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
}
