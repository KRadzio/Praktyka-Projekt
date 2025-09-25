#include "FileSelector.hpp"

FileSelector::FileSelector()
{
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
    if (currEntrySelected != "")
    {
        dirMaped[currEntrySelected] = false;
        currEntrySelected = "";
    }
}

int FileSelector::SelectEntry(std::filesystem::path entryname)
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
        if (std::filesystem::is_directory(entryname))
        {
            currDirectoryPath = entryname;
            RefreshCurrDir();
            return DirEntry;
        }
        else if (std::filesystem::is_regular_file(entryname))
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
        if (std::filesystem::is_directory(currEntrySelected))
        {
            currDirectoryPath = currEntrySelected;
            RefreshCurrDir();
            return DirEntry;
        }
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
    currEntrySelected = "";
    currDir.clear();
    dirMaped.clear();
    for (const auto &entry : std::filesystem::directory_iterator(currDirectoryPath))
    {
        currDir.emplace(currDir.end(), entry);
        dirMaped.emplace(std::pair<std::filesystem::path, bool>(entry.path(), false));
    }
}