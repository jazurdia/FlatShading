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

bool init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("Software Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer) {
        std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}


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



// Don't know why do we need this. I think it may be useful later.
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

std::vector<glm::vec3> setupVertexArray(const std::vector<glm::vec3>& vertices, const std::vector<Face>& faces) {
    std::vector<glm::vec3> vertexArray;
    for (const auto& face : faces) { //for each face
        for (const auto& vertexIndices : face.vertexIndices) { // for each vertex face
            // Get the vertex position and normal from the input arrays using the indices from the face
            vertexArray.push_back(vertices[vertexIndices[0] - 1]);
        }
    }
    return vertexArray;
}

struct Fragment {
    glm::ivec2 position; // x & y positions in screen space.
    // Other interpolated attributes (e.g., color, texture coordinates, normals) can be added here

    // color
    Color color;

};

struct Uniforms { // Stores the parameters for the shader
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};

// Vertex Shader function
glm::vec3 vertexShader(const glm::vec3& vertex, const Uniforms& uniforms) {
    // Apply transformations to the input vertex using the matrices from the uniforms
    glm::vec4 transformedVertex = uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex, 1.0f);

    // Return the transformed vertex as a vec3
    return glm::vec3(transformedVertex);
}

// Primitive Assembly function
std::vector<std::vector<glm::vec3>> primitiveAssembly(const std::vector<glm::vec3>& transformedVertices) {
    // Group your vertices in groups of 3
    std::vector<std::vector<glm::vec3>> primitives;
    for (int i = 0; i < transformedVertices.size(); i += 3) {
        primitives.push_back({transformedVertices[i], transformedVertices[i + 1], transformedVertices[i + 2]});
    }
    // Return your assembled vertices
    return primitives;
}

// Función para convertir un triángulo en fragmentos
std::vector<Fragment> convertTriangleToFragment(const Triangle& triangle) {
    std::vector<Fragment> triangleFragments;
    // Aquí va la lógica de conversión
    // Por ejemplo, simplemente convierte los vértices del triángulo en fragmentos
    triangleFragments.push_back({{static_cast<int>(triangle.a.x), static_cast<int>(triangle.a.y)}});
    triangleFragments.push_back({{static_cast<int>(triangle.b.x), static_cast<int>(triangle.b.y)}});
    triangleFragments.push_back({{static_cast<int>(triangle.c.x), static_cast<int>(triangle.c.y)}});
    return triangleFragments;
}

// Función rediseñada de rasterización
std::vector<Fragment> rasterizeFunc(const std::vector<Triangle>& triangles) {
    std::vector<Fragment> fragments;

    for (const Triangle& triangle : triangles) {
        std::vector<Fragment> triangleFragments = convertTriangleToFragment(triangle);
        fragments.insert(fragments.end(), triangleFragments.begin(), triangleFragments.end());
    }

    return fragments;
}

// Fragment Shader Color function. I should be paying attention to this later.
Color fragmentShader(const Fragment& fragment) {
    // Example: Assign a constant color to each fragment
    Color fragColor(255, 0, 0, 255); // Red color with full opacity

    // You can modify this function to implement more complex shading
    // based on the fragment's attributes (e.g., depth, interpolated normals, texture coordinates, etc.)

    return fragColor;
}



