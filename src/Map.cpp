#include "Map.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

// 定義靜態成員變數
const std::string Map::ORANGE = "\033[48;2;255;165;0m";
const std::string Map::WHITE = "\033[48;2;255;255;255m";
const std::string Map::RESET = "\033[0m";

std::vector<std::vector<int>> Map::LoadMap(const std::string& filename) {
    std::vector<std::vector<int>> matrix;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return matrix;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<int> row;
        int number;

        while (ss >> number) {
            row.push_back(number);
        }

        if (!row.empty()) {
            matrix.push_back(row);
        }
    }

    file.close();
    return matrix;
}
void Map::RenderMap(const std::vector<std::vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& cell : row) {
            if (cell == 1) {
                std::cout << ORANGE << "  " << RESET;
            } else {
                std::cout << WHITE << "  " << RESET;
            }
        }
        std::cout << std::endl;
    }
}