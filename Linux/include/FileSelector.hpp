#ifndef FILE_SELECTOR_HPP
#define FILE_SELECTOR_HPP

#include <filesystem>
#include <string>
#include <map>
#include <vector>

enum EntryType
{
    Ignore= -1,
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

    int SelectEntry(std::string entryname);
    void GoUpADirectory();

private:
    void RefreshCurrDir();

private:
    FileSelector();
    ~FileSelector();

private:
    std::string currDirPath;
    std::vector<std::filesystem::directory_entry> currDir;
    std::map<std::string, bool> dirMaped;

    std::string currEntrySelected = "";
};

#endif
