#include "shapes.h"

#include "common.h"

#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include <cglm/cglm.h>


static struct mesh create_mesh(size_t n_triangles, size_t n_vertices)
{
    struct mesh r;
    r.n_vertices = n_vertices;
    r.n_triangles = n_triangles;

    r.vertex_buffer = r._vertex_ptr = (float *)malloc(3 * n_vertices * sizeof(float));
    r.index_buffer = r._index_ptr = (uint32_t *)malloc(3 * n_triangles * sizeof(uint32_t));

    return r;
}

static struct abstract_mesh describe_mesh(const struct mesh *mesh)
{
    struct abstract_mesh ab_mesh;

    ab_mesh._sizeofvertices = mesh->n_vertices * sizeof(struct vertex);
    ab_mesh._sizeoftriangles = mesh->n_triangles * sizeof(struct triangle);

    ab_mesh.vertices = ab_mesh._vertices_head = (struct vertex *)malloc(ab_mesh._sizeofvertices);
    ab_mesh.triangles = ab_mesh._triangles_head = (struct triangle *)malloc(ab_mesh._sizeoftriangles);

    for (uint32_t i = 0; i < mesh->n_vertices; i++) {
        for (uint32_t j = 0; j < 3; j++) {
            ab_mesh.vertices[i].vertex[j] = mesh->vertex_buffer[i * 3 + j];
        }

        if (i < mesh->n_vertices - 1) {
            ab_mesh.vertices[i].next = &ab_mesh.vertices[i + 1];
        } else {
            ab_mesh.vertices[i].next = NULL;
            ab_mesh._vertices_head = &ab_mesh.vertices[i];
        }
    }

    for (uint32_t u = 0; u < mesh->n_triangles; u++) {
        for (uint32_t k = 0; k < 3; k++) {
            ab_mesh.triangles[u].triangle[k] = mesh->index_buffer[u * 3 + k];
        }

        if (u < mesh->n_triangles - 1) {
            ab_mesh.triangles[u].next = &ab_mesh.triangles[u + 1];
        } else {
            ab_mesh.triangles[u].next = NULL;
            ab_mesh._triangles_head = &ab_mesh.triangles[u];
        }
    }

    return ab_mesh;
}

static void abstract_mesh_insert_vertices(struct abstract_mesh *ab_mesh, size_t n_vertices, float vertices[][3], uint32_t new_index)
{
    struct vertex new_vert;
    
    ab_mesh->_sizeofvertices += n_vertices * sizeof(struct vertex);
    struct vertex *ext_buff = realloc(ab_mesh->vertices, ab_mesh->_sizeofvertices);

    for (uint32_t i = 0; i < n_vertices; i++) {
    }
}

void free_mesh(struct mesh *mesh)
{
    free(mesh->vertex_buffer);
    free(mesh->index_buffer);
}

static void mesh_add_vertex(struct mesh *mesh, vec3 vertex)
{
    if ((mesh->_vertex_ptr + 3 - mesh->vertex_buffer) / 3 > mesh->n_vertices) {
        WARN("Appending another vertex to the mesh would exceed allocated buffer. Skipping vertex.");
        return;
    }

    for (uint32_t i = 0; i < 3; i++) {
        *mesh->_vertex_ptr = vertex[i];
        mesh->_vertex_ptr++;
    }
}

static void mesh_add_triangle(struct mesh *mesh, uint32_t triangle[3])
{
    if ((mesh->_index_ptr + 3 - mesh->index_buffer) / 3 > mesh->n_triangles) {
        WARN("Appending another index to the mesh would exceed allocated buffer. Skipping index.");
        return;
    }

    for (uint32_t i = 0; i < 3; i++) {
        *mesh->_index_ptr = triangle[i];
        mesh->_index_ptr++;
    }
}

static void mesh_supply_vertices(struct mesh *mesh, float vertices[][3])
{
    for (uint32_t i = 0; i < mesh->n_vertices; i++) {
        mesh_add_vertex(mesh, vertices[i]);
    }
}

static void mesh_supply_triangles(struct mesh *mesh, uint32_t triangles[][3])
{
    for (uint32_t i = 0; i < mesh->n_triangles; i++) {
        mesh_add_triangle(mesh, triangles[i]);
    }
}

struct mesh generate_icosphere(size_t n_subdivisions)
{
    const struct mesh icosahedron = create_icosahedron();
    struct abstract_mesh wrk_mesh = describe_mesh(&icosahedron);

    for (uint32_t i = 0; i < icosahedron.n_triangles; i++) {
        // We need to create 3 new vertices

    }
}

inline struct mesh create_icosahedron(void)
{
    struct mesh r = create_mesh(20, 12);

    float phi = (1.0f + sqrt(5.0f)) * 0.5f;
    float a = 1.0f;
    float b = 1.0f / phi;

    float vertices[][3] = {
        { b,  a,  0}, // 0
        {-b,  a,  0}, // 1
        { b, -a,  0}, // 2
        {-b, -a,  0}, // 3
        { 0,  b,  a}, // 4
        { 0, -b,  a}, // 5
        { 0,  b, -a}, // 6
        { 0, -b, -a}, // 7
        { a,  0,  b}, // 8
        { a,  0, -b}, // 9
        {-a,  0,  b}, // 10
        {-a,  0, -b}  // 11
    };
    mesh_supply_vertices(&r, vertices);

    uint32_t triangles[][3] = {
        {0,  1,  4},
        {0,  1,  6},
        {0,  8,  9},
        {1, 10, 11},
        {0,  4,  8},
        {1,  4, 10},
        {0,  6,  9},
        {1,  6, 11},
        {4,  5, 10},
        {4,  5,  8},
        {6,  7, 11},
        {6,  7,  9},
        {2,  3,  5},
        {2,  3,  7},
        {2,  8,  9},
        {3, 10, 11},
        {2,  5,  8},
        {3,  5, 10},
        {2,  7,  9},
        {3,  7, 11}
    };
    mesh_supply_triangles(&r, triangles);

    return r;
}