#ifndef FILESELECTOR_HPP
#define FILESELECTOR_HPP

#include <filesystem>
#include <string>
#include <map>
#include <vector>

#include "imgui.h"

#include "imgui_stdlib.h"

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

// Used for load and save menus and file system operations
// EXEPT for loading and saving config file

// singleton
class FileSelector
{
public:
    enum EntryType
    {
        Error = -1, // can not read
        Ignore,     // not needed for the simple file explorer
        FileEntry,  // file
        DirEntry    // dir
    };

public:
    static FileSelector &GetInstance();
    // only current directory path
    inline std::filesystem::path GetCurrDirectoryPath() { return currDirectoryPath; }
    // set the directory path after loading config
    void SetDirectoryPath(std::filesystem::path path);
    // refresh the directory vector and map
    // sets currenEntry to none
    void RefreshCurrDir();
    // load an image
    // 0 ok
    // 1 canceled
    // 2 nothing
    int32_t LoadMenu(Image *imageToLoad, bool noTexture = false);
    // save an image as
    // 0 ok
    // 1 canceled
    // 2 nothing
    int32_t SaveAsMenu(Image *imageToSave);
    // save an image
    // 0 ok
    // 1 canceled
    // 2 nothing
    int32_t SaveWarningPopup(Image *imageToSave);

    // does a file with such name exist
    inline bool FileExists(std::filesystem::path path) { return std::filesystem::exists(path); }

private:
    void WarningAndErrorPopUp(Image *imageToSave, int32_t *returnCode);

    void DeselectCurrEntry();
    // select by path
    int SelectEntry(std::filesystem::path entryname);
    // if any entry has been selected (flag is set in map)
    int SelectCurrEntry();
    // set current directory to parent directory
    void GoUpADirectory();

private:
    FileSelector();
    ~FileSelector();

private:
    std::filesystem::path currDirectoryPath;               // the directory path
    std::vector<std::filesystem::directory_entry> currDir; // the dircetory
    std::map<std::filesystem::path, bool> dirMaped;        // maped by full path

    std::filesystem::path currEntrySelected = ""; // full path

// File name and extension
#ifdef __linux__
    std::string fileNameBuff = std::string(64, '\0');
#elif _WIN32
    std::u8string fileNameBuff = std::u8string(64, u8'\0');
#endif
    int currExtension = 0;
    bool errorPopupActive = false;
    bool warningPopupActive = false;
    bool customName = false; // in save as
};

#endif
