#include "FileSelector.hpp"

FileSelector::FileSelector()
{
    currPath = std::filesystem::current_path();
    for (const auto &entry : std::filesystem::directory_iterator(currPath))
    {
        currDir.emplace(currDir.end(), entry);
        currDirEntryNames.emplace(currDirEntryNames.end(), entry.path().filename());
        dirMaped.emplace(std::pair<std::string, bool>(entry.path().filename(), false));
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
    if (currEntrySelected != "")
    {
        dirMaped[currEntrySelected] = false;
        currEntrySelected = "";
    }
}

int FileSelector::SelectEntry(std::string entryname)
{
    // empty
    if (currEntrySelected == "")
    {
        currEntrySelected = entryname;
        dirMaped[currEntrySelected] = true;
        return Ignore;
    }

    // change selection
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
        if (std::filesystem::is_directory(currPath.string() + '/' + entryname))
        {
            currPath /= entryname;
            RefreshCurrDir();
            return DirEntry;
        }
        else if (std::filesystem::is_regular_file(currPath.string() + '/' + entryname))
            return FileEntry;
        return Ignore;
    }
}

int FileSelector::SelectCurrEntry()
{
    // empty
    if (currEntrySelected == "")
        return Ignore;

    else
    {
        if (std::filesystem::is_directory(currPath.string() + '/' + currEntrySelected))
        {
            currPath /= currEntrySelected;
            RefreshCurrDir();
            return DirEntry;
        }
        else if (std::filesystem::is_regular_file(currPath.string() + '/' + currEntrySelected))
            return FileEntry;
        return Ignore;
    }
}

void FileSelector::GoUpADirectory()
{
    currPath = currPath.parent_path();
    RefreshCurrDir();
}

void FileSelector::RefreshCurrDir()
{
    currEntrySelected = "";
    currDir.clear();
    currDirEntryNames.clear();
    dirMaped.clear();
    for (const auto &entry : std::filesystem::directory_iterator(currPath))
    {
        currDir.emplace(currDir.end(), entry);
        currDirEntryNames.emplace(currDirEntryNames.end(), entry.path().filename());
        dirMaped.emplace(std::pair<std::string, bool>(entry.path().filename(), false));
    }
}