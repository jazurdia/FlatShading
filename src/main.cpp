#include <iostream>
#include <SDL.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "gl.h"

// testing functions in gl.h

Triangle verticesToTriangle(const std::vector<glm::vec3>& vertices) {
    if (vertices.size() != 3) {
        throw std::runtime_error("Invalid number of vertices for a triangle");
    }
    return Triangle{vertices[0], vertices[1], vertices[2]};
}


void render(std::vector<glm::vec3> vertices, Uniforms uniforms) {
    // 1. Vertex Shader using the vertexShader function from gl.h
    std::vector<glm::vec3> transformedVertices;
    transformedVertices.reserve(vertices.size());

    // Transform each vertex using the vertexShader function
    for (const auto& vertex : vertices) {
        transformedVertices.push_back(vertexShader(vertex, uniforms));
    }

    // 2. Primitive Assembly using the primitiveAssembly function from gl.h
    std::vector<Triangle> triangles;
    auto primitives = primitiveAssembly(transformedVertices);
    for (const auto& primitive : primitives) {
        triangles.push_back(verticesToTriangle(primitive));
    }

    // Rasterization function
    std::vector<Fragment> fragments = rasterizeFunc(triangles);




    // 4. Fragment Shader
    // ... (resto de tu lógica de renderizado)
}



// Una función main que cree una ventana de SDL y inicie un main loop de renderizado
int main(int argc, char* argv[]) {
    bool initSdl = init_sdl();
    if (!initSdl) {
        return 1;
    }

    std::vector<glm::vec3> vertices = {
            {300.0f, 200.0f, 0.0f},
            {400.0f, 400.0f, 0.0f},
            {500.0f, 200.0f, 0.0f}
    };

    Uniforms uniforms{};

    uniforms.model = glm::mat4(1);
    uniforms.view = glm::mat4(1);
    uniforms.projection = glm::mat4(1);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        render(vertices, uniforms);

        renderBuffer(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}