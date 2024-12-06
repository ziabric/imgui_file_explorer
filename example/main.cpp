#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <ImGuiFileExplorer.hpp>

int main()
{
    bool exampleWindowOpenFlag = true;
    bool showOpenFilesFlag = true;
    std::filesystem::path homePath = std::filesystem::current_path();
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
    std::set<std::filesystem::path> selectedItems = {};
    std::set<std::filesystem::path> fav = {
         "/home"
        ,"/tmp"
        ,"/etc/ssh"
        ,"/"
    };

    sf::RenderWindow window(sf::VideoMode({640, 480}), "Example");
    window.setFramerateLimit(24);
    if (!ImGui::SFML::Init(window))
        return -1;

    sf::Event event;

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::FileExplorer(
            "Filesystem",
            exampleWindowOpenFlag,
            windowFlags,
            showOpenFilesFlag,
            homePath,
            fav,
            selectedItems,
            ImGui::FileExplorerViewMode::Rows
        );

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}