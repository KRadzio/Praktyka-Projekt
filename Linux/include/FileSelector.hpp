#ifndef FILE_SELECTOR_HPP
#define FILE_SELECTOR_HPP

#include <filesystem>
#include <string>
#include <map>
#include <vector>

class FileSelector
{
public:
    enum EntryType
    {
        Error = -1,
        Ignore,
        FileEntry,
        DirEntry
    };

public:
    static FileSelector &GetInstance();
    inline std::string GetCurrPath() { return currDirPath; }
    inline std::vector<std::filesystem::directory_entry> GetCurrDir() { return currDir; }
    inline std::map<std::string, bool> &GetDirMaped() { return dirMaped; }
    inline std::string GetCurrEntry() { return currEntrySelected; }
    inline std::vector<std::string> GetCurrDirEntryNames() { return currDirEntryNames; }
    inline std::string GetFullPathToFile() { return currDirPath + '/' + currEntrySelected; }
    void DeselectCurrEntry();

    int SelectEntry(std::string entryname);
    int SelectCurrEntry();
    void GoUpADirectory();
    void RefreshCurrDir();

    inline bool FileExists(std::string filename) { return std::filesystem::exists(filename); }

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
