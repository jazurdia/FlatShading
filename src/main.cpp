#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include "gl.h"
#include "Face.h"
#include "Uniforms.h"

// this will render the object.
void render (std::vector<glm::vec3> vertices, Uniforms uniforms) {
    // 1. Vertex Shader
    // 2. Primitive Assembly
    // 3. Rasterization
    // 4. Fragment Shader
}

int main(int argc, char* argv[]) {
    initSDL();

    vertices = {
            {300.0f, 200.0f, 0.0f},
            {400.0f, 400.0f, 0.0f},
            {500.0f, 200.0f, 0.0f}
    };

    Uniforms uniforms;

    glm::mat4 model = glm::mat4(1);
    glm::mat4 view = glm::mat4(1);
    glm::mat4 projection = glm::mat4(1);

    uniforms.model = model;
    uniforms.model = view;
    uniforms.model = projection;

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