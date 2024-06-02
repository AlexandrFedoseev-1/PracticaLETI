#include "datareader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

// Функция для обработки строки и извлечения координат и имени
void parseBorderLine(std::string& line, Data& data, size_t& posB1)  {
    double x1, y1, x2, y2;
    std::string name;
    line = line.substr(posB1);
    if (sscanf_s(line.c_str(), "%[^(](%lf,%lf) X(%lf);", name.c_str(), sizeof(name.c_str()), &x1, &y1, &x2) == 4) {
        data.borderLines.push_back({ {x1, y1}, {x2, y1}, name.c_str() });

    }
    else if (sscanf_s(line.c_str(), "%[^(](%lf,%lf) Y(%lf);", name.c_str(), sizeof(name.c_str()), &x1, &y1, &y2) == 4) {
        data.borderLines.push_back({ {x1, y1}, {x1, y2}, name.c_str()});

    }
}
void parseWireLine(std::string& line, Data& data, size_t& pos) {
    double x1, y1, x2, y2;
    std::string nameWire, wwire, type;
    line = line.substr(pos);
    if (sscanf_s(line.c_str(), "Wire(%[^)]); W_WIRE(%[^)]) %[^(](%lf,%lf) X(%lf);", nameWire.c_str(), sizeof(nameWire.c_str()), wwire.c_str(), sizeof(wwire.c_str()), type.c_str(), sizeof(type.c_str()), &x1, &y1, &x2) == 6) {
        std::string name = std::string(nameWire.c_str()) + "_" + std::string(type.c_str());
        std::string fullname = "Wire(" + std::string(nameWire.c_str()) + "); W_WIRE(" + std::string(wwire.c_str()) + ") " + std::string(type.c_str());
        data.elementsLines.push_back({ {x1, y1}, {x2, y1}, name, fullname});

    }
    else if (sscanf_s(line.c_str(), "Wire(%[^)]); W_WIRE(%[^)]) %[^(](%lf,%lf) Y(%lf);", nameWire.c_str(), sizeof(nameWire.c_str()), wwire.c_str(), sizeof(wwire.c_str()), type.c_str(), sizeof(type.c_str()), &x1, &y1, &y2) == 6) {
        std::string name =  std::string(nameWire.c_str()) + "_" + std::string(type.c_str());
        std::string fullname = "Wire(" + std::string(nameWire.c_str()) + "); W_WIRE(" + std::string(wwire.c_str()) + ") " + std::string(type.c_str());
        data.elementsLines.push_back({ {x1, y1}, {x1, y2}, name, fullname });

    }
}
void parseWWireLine(std::string& line, Data& data, size_t& pos) {
    double x1, y1, x2, y2;
    std::string  wwire, type;
    line = line.substr(pos);
    if (sscanf_s(line.c_str(), "W_WIRE(%[^)]) %[^(](%lf,%lf) X(%lf);", wwire.c_str(), sizeof(wwire.c_str()), type.c_str(), sizeof(type.c_str()), &x1, &y1, &x2) == 5) {
        std::string name = std::string(type.c_str());
        std::string fullname = "W_WIRE(" + std::string(wwire.c_str()) + ") " + std::string(type.c_str());
        data.elementsLines.push_back({ {x1, y1}, {x2, y1}, name, fullname });

    }
    else if (sscanf_s(line.c_str(), "W_WIRE(%[^)]) %[^(](%lf,%lf) Y(%lf);", wwire.c_str(), sizeof(wwire.c_str()), type.c_str(), sizeof(type.c_str()), &x1, &y1, &y2) == 5) {
        std::string name = std::string(type.c_str());
        std::string fullname = "W_WIRE(" + std::string(wwire.c_str()) + ") " + std::string(type.c_str());
        data.elementsLines.push_back({ {x1, y1}, {x1, y2}, name, fullname });

    }
}

void parsePointName(std::string& line, Data& data, size_t& pos) {
    double x1, y1;
    char name[256];
    line = line.substr(pos);

    if (sscanf_s(line.c_str(), "TB(%lf,%lf, \"%[^\"]\");", &x1, &y1, name, (unsigned)_countof(name)) == 3) {
        data.cellPoints.push_back({ {x1, y1}, std::string(name) });
    }
    else if (sscanf_s(line.c_str(), "OR(%*[^)]) %[^(](%lf,%lf);", name, (unsigned)_countof(name), &x1, &y1) == 3) {
        data.contactPoints.push_back({ {x1, y1}, std::string(name) });
    }
}

Data readLinesFromFile(const std::string& filename) {
    Data data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find("B1(");
        if (pos != std::string::npos)
        {
            parseBorderLine(line, data, pos);
            continue;
        }
        pos = line.find("Wire(");
        if (pos != std::string::npos)
        {
            parseWireLine(line, data, pos);
            continue;
        }
        pos = line.find("W_WIRE(");
        if (pos != std::string::npos)
        {
            parseWWireLine(line, data, pos);
            continue;
        }
        pos = line.find("TB(");
        if (pos != std::string::npos)
        {
            parsePointName(line, data, pos);
            continue;
        }
        pos = line.find("OR(");
        if (pos != std::string::npos)
        {
            parsePointName(line, data, pos);
            continue;
        }
    }

    file.close();
    return data;
}


// Функция для нахождения точек пересечения линий
std::vector<Point> findIntersectionPoints(const std::vector<Line>& lines) {
    std::vector<Point> intersectionPoints;
    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].name=="B1")
        {
            for (size_t j = i + 1; j < lines.size(); ++j) {
                if (lines[j].name == "B1")
                {
                    const Line& line1 = lines[i];
                    const Line& line2 = lines[j];
                    // Проверка на пересечение линий
                    double det = (line1.end.x - line1.start.x) * (line2.end.y - line2.start.y) -
                                 (line2.end.x - line2.start.x) * (line1.end.y - line1.start.y);
                    if (det == 0) continue; // Линии параллельны
                    double t = ((line2.start.x - line1.start.x) * (line2.end.y - line2.start.y) -
                                (line2.start.y - line1.start.y) * (line2.end.x - line2.start.x)) / det;
                    double u = -((line1.start.x - line1.end.x) * (line1.start.y - line2.start.y) -
                                 (line1.start.y - line1.end.y) * (line1.start.x - line2.start.x)) / det;
                    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
                        double x = line1.start.x + t * (line1.end.x - line1.start.x);
                        double y = line1.start.y + t * (line1.end.y - line1.start.y);
                        intersectionPoints.push_back({ x, y });
                    }
                }
            }
        }

    }

    std::sort(intersectionPoints.begin(), intersectionPoints.end(), [](const Point& a, const Point& b) {
        return a.x < b.x;
    });

    return intersectionPoints;
}


bool pointInsideCell(const Point& point, const Point& bottomLeft, const Point& topRight) {
    // Проверяем, находится ли точка внутри прямоугольника, образованного ячейкой
    return point.x >= bottomLeft.x && point.x <= topRight.x &&
           point.y >= bottomLeft.y && point.y <= topRight.y;
}
bool elementIntersectionCell(const Line& line, const Point& bottomLeft, const Point& topRight) {
    // Проверяем, находится ли точка внутри прямоугольника, образованного ячейкой
    return line.start.x < bottomLeft.x && line.end.x > topRight.x &&
               line.start.y > bottomLeft.y && line.start.y < topRight.y ||
           line.start.x > topRight.x && line.end.x < bottomLeft.x &&
               line.start.y > bottomLeft.y && line.start.y < topRight.y ||
           line.start.y < bottomLeft.y && line.end.y > topRight.y &&
               line.start.x < bottomLeft.x && line.start.x > topRight.x ||
           line.start.y > topRight.y && line.end.y < bottomLeft.y &&
               line.start.x < bottomLeft.x && line.start.x > topRight.x;
}
Point getLimitPoint(const Point& bottomLeft, const Point& topRight, Point& elementPoint) {
    if (bottomLeft.x > elementPoint.x) elementPoint.x = bottomLeft.x;
    if (bottomLeft.y > elementPoint.y) elementPoint.y = bottomLeft.y;
    if (topRight.x < elementPoint.x) elementPoint.x = topRight.x;
    if (topRight.y < elementPoint.y) elementPoint.y = topRight.y;
    return elementPoint;
}

Line getLimitElementsLine(const Point& bottomLeft, const Point& topRight, const Line& elementsLine) {
    Line line = elementsLine;
    line.start = getLimitPoint(bottomLeft, topRight, line.start);
    line.end = getLimitPoint(bottomLeft, topRight, line.end);
    return line;
}

std::vector<Line> findElementsInsideCell(const Point& bottomLeft, const Point& topRight, const std::vector<Line>& elementsLines) {
    std::vector<Line> linesInsideCells;

    for (const auto& line : elementsLines) {
        // Проверяем, находятся ли начальная и конечная точки линии внутри текущей ячейки
        if (pointInsideCell(line.start, bottomLeft, topRight) || pointInsideCell(line.end, bottomLeft, topRight) || elementIntersectionCell(line,bottomLeft,topRight)) {
            linesInsideCells.push_back(getLimitElementsLine(bottomLeft, topRight, line));
        }
    }

    return linesInsideCells;
}

std::vector<NamePoint> findContactsInsideCell(const Point& bottomLeft, const Point& topRight, const std::vector<NamePoint>& contacts) {
    std::vector<NamePoint> contInsideCell;
    for (const auto& contPoint : contacts) {
        // Проверяем, находится ли точка внутри текущей ячейки
        if (pointInsideCell(contPoint.coord, bottomLeft, topRight)) {
            contInsideCell.push_back(contPoint);
        }
    }
    return contInsideCell;
}

std::string findCellName(const Point& bottomLeft, const Point& topRight, Data data) {
    for (const auto& namePoint : data.cellPoints) {
        if (bottomLeft.x < namePoint.coord.x && bottomLeft.y < namePoint.coord.y && topRight.y > namePoint.coord.y && topRight.x > namePoint.coord.x) return namePoint.name;
    }
    return "";
}
std::string checkStrClmCellName(const Point& bottomLeft, const Point& topRight, const std::vector<NamePoint> namePoint) {
    for (const auto& namePoint : namePoint) {
        if (bottomLeft.x < namePoint.coord.x && bottomLeft.y < namePoint.coord.y && topRight.y > namePoint.coord.y && topRight.x > namePoint.coord.x) return namePoint.name;
    }
    return "";
}


std::string getCellsSpecification(const Point& bottomLeft, const Point& topRight, std::vector<Line>& elementsLines) {
    std::string specification;
    std::string names;
    int i = 0;
    for (auto& el : elementsLines) {
        std::ostringstream x, y;
        y << std::fixed << std::setprecision(2) << el.start.y;
        x << std::fixed << std::setprecision(2) << el.start.x;
        if (names.find(el.name) == std::string::npos) {
            if (bottomLeft.x == el.start.x || bottomLeft.x == el.end.x)
            {
                specification += el.name + "--lft--" + y.str() + "\n";
            }
            if (bottomLeft.y == el.start.y || bottomLeft.y == el.end.y)
            {
                specification += el.name + "--btm--" + x.str() + "\n";
            }
            if (topRight.y == el.start.y || topRight.y == el.end.y)
            {
                specification += el.name + "--top--" + x.str() + "\n";
            }
            if (topRight.x == el.start.x || topRight.x == el.end.x)
            {
                specification += el.name + "--rht--" + y.str() + "\n";
            }
        }
        else
        {
            i++;
            std::string name = el.name;
            if (bottomLeft.x == el.start.x || bottomLeft.x == el.end.x)
            {
                el.name = name + "_" + std::to_string(i);
                specification += el.name + "--lft--" + y.str() + "\n";

            }
            if (bottomLeft.y == el.start.y || bottomLeft.y == el.end.y)
            {
                el.name = name + "_" + std::to_string(i);
                specification += el.name + "--btm--" + x.str() + "\n";
            }
            if (topRight.y == el.start.y || topRight.y == el.end.y)
            {
                el.name = name + "_" + std::to_string(i);
                specification += el.name + "--top--" + x.str() + "\n";
            }
            if (topRight.x == el.start.x || topRight.x == el.end.x)
            {
                el.name = name + "_" + std::to_string(i);
                specification += el.name + "--rht--" + y.str() + "\n";
            }
        }
        names += el.name;
    }
    return specification;
}

std::string getNewCellsSpecification(const Cell& cell) {
    std::string newSpecification;
    std::vector<Line> el = cell.elementsLines;
    newSpecification = getCellsSpecification(cell.bottomLeft, cell.topRight, el);
    return newSpecification;
}


std::vector<std::vector<NamePoint>>  generateNameCell(Data& data) {
    std::vector<NamePoint> strname, colname;
    std::vector<std::vector<NamePoint>> strAndclm;
    auto iter{ data.cellPoints.begin() };
    for (int i = 0; i < data.cellPoints.size(); ++i)
    {
        if (data.cellPoints.at(i).name.find("S") != std::string::npos)
        {
            data.strings.push_back(data.cellPoints.at(i));
            std::string name1, name;
            if (sscanf_s(data.cellPoints.at(i).name.c_str(), "S%s", name1.c_str(), sizeof(name1.c_str())) == 1) {
                name = "_" + std::string(name1.c_str());
                strname.push_back({ data.cellPoints.at(i).coord, name.c_str() });
            }
            iter = data.cellPoints.erase(iter);
            i--;
        }
        else if (data.cellPoints.at(i).name.find("C") != std::string::npos)
        {
            std::string name1,name2,name;
            if (sscanf_s(data.cellPoints.at(i).name.c_str(), "C%s %s", name1.c_str(), sizeof(name1.c_str()), name2.c_str(), sizeof(name2.c_str())) == 2) {
                name = "_" + std::string(name1.c_str()) + ", " + std::string(name2.c_str());
                colname.push_back({ data.cellPoints.at(i).coord, name});
            }
            iter = data.cellPoints.erase(iter);
            i--;
        }
        else
        {
            ++iter;
        }
    }
    for (auto& string : data.strings)
    {
        string.name += " g:";
    }
    strAndclm.push_back(strname);
    strAndclm.push_back(colname);

    return strAndclm;
}




std::vector<Cell> findCellCoordinates(const std::vector<Point>& intersectionPoints, Data& data) {
    std::vector<std::vector<NamePoint>> strAndClm = generateNameCell(data);
    std::vector<NamePoint> str = strAndClm.at(0);
    std::vector<NamePoint> clm = strAndClm.at(1);
    std::vector<NamePoint> checkstrclm = str;
    std::vector<Line> strclmcelsize;
    Line elch;
    int count = 0;

    checkstrclm.insert(checkstrclm.end(), clm.begin(), clm.end());
    for (size_t i = 0; i < intersectionPoints.size(); ++i) {
        for (size_t j = i + 1; j < intersectionPoints.size(); ++j) {
            if (intersectionPoints[j].x <= intersectionPoints[i].x ||
                intersectionPoints[j].y <= intersectionPoints[i].y) {
                continue;
            }
            std::string scname = checkStrClmCellName(intersectionPoints[i], intersectionPoints[j], checkstrclm);
            if (scname!="")
            {

                if (!count)
                {
                    strclmcelsize.push_back({ intersectionPoints[i], intersectionPoints[j], scname });
                    elch = strclmcelsize.at(count);
                    count++;
                }

                if (scname != elch.name)
                {
                    strclmcelsize.push_back({ intersectionPoints[i], intersectionPoints[j], scname });
                    elch = strclmcelsize.at(count);
                    count++;
                }


                continue;
            }


            std::string name = findCellName(intersectionPoints[i], intersectionPoints[j], data);
            std::vector<Line> elements = findElementsInsideCell(intersectionPoints[i], intersectionPoints[j], data.elementsLines);
            std::vector<NamePoint> contacts = findContactsInsideCell(intersectionPoints[i], intersectionPoints[j], data.contactPoints);
            std::string specification = getCellsSpecification(intersectionPoints[i], intersectionPoints[j], elements);

            data.cells.push_back({ intersectionPoints[i], intersectionPoints[j], name, elements, contacts, specification });
            break;
        }
    }
    for (auto& cell : data.cells)
    {
        for (auto& line : strclmcelsize)
        {
            if (cell.bottomLeft.y==line.start.y  || cell.bottomLeft.x == line.start.x )
            {
                cell.name += line.name;
            }
        }
        for (auto& string : data.strings)
        {
            if (cell.bottomLeft.y < string.coord.y && cell.topRight.y > string.coord.y)
            {
                string.name += " " + cell.name + ";";
            }
        }
    }
    std::sort(data.strings.begin(), data.strings.end(), [](const NamePoint& point1, const NamePoint& point2) {
        return  point1.coord.y > point2.coord.y;
    });
    std::sort(data.cells.begin(), data.cells.end(), [](const Cell& cell1, const Cell& cell2) {
        return  cell1.bottomLeft.y > cell2.bottomLeft.y;
    });
    return data.cells;
}



void moveCellToOrigin(Cell& cell) {
    // Найдем сдвиг для перемещения нижнего левого угла ячейки в (0,0)
    double offsetX = cell.bottomLeft.x;
    double offsetY = cell.bottomLeft.y;

    // Применим сдвиг ко всем точкам границ ячейки
    cell.bottomLeft.x -= offsetX;
    cell.bottomLeft.y -= offsetY;
    cell.topRight.x -= offsetX;
    cell.topRight.y -= offsetY;

    // Применим сдвиг ко всем точкам внутри ячейки
    for (auto& line : cell.elementsLines) {
        line.start.x -= offsetX;
        line.start.y -= offsetY;
        line.end.x -= offsetX;
        line.end.y -= offsetY;
    }
    for (auto& contact : cell.contactPoints) {
        contact.coord.x -= offsetX;
        contact.coord.y -= offsetY;
    }
    cell.specification = getNewCellsSpecification(cell);
}

std::vector<Cell> moveAllCellsToOrigin(std::vector<Cell>& cells) {
    std::vector<Cell> newCells = cells;
    // Применим сдвиг ко всем элементам внутри каждой ячейки
    for (auto& cell : newCells) {
        moveCellToOrigin(cell);
    }
    return newCells;
}
