#include <vector>
#include "SDL.h"
#include "glm/glm.hpp"
#include "Uniforms.h"
#include "Vertex.h"
#include "Fragment.h"
#include "triangle.h"

Vertex vertexShader(const Vertex& vertex, const Uniforms& u) {
    glm::vec4 v = glm::vec4(vertex.position.x, vertex.position.y, vertex.position.z, 1);
    glm::vec4 r = u.viewport * u.projection * u.view * u.model * v;

    return Vertex{
            glm::vec3(r.x/r.w, r.y/r.w, r.z/r.w),
            vertex.color
    };
};

std::vector<std::vector<Vertex>> primitiveAssembly (const std::vector<Vertex>& transformedVertices) {
    std::vector<std::vector<Vertex>> groupedVertices;
    for (int i = 0; i < transformedVertices.size(); i += 3) {
        std::vector<Vertex> vertexGroup;
        vertexGroup.push_back(transformedVertices[i]);
        vertexGroup.push_back(transformedVertices[i+1]);
        vertexGroup.push_back(transformedVertices[i+2]);

        groupedVertices.push_back(vertexGroup);
    }

    return groupedVertices;
}


// Rasterizes a collection of triangles into fragments
std::vector<Fragment> rasterizeTriangles(const std::vector<std::vector<Vertex>>& triangles) {
    std::vector<Fragment> fragments;

    // Iterate over each triangle and rasterize it
    for (const std::vector<Vertex>& triangleVertices : triangles) {
        if (triangleVertices.size() != 3) {
            // Handle error or invalid input
            continue;
        }

        // Rasterize the triangle into fragments
        std::vector<Fragment> rasterizedTriangle = triangle(
                triangleVertices[0],
                triangleVertices[1],
                triangleVertices[2]
        );

        // Add the rasterized fragments to the main collection
        fragments.insert(
                fragments.end(),
                rasterizedTriangle.begin(),
                rasterizedTriangle.end()
        );
    }

    return fragments;
}

// fragment shaders
Fragment fragmentShader(Fragment& fragment) {
    fragment.color = fragment.color * fragment.intensity;
    return fragment;
}
