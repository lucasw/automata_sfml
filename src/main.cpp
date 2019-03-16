#include <SFML/Graphics.hpp>
#include <iostream>

class Person
{

};

class Land
{
public:
  Land(const std::string& path)
  {
    if (!map_tex_.loadFromFile(path + "map.png")) {
      throw std::runtime_error("couldn't load image from disk " + path);
    }
    sprite_.setTexture(map_tex_);

    const auto width = map_tex_.getSize().x;
    const auto height = map_tex_.getSize().y;

    std::cout << width << " " << height << "\n";
  }

  sf::Texture map_tex_;
  sf::Sprite sprite_;
};

int main(int argn, char* argv[])
{
  sf::RenderWindow window(sf::VideoMode(1000, 800), "automata");
  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);

  Land land("../automata_sfml/data/");

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    window.clear();
    // window.draw(shape);
    window.draw(land.sprite_);
    window.display();
  }

  return 0;
}
