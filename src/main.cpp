#include <SFML/Graphics.hpp>
#include <iostream>

class Person
{
public:
  Person(const size_t& x, const size_t& y) :
    x_(x),
    y_(y)
  {
  }

  void notVeryRandomNewDir()
  {
    // std::cout << "new dir " << dir_ << " ";
    dir_ = (move_count_ / 7 + dir_ + 1) % 5;
    // std::cout << dir_ << "\n";
  }

  void update(sf::Image& map)
  {
    ++tick_count_;
    if (tick_count_ % ticks_to_move_ != 0) {
      return;
    }
    sf::Color color = map.getPixel(x_, y_);
    // TODO(lucasw) handle swimming

    // auto n =
    if (dir_ == 0) {
      // stand still
    } else if (dir_ == 1) {
      if (x_ < map.getSize().x - 1) {
        sf::Color color = map.getPixel(x_ + 1, y_);
        if (color.r > 0) {
          x_ += 1;
        } else {
          notVeryRandomNewDir();
        }
      } else {
        notVeryRandomNewDir();
      }
    } else if (dir_ == 3) {
      if (x_ > 0) {
        sf::Color color = map.getPixel(x_ - 1, y_);
        if (color.r > 0) {
          x_ -= 1;
        } else {
          notVeryRandomNewDir();
        }
      } else {
        notVeryRandomNewDir();
      }
    } else if (dir_ == 2) {
      if (y_ < map.getSize().y - 1) {
        sf::Color color = map.getPixel(x_, y_ + 1);
        if (color.r > 0) {
          y_ += 1;
        } else {
          notVeryRandomNewDir();
        }
      } else {
        notVeryRandomNewDir();
      }
    } else if (dir_ == 4) {
      if (y_ > 0) {
        sf::Color color = map.getPixel(x_, y_ - 1);
        if (color.r > 0) {
          y_ -= 1;
        } else {
          notVeryRandomNewDir();
        }
      } else {
        notVeryRandomNewDir();
      }
    } else {
      notVeryRandomNewDir();
    }

    if (move_count_ % change_dir_max_ == 0) {
      notVeryRandomNewDir();
    }

    ++move_count_;
  }

  bool spawn()
  {
    ++spawn_count_;
    if (spawn_count_ % spawn_max_ == spawn_max_ - 1) {
      return true;
    }
    return false;
  }

  size_t move_count_ = 0;
  size_t tick_count_ = 0;
  size_t ticks_to_move_ = 33;
  size_t x_;
  size_t y_;
  size_t dir_ = 1;

  size_t change_dir_max_ = 41;
  size_t change_dir_count_ = 0;

  size_t spawn_count_ = 0;
  size_t spawn_max_ = 2713;
};

class Land
{
public:
  Land(const std::string& path)
  {
    if (!map_tex_.loadFromFile(path + "map.png")) {
      throw std::runtime_error("couldn't load image from disk " + path);
    }
    map_image_ = map_tex_.copyToImage();
    sprite_.setTexture(map_tex_);

    const auto width = map_tex_.getSize().x;
    const auto height = map_tex_.getSize().y;

    std::cout << width << " " << height << "\n";

    people_.emplace_back(Person(width/2, height/2));
  }

  void update()
  {
    std::vector<Person> new_people;
    for(auto& person : people_) {
      person.update(map_image_);
      if (person.spawn()) {
        new_people.emplace_back(Person(person.x_, person.y_));
      }
    }

    if (new_people.size() > 0) {
      std::cout << people_.size() << " new people " << new_people.size() << "\n";
      people_.insert(people_.end(), new_people.begin(), new_people.end());
    }

  }

  void draw(sf::RenderWindow& window)
  {
    window.draw(sprite_);

    std::vector<sf::Vertex> vertices;
    const auto color = sf::Color(255, 10, 10);
    for(auto& person : people_) {
      vertices.push_back(sf::Vertex(sf::Vector2f(person.x_, person.y_), color));
    }
    window.draw(&vertices[0], vertices.size(), sf::Points);
  }

  std::vector<Person> people_;

  sf::Image map_image_;
  sf::Texture map_tex_;
  sf::Sprite sprite_;
};

int main(int argn, char* argv[])
{
  sf::RenderWindow window(sf::VideoMode(1000, 800), "automata");

  Land land("../automata_sfml/data/");

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    land.update();
    window.clear();
    land.draw(window);
    window.display();
  }

  return 0;
}
