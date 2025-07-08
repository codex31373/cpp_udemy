#include <iostream>


struct Point
{
  int x{ 0 }, y{ 0 };

  Point(){}
  
  Point(const int x, const int y) : x{x}, y{y} {}
};

struct Line
{
  Point *start, *end;
  
  Line(Point* const start, Point* const end)
    : start(start), end(end)
  {
  }

  ~Line()
  {
    delete start;
    delete end;
  }

  Line deep_copy() const
  {
    return Line(new Point(*start), new Point(*end));
  }
};

//serialization and deserialization

int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\n7 July 2025\n"
              << "\033[0m" << std::endl;

    return 0;
}