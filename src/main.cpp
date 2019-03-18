#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

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
    x_move_max_ = rand() % 20 - 10;
    y_move_max_ = rand() % 20 - 10;
  }

  void update(cv::Mat& map)
  {
    ++tick_count_;
    if (tick_count_ % ticks_to_move_ != 0) {
      return;
    }
    // TODO(lucasw) handle swimming

    ++x_move_count_;
    ++y_move_count_;
    size_t nx = x_, ny = y_;
    if (y_move_count_ > std::abs(y_move_max_)) {
      y_move_count_ = 0;
      if (y_move_max_ > 0) {
        ++ny;
      } else if (y_move_max_ < 0) {
        --ny;
      }
      if (ny < map.rows) {
        const auto val = map.at<uint8_t>(ny, x_);
        if (val > 0) {
          y_ = ny;
        } else {
          notVeryRandomNewDir();
        }
      } else {
        notVeryRandomNewDir();
      }
    }
    if (x_move_count_ > std::abs(x_move_max_)) {
      x_move_count_ = 0;
      if (x_move_max_ > 0) {
        ++nx;
      } else if (x_move_max_ < 0) {
        --nx;
      }

      if (nx < map.cols) {
        const auto val = map.at<uint8_t>(y_, nx);
        if (val > 0) {
          x_ = nx;
        } else {
          notVeryRandomNewDir();
        }
      } else {
        notVeryRandomNewDir();
      }
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
  int x_move_count_ = 0;
  int x_move_max_ = 4;
  int y_move_count_ = 0;
  int y_move_max_ = 0;

  size_t tick_count_ = 0;
  size_t ticks_to_move_ = 3;
  size_t x_;
  size_t y_;

  size_t change_dir_max_ = 41;
  size_t change_dir_count_ = 0;

  size_t spawn_count_ = 0;
  size_t spawn_max_ = 73;
};

class Land
{
public:
  Land(const std::string& path)
  {
    cv::Mat map = cv::imread(path + "map.png", cv::IMREAD_GRAYSCALE);
    if (map.empty()) {
      throw std::runtime_error("couldn't load image from disk " + path);
    }

    cv::resize(map, map_, cv::Size(320, 200), cv::INTER_NEAREST);

    const auto width = map_.cols;
    const auto height = map_.rows;

    std::cout << width << " " << height << "\n";

    people_.emplace_back(Person(width/2, height/2));
  }

  void update()
  {
    std::vector<Person> new_people;
    for(auto& person : people_) {
      person.update(map_);
      if ((people_.size() < people_limit_) && person.spawn()) {
        new_people.emplace_back(Person(person.x_, person.y_));
        new_people[new_people.size() - 1].spawn_max_ += rand() % 23;
      }
    }

    if (new_people.size() > 0) {
      std::cout << people_.size() << " new people " << new_people.size() << "\n";
      people_.insert(people_.end(), new_people.begin(), new_people.end());
    }

  }

  void draw()
  {
    const auto color = cv::Vec3b(255, 0, 0);
    cv::cvtColor(map_, image_, cv::COLOR_GRAY2RGB);
    for(auto& person : people_) {
      image_.at<cv::Vec3b>(person.y_, person.x_) = color;
    }
  }

  std::vector<Person> people_;

  cv::Mat image_;
  cv::Mat map_;

  const size_t people_limit_ = 210000;
};

int main(int argn, char* argv[])
{
  Land land("../automata_sfml/data/");

  cv::Mat image;
  land.draw();
  cv::resize(land.image_, image, cv::Size(), 3.0, 3.0, cv::INTER_NEAREST);
  while (true) {
    land.update();
    land.draw();
    cv::resize(land.image_, image, cv::Size(), 3.0, 3.0, cv::INTER_NEAREST);
    cv::imshow("automata", image);
    const int key = cv::waitKey(10);
    if (key == 'q') {
      break;
    }
    // std::this_thread::sleep_for(10ms);
  }

  return 0;
}
