#ifndef FILESELECTOR
#define FILESELECTOR

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
    inline std::filesystem::path GetCurrDirectoryPath() { return currDirectoryPath; }
    inline std::vector<std::filesystem::directory_entry> GetCurrDir() { return currDir; }
    inline std::map<std::filesystem::path, bool> &GetDirMaped() { return dirMaped; }
    inline std::string GetCurrEntry() { return currEntrySelected; }
    inline std::string GetFullPathToFile() { return currEntrySelected; }
    void DeselectCurrEntry();

    int SelectEntry(std::filesystem::path entryname);
    int SelectCurrEntry();
    void GoUpADirectory();
    void RefreshCurrDir();

    inline bool FileExists(std::string filename) { return std::filesystem::exists(filename); }
    inline bool FileExists(std::filesystem::path path) { return std::filesystem::exists(path); }

private:
    FileSelector();
    ~FileSelector();

private:
    std::filesystem::path currDirectoryPath; // the directory path
    std::vector<std::filesystem::directory_entry> currDir; // the dircetory
    std::map<std::filesystem::path, bool> dirMaped; // maped by full path

    std::filesystem::path currEntrySelected = ""; // relative path
};

#endif
