#ifndef FILESELECTOR_HPP
#define FILESELECTOR_HPP

#include <filesystem>
#include <string>
#include <map>
#include <vector>

#include "imgui.h"

#include "Image.hpp"

#define POPUP_WIDTH 200
#define POPUP_HEIGHT 100
#define FILE_POPUP_WIDTH 300
#define FILE_POPUP_HEIGHT 340
#define SAVE_POPUP_HEIGHT 480
#define BUTTON_OFFSET 20

#define DIR_LIST_WIDTH 290
#define DIR_LIST_HEIGHT 200

#define ERROR_SPACE_HEIGHT 120

#define CANCEL_BUTTON_W_FS 120


// RENAME TO FILEMANAGER

// singleton
class FileSelector
{
public:
    enum EntryType
    {
        Error = -1, // can not read
        Ignore, // not needed for the simple file explorer
        FileEntry, // file
        DirEntry // dir
    };

public:
    static FileSelector &GetInstance();
    // only current directory path
    inline std::filesystem::path GetCurrDirectoryPath() { return currDirectoryPath; }
    // vector of directory entries
    inline std::vector<std::filesystem::directory_entry> GetCurrDir() { return currDir; }
    // current directory entries mapped (used to determine which entry is selected)
    inline std::map<std::filesystem::path, bool> &GetDirMaped() { return dirMaped; }
    // full path to selected entry
    inline std::filesystem::path GetFullPathToEntry() { return currEntrySelected; }
    void DeselectCurrEntry();

    // select by path
    int SelectEntry(std::filesystem::path entryname);
    // if any entry has been selected (flag is set in map)
    int SelectCurrEntry();
    // set current directory to parent directory
    void GoUpADirectory();
    // refresh the directory vector and map
    // sets currenEntry to none
    void RefreshCurrDir();

    // load an  image
    // -1 error
    // 0 ok
    // 1 canceled
    // 2 nothing
    int32_t LoadMenu(Image* imageToLoad, bool noTexture = false);

    // does a file with such name exist
    inline bool FileExists(std::filesystem::path path) { return std::filesystem::exists(path); }

private:
    FileSelector();
    ~FileSelector();

private:
    std::filesystem::path currDirectoryPath; // the directory path
    std::vector<std::filesystem::directory_entry> currDir; // the dircetory
    std::map<std::filesystem::path, bool> dirMaped; // maped by full path

    std::filesystem::path currEntrySelected = ""; // full path

    bool errorPopupActive = false;
    bool errorPopupAlgActive = false;
    bool warningPopupActive = false;
    bool customName = false; // in save as
};

#endif
