#include "shapes.h"

#include "common.h"

#include <stdlib.h>

#include <cglm/cglm.h>


static struct mesh create_mesh(size_t n_triangles, size_t n_vertices)
{
    struct mesh r;
    r.n_vertices = n_vertices;
    r.n_triangles = n_triangles;

    r.vertex_buffer = (float *)malloc(3 * n_vertices * sizeof(float));
    r.index_buffer = (uint32_t *)malloc(3 * n_triangles * sizeof(uint32_t));

    r._current_vert_i = 0;
    r._current_triangle_i = 0;

    return r;
}

static void mesh_extend(struct mesh *mesh, size_t new_tri, size_t new_vert)
{
    mesh->n_vertices += new_vert;
    mesh->n_triangles += new_tri;

    const size_t new_vert_buffer_size = mesh->n_vertices * 3 * sizeof(float);
    const size_t new_ind_buffer_size = mesh->n_triangles * 3 * sizeof(uint32_t);

    mesh->vertex_buffer = realloc(mesh->vertex_buffer, new_vert_buffer_size);
    mesh->index_buffer = realloc(mesh->index_buffer, new_ind_buffer_size);
}

void free_mesh(struct mesh *mesh)
{
    free(mesh->vertex_buffer);
    free(mesh->index_buffer);
}

static void mesh_add_vertex(struct mesh *mesh, vec3 vertex)
{
    if (mesh->_current_vert_i >= mesh->n_vertices) {
        WARN("Appending another vertex to the mesh would exceed allocated buffer. Skipping vertex.");
        return;
    }

    for (uint32_t i = 0; i < 3; i++) {
        mesh->vertex_buffer[mesh->_current_vert_i * 3 + i] = vertex[i];
    }
    mesh->_current_vert_i++;
}

static void mesh_add_triangle(struct mesh *mesh, uint32_t triangle[3])
{
    if (mesh->_current_triangle_i >= mesh->n_triangles) {
        WARN("Appending another index to the mesh would exceed allocated buffer. Skipping index.");
        return;
    }

    for (uint32_t i = 0; i < 3; i++) {
        mesh->index_buffer[mesh->_current_triangle_i * 3 + i] = triangle[i];
    }
    mesh->_current_triangle_i++;
}

static void mesh_add_vertices(struct mesh *mesh, size_t n_vertices, float vertices[][3])
{
    for (uint32_t i = 0; i < n_vertices; i++) {
        mesh_add_vertex(mesh, vertices[i]);
    }
}

static void mesh_add_triangles(struct mesh *mesh, size_t n_triangles, uint32_t triangles[][3])
{
    for (uint32_t i = 0; i < n_triangles; i++) {
        mesh_add_triangle(mesh, triangles[i]);
    }
}

static inline void mesh_supply_vertices(struct mesh *mesh, float vertices[][3])
{
    mesh_add_vertices(mesh, mesh->n_vertices, vertices);
}

static inline void mesh_supply_triangles(struct mesh *mesh, uint32_t triangles[][3])
{
    mesh_add_triangles(mesh, mesh->n_triangles, triangles);
}

static void project_to_unit_sphere(struct mesh *mesh)
{
    for (uint32_t i = 0; i < mesh->n_vertices; i++) {
        vec3 vert_pos;
        for (uint32_t u = 0; u < 3; u++) {
            vert_pos[u] = mesh->vertex_buffer[i * 3 + u];
        }

        float l = glm_vec3_norm(vert_pos);
        glm_vec3_divs(vert_pos, l, vert_pos);

        for (uint32_t j = 0; j < 3; j++) {
            mesh->vertex_buffer[i * 3 + j] = vert_pos[j];
        }
    }
}

static void subdivide(struct mesh *mesh)
{
    const uint32_t n_tri = mesh->n_triangles;
    const uint32_t n_new_tri = 3 * n_tri;
    mesh_extend(mesh, n_new_tri, n_new_tri);

    for (uint32_t i = 0; i < n_tri; i++) {
        vec3 v_coords[3];
        uint32_t tmp_indices[2];

        for (uint32_t u = 0; u < 3; u++) {
            for (uint32_t j = 0; j < 3; j++) {
                v_coords[u][j] = mesh->vertex_buffer[mesh->index_buffer[i * 3 + u] * 3 + j];
            }
        }

        vec3 v0, v1, v2;
        glm_vec3_add(v_coords[0], v_coords[1], v0);
        glm_vec3_divs(v0, 2, v0);

        glm_vec3_add(v_coords[0], v_coords[2], v1);
        glm_vec3_divs(v1, 2, v1);

        glm_vec3_add(v_coords[1], v_coords[2], v2);
        glm_vec3_divs(v2, 2, v2);

        float new_vertices[][3] = {
            {v0[0], v0[1], v0[2]},
            {v1[0], v1[1], v1[2]},
            {v2[0], v2[1], v2[2]}
        };
        uint32_t vert_i = mesh->_current_vert_i;
        mesh_add_vertices(mesh, 3, new_vertices);

        tmp_indices[0] = mesh->index_buffer[i * 3 + 1];
        tmp_indices[1] = mesh->index_buffer[i * 3 + 2];

        mesh->index_buffer[i * 3 + 1] = vert_i;
        mesh->index_buffer[i * 3 + 2] = vert_i + 1;

        uint32_t new_triangles[][3] = {
            {vert_i,     vert_i + 1,     vert_i + 2},
            {vert_i,     tmp_indices[0], vert_i + 2},
            {vert_i + 2, tmp_indices[1], vert_i + 1}
        };
        mesh_add_triangles(mesh, 3, new_triangles);
    }
}

struct mesh generate_icosphere(size_t n_subdivisions)
{
    struct mesh mesh = create_icosahedron();

    for (uint32_t i = 0; i < n_subdivisions; i++) {
        subdivide(&mesh);
    }

    project_to_unit_sphere(&mesh);
    return mesh;
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