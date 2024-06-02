#ifndef DATAREADER_H
#define DATAREADER_H

#include <vector>
#include <string>
#include <vector>


struct Point {
    double x;
    double y;
};


struct Line {
    Point start;
    Point end;
    std::string name;
    std::string fullname;
};

struct NamePoint {
    Point coord;
    std::string name;
};
struct ElementSpecification {
    std::string originalName;
    std::string specification;
};
struct Cell {
    Point bottomLeft;
    Point topRight;
    std::string name;
    std::vector<Line> elementsLines;
    std::vector<NamePoint> contactPoints;
    std::string specification;
};

struct Data {
    std::vector<Line> borderLines;
    std::vector<Line> elementsLines;
    std::vector<Cell> cells;
    std::vector<NamePoint> cellPoints;
    std::vector<NamePoint> contactPoints;
    std::vector<NamePoint> strings;
};



Data readLinesFromFile(const std::string& filename);
std::vector<Point> findIntersectionPoints(const std::vector<Line>& lines);
std::vector<Cell> findCellCoordinates(const std::vector<Point>& intersectionPoints, Data& data);
std::vector<Cell> moveAllCellsToOrigin(std::vector<Cell>& cells);
#endif // DATAREADER_H
