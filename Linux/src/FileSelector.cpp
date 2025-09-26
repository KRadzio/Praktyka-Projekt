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