#include "FileSelector.hpp"

FileSelector::FileSelector()
{
    currDirPath = std::filesystem::current_path();
    for (const auto &entry : std::filesystem::directory_iterator(currDirPath))
    {
        currDir.emplace(currDir.end(), entry);
        std::string tmp = entry.path();
        int pos = 0;
        for (int i = tmp.length(); i >= 0; i--)
            if (tmp[i] == '/')
            {
                pos = i;
                break;
            }

        tmp = tmp.substr(pos);
        currDirEntryNames.emplace(currDirEntryNames.end(), tmp);
        dirMaped.emplace(std::pair<std::string, bool>(tmp, false));
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
        if (std::filesystem::is_directory(currDirPath + '/' + entryname))
        {
            currDirPath += '/' + entryname;
            RefreshCurrDir();
            return DirEntry;
        }
        else if (std::filesystem::is_regular_file(currDirPath + '/' + entryname))
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
        if (std::filesystem::is_directory(currDirPath + '/' + currEntrySelected))
        {
            currDirPath += '/' + currEntrySelected;
            RefreshCurrDir();
            return DirEntry;
        }
        else if (std::filesystem::is_regular_file(currDirPath + '/' + currEntrySelected))
            return FileEntry;
        return Ignore;
    }
}

void FileSelector::GoUpADirectory()
{
    int pos = 0;
    for (int i = currDirPath.length(); i >= 0; i--)
        if (currDirPath[i] == '/')
        {
            pos = i;
            break;
        }
    // root
    if (pos == 0)
        pos++;
    currDirPath = currDirPath.substr(0, pos);
    RefreshCurrDir();
}

void FileSelector::RefreshCurrDir()
{
    currEntrySelected = "";
    currDir.clear();
    currDirEntryNames.clear();
    dirMaped.clear();
    for (const auto &entry : std::filesystem::directory_iterator(currDirPath))
    {
        currDir.emplace(currDir.end(), entry);
        std::string tmp = entry.path();
        int pos = 0;
        for (int i = tmp.length(); i >= 0; i--)
            if (tmp[i] == '/')
            {
                pos = i;
                break;
            }

        tmp = tmp.substr(pos);
        currDirEntryNames.emplace(currDirEntryNames.end(), tmp);
        dirMaped.emplace(std::pair<std::string, bool>(tmp, false));
    }
}
