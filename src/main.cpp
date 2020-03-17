#include <iostream>

#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <docopt/docopt.h>

static constexpr auto USAGE =
  R"(Naval Fate.

    Usage:
          naval_fate start
          naval_fate ship new <name>...
          naval_fate ship <name> move <x> <y> [--speed=<kn>]
          naval_fate ship shoot <x> <y>
          naval_fate mine (set|remove) <x> <y> [--moored | --drifting]
          naval_fate (-h | --help)
          naval_fate --version
 Options:
          -h --help     Show this screen.
          --version     Show version.
          --speed=<kn>  Speed in knots [default: 10].
          --moored      Moored (anchored) mine.
          --drifting    Drifting mine.
)";

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
      { std::next(argv), std::next(argv, argc) },
      true,// show help if requested
      "Naval Fate 2.0");// version string

    for (auto const &arg : args) {
      std::cout << arg.first << arg.second << std::endl;
    }


  //Use the default logger (stdout, multi-threaded, colored)
  //spdlog::info("Hello, {}!", "World");


  sf::RenderWindow window(sf::VideoMode(1024, 768), "Some game!");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  constexpr auto scale_factor = 2.0;
  ImGui::GetStyle().ScaleAllSizes(scale_factor);
  ImGui::GetIO().FontGlobalScale = scale_factor;

  std::array<bool, 12> states{};

  sf::Clock deltaClock;
  while (window.isOpen()) {
    sf::Event event{};
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    ImGui::SFML::Update(window, deltaClock.restart());


    ImGui::Begin("The Plan");

    int index = 0;
    for (const auto &step : { "Understand what is going on", "Adjust the game", "Render a shader", "Profit! (Stonks)" }) {
      ImGui::Checkbox(fmt::format("{} : {}", index, step).c_str(), std::next(begin(states), index));
      ++index;
    }

    bool can_shade = sf::Shader::isAvailable();
    ImGui::Text(can_shade ? "Shader is available" : "Shader is NOT available");

    ImGui::End();

    window.clear();
    ImGui::SFML::Render(window);

    if (can_shade) {
      // draw some shadurs
      sf::Shader shader;
      if(shader.loadFromFile("../../src/shader/rainbow.frag", sf::Shader::Fragment)) {
        window.draw(shader);
      }
    }

    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}
