#ifndef IMGUIFILEEXPLORER_H
#define IMGUIFILEEXPLORER_H

#include <filesystem>
#include <set>
#include <string>

#include <imgui.h>

namespace ImGui
{
enum class FileExplorerViewMode
{
    Rows = 0,
    Tree,
    Boxes
};
}

namespace 
{

inline void drawPopupWindow(
    std::filesystem::path &path
)
{
    if (std::filesystem::is_directory(path))
    {
        ImGui::Text("Directory");
    }
    else if (std::filesystem::is_regular_file(path))
    {
        ImGui::Text("File");
        ImGui::Text("Size: %s", std::to_string(std::filesystem::file_size(path)).c_str());
    }
    else
    {
        ImGui::Text("Smth text");
    }

}

inline void drawPathControls(
    std::filesystem::path &currentPath
)
{
    if (ImGui::Button("<=") && currentPath.has_parent_path())
        currentPath = currentPath.parent_path();
}

inline void drawPath(
    std::filesystem::path &currentPath
)
{
    for (const auto &item : currentPath) 
    {
        ImGui::SameLine();
        std::string item_string = item.string();
        if (ImGui::Button(item_string == "" ? "/" : item_string.c_str()))
        {
            if (item_string == "") 
                currentPath = currentPath.root_path();
            else 
            {
                const std::filesystem::path newPath = item;
                while (currentPath.filename() != newPath) 
                {
                    if ( !currentPath.has_parent_path() )
                        return;
                    else
                        currentPath = currentPath.parent_path();
                }
            }
            return;
        }
    }

}

inline void drawFavorites(
    std::filesystem::path &currentPath,
    std::set<std::filesystem::path> &favoritesPath
)
{
    std::filesystem::path itemToDelete;

    for (auto path : favoritesPath)
    {
        if (ImGui::Button(std::string("X##" + path.string()).c_str()))
        {
            itemToDelete = path;
            break;
        }

        ImGui::SameLine();

        if (ImGui::Button(path.string().c_str()))
            currentPath = path;
    }

    if (std::filesystem::exists(itemToDelete))
        favoritesPath.erase(itemToDelete);
}

inline void drawFiles(
     std::filesystem::path &currentPath
    ,std::set<std::filesystem::path> &selecedItems
    ,ImGui::FileExplorerViewMode viewMode
)
{

    switch (viewMode)
    {
    case ImGui::FileExplorerViewMode::Rows:
        for (const auto &fileIter : std::filesystem::directory_iterator(currentPath))
        {
            std::filesystem::path filePath{fileIter};

            // bool selectFlag = selecedItems.contains(filePath);
            bool selectFlag = selecedItems.find(filePath) != selecedItems.end();

            if (ImGui::Checkbox(std::string("##CheckBox_" + filePath.filename().string()).c_str(), &selectFlag))
            {
                if (selectFlag)
                    selecedItems.insert(filePath);
                else
                    selecedItems.erase(filePath);
                selectFlag = !selectFlag;
            }
            ImGui::SameLine();


            if (std::filesystem::is_directory(filePath))
            {
                if (ImGui::Button(filePath.filename().string().c_str()))
                {
                    currentPath = filePath;
                    return;
                }
            }
            else if (std::filesystem::is_regular_file(filePath))
            {
                if (ImGui::Button(filePath.filename().string().c_str()))
                {
                    return;
                }
            }

            if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {
                ImGui::OpenPopup(std::string("popup_" + filePath.string()).c_str());

            }

            if (ImGui::BeginPopup(std::string("popup_" + filePath.string()).c_str(), ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoResize))
            {
                drawPopupWindow(filePath);
                ImGui::EndPopup();
            }
        }
        break;
    case ImGui::FileExplorerViewMode::Boxes:
        break;
    case ImGui::FileExplorerViewMode::Tree:
        break;
    }
}

inline void drawStatusBar(
     std::set<std::filesystem::path> &selectedPaths
    ,bool &selectButtonFlag
)
{
    if (ImGui::Button("Select"))
        selectButtonFlag = true;
    ImGui::SameLine();
    ImGui::Text("%s", std::string("Select " + std::to_string(selectedPaths.size()) + " items").c_str());
}
}

namespace ImGui {

inline bool FileExplorer(
      std::string_view title
    , bool &openFlag
    , ImGuiWindowFlags &flags
    , bool &showHiddenFiles
    , std::filesystem::path &currentPath
    , std::set<std::filesystem::path> &favoritesPath
    , std::set<std::filesystem::path> &selectedItems
    , ImGui::FileExplorerViewMode viewMode
)
{
    bool selectButtonFlag = false;
    
    if (openFlag)
    {
        ImGui::Begin("Filesystem", &openFlag, flags);

        // Draw current directory path
        {
            ImGui::BeginChild("##Current directory path controls", {}, ImGuiChildFlags_AutoResizeY + ImGuiChildFlags_AutoResizeX + ImGuiChildFlags_Border);
            drawPathControls(currentPath);
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("##Current directory path", {}, ImGuiChildFlags_AutoResizeY + ImGuiChildFlags_Border);
            drawPath(currentPath);
            ImGui::EndChild();
        }

        // Draw control buttons and status bar
        {
            ImGui::BeginChild("##StatusBar", {}, ImGuiChildFlags_AutoResizeY + ImGuiChildFlags_Border);
            drawStatusBar(selectedItems, selectButtonFlag);
            ImGui::EndChild();
        }

        // Main widget
        {
            ImGui::BeginChild("##MainWidget", {},  ImGuiChildFlags_Border);            
            ImGui::Columns(2);

            // Draw favorite paths
            {
                ImGui::BeginChild("##FavoritePaths", {}, ImGuiChildFlags_None);
                drawFavorites(currentPath, favoritesPath);
                ImGui::EndChild();
            }

            ImGui::NextColumn();

            // Draw items in current directory
            {
                ImGui::BeginChild("##FileExplorer", {}, ImGuiChildFlags_None);
                drawFiles(currentPath, selectedItems, viewMode);
                ImGui::EndChild();
            }

            ImGui::EndChild();
            ImGui::Columns(1);
        }

        ImGui::End();
    }
    return selectButtonFlag;
}
}

#endif // IMGUIFILEEXPLORER_H