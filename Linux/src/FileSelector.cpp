#include "FileSelector.hpp"

FileSelector::FileSelector()
{
    currDirPath = std::filesystem::current_path();
    for (const auto &entry : std::filesystem::directory_iterator(currDirPath))
    {
        currDir.emplace(currDir.end(), entry);
        dirMaped.emplace(std::pair<std::string, bool>(entry.path(), false));
    }
}

FileSelector::~FileSelector() {}

FileSelector &FileSelector::GetInstance()
{
    static FileSelector *instance = new FileSelector();
    return *instance;
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
        if (std::filesystem::is_directory(entryname))
        {
            currDirPath = entryname;
            RefreshCurrDir();
            return DirEntry;
        }
        else if(std::filesystem::is_regular_file(entryname))
            return FileEntry;
        return Ignore;
    }
}

void FileSelector::GoUpADirectory()
{
    auto it = currDirPath.length();
    it--;
    for (int i = it; i >= 0; i--)
    {
        if (currDirPath[i] == '/')
        {
            it = i;
            break;
        }
    }
    // root
    if (it == 0)
        it++;
    currDirPath = currDirPath.substr(0, it);
    RefreshCurrDir();
}

void FileSelector::RefreshCurrDir()
{
    currEntrySelected = "";
    currDir.clear();
    dirMaped.clear();
    for (const auto &entry : std::filesystem::directory_iterator(currDirPath))
    {
        currDir.emplace(currDir.end(), entry);
        dirMaped.emplace(std::pair<std::string, bool>(entry.path(), false));
    }
}
