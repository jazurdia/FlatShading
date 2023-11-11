#pragma once
#include <glm/glm.hpp>
#include <cstdint>
#include <vector>
#include <array>
#include <algorithm>
#include <sstream>
#include <fstream>

// CONST
// screen witdh and height
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;


// define un struct color que contiene 3 floats
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

Color currentColor = {255, 255, 255, 255};
Color clearColor = {0, 0, 0, 255};

// setColor
void setColor(Color color) {
    currentColor = color;
}


// Point, but instead of using x, y, we use glm::vec3
struct Point {
    glm::vec3 position; // Cambié esto para usar glm::vec3
};


// function to draw a point
void drawPoint(Point p) {
    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    SDL_RenderDrawPoint(renderer, static_cast<int>(p.position.x), static_cast<int>(p.position.y)); // Utilizo solo x e y para SDL_RenderDrawPoint
}

// dont know why do we need this. I think it may be useful later.
void pixel(int x, int y) {
    // Set the pixel at coordinates (x, y)
    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    SDL_RenderDrawPoint(renderer, x + 200, y);
}

// Line that has a start and an end, both are glm::vec3
struct Line {
    glm::vec3 start;
    glm::vec3 end;
};

// function to draw a line
void drawLine(Line line) {
    int x1 = round(line.start.x), y1 = round(line.start.y);
    int x2 = round(line.end.x), y2 = round(line.end.y);

    // Draw a line between (x1, y1) and (x2, y2) using point
    // Bresenham's line algorithm
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    while (true) {
        pixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x1 += sx; }
        if (e2 < dy) { err += dx; y1 += sy; }
    }
}

// Triangle that has 3 points, all of them are glm::vec3. We are only going to be using triangles so we dont need a polygon struct
struct Triangle {
    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 c;
};

// function to draw a triangle
void drawTriangle(Triangle triangle) {
    drawLine({triangle.a, triangle.b});
    drawLine({triangle.b, triangle.c});
    drawLine({triangle.c, triangle.a});
}

// Face Structure, it has 3 points, all of them are glm::vec3
struct Face {
    std::vector<std::array<int, 3>> vertexIndices;
};


// load object function
bool loadOBJ(const std::string& path, std::vector<glm::vec3>& out_vertices, std::vector<Face>& out_faces) {
    // Open the OBJ file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening OBJ file: " << path << std::endl;
        return false;
    }

    // Variables to store the vertices and faces from the OBJ file
    std::vector<glm::vec3> vertices;
    std::vector<Face> faces;

    // Read the file line by line
    std::string line;
    while (std::getline(file, line)) {
        // Split the line into tokens
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        // Process vertex data
        if (type == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
            // Process face data
        else if (type == "f") {
            Face face;
            std::string vertexStr;
            while (iss >> vertexStr) {
                std::array<int, 3> vertexIndices;
                std::replace(vertexStr.begin(), vertexStr.end(), '/', ' ');
                std::istringstream viss(vertexStr);
                viss >> vertexIndices[0] >> vertexIndices[1] >> vertexIndices[2];
                face.vertexIndices.push_back(vertexIndices);
            }
            faces.push_back(face);
        }
    }

    // Close the file
    file.close();

    // Return the extracted vertices and faces
    out_vertices = vertices;
    out_faces = faces;
    return true;
}