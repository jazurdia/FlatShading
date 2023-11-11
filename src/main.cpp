#include <iostream>
#include <SDL.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "gl.h"

// testing functions in gl.h


// Una función main que cree una ventana de SDL y inicie un main loop de renderizado
int main(int arcg, char* argv[]) {
    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window
    window = SDL_CreateWindow("Flat Shading", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    // Check if window was created
    if (window == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // load variables.
    std::vector<glm::vec3> ver;
    std::vector<Face> faces;

    bool success = loadOBJ("nave_espacial.obj", ver, faces);

    // check.
    if (!success) {
        std::cerr << "Error loading OBJ file!" << std::endl;
        return 1;
    }

    bool running = true; // Loop flag


    // Apply rotation
    const auto pi = glm::pi<float>();
    float angleZ = pi;
    float angleX = 0;
    float angleY = pi/2;

    // While application is running
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // clear
        SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        SDL_RenderClear(renderer);

        angleY += 0.015f;


        for (const auto& face : faces) {
            // glm::vec3 A = vertices[face.vertexIndices[0][0] - 1];
            glm::vec3 A = ver[face.vertexIndices[0][0] - 1];
            glm::vec3 B = ver[face.vertexIndices[1][0] - 1];
            glm::vec3 C = ver[face.vertexIndices[2][0] - 1];


            const float size = 50.0f;

            A *= size;
            B *= size;
            C *= size;

            A = glm::rotateZ(A, angleZ);
            B = glm::rotateZ(B, angleZ);
            C = glm::rotateZ(C, angleZ);

            A = glm::rotateX(A, angleX / 4);
            B = glm::rotateX(B, angleX / 4);
            C = glm::rotateX(C, angleX / 4);
            //
            A = glm::rotateY(A, angleY);
            B = glm::rotateY(B, angleY);
            C = glm::rotateY(C, angleY);

            // Ajustar a escala
            A.x += 320.0f;
            A.y += 240.0f;
            B.x += 320.0f;
            B.y += 240.0f;
            C.x += 320.0f;
            C.y += 240.0f;

            drawTriangle({A, B, C});

        }

        // Update screen
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}