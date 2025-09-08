#ifndef FILE_SELECTOR_HPP
#define FILE_SELECTOR_HPP

#include <filesystem>
#include <string>
#include <map>
#include <vector>

enum EntryType
{
    Error = -1,
    Ignore,
    FileEntry,
    DirEntry
};

class FileSelector
{
public:
    static FileSelector &GetInstance();
    inline std::string GetCurrPath() { return currDirPath; }
    inline std::vector<std::filesystem::directory_entry> GetCurrDir() { return currDir; }
    inline std::map<std::string, bool> &GetDirMaped() { return dirMaped; }
    inline std::string GetCurrEntry() { return currEntrySelected; }
    inline std::vector<std::string> GetCurrDirEntryNames() { return currDirEntryNames; }
    inline std::string GetFullPathToFile() { return currDirPath + '/' + currEntrySelected; }

    int SelectEntry(std::string entryname);
    int SelectCurrEntry();
    void GoUpADirectory();

private:
    void RefreshCurrDir();

private:
    FileSelector();
    ~FileSelector();

private:
    std::string currDirPath;
    std::vector<std::filesystem::directory_entry> currDir;
    std::vector<std::string> currDirEntryNames;
    std::map<std::string, bool> dirMaped;

    std::string currEntrySelected = "";
};

#endif
