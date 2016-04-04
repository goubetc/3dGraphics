#ifndef IO_HPP
#define IO_HPP

/**@file
 *@brief Input/Output functions for Obj mesh.
 *
 */

#include <vector>
#include <glm/glm.hpp>
#include <string>

/**
 * \brief Import data from an obj mesh file.
 *
 * \param filename The path to the mesh file.
 * \param vertices The list of vertices of the mesh.
 * \param normals The list of normals of the mesh.
 * \param triangles The list of triangles of the mesh.
 * \return False if import failed, true otherwise.
 */
bool read_obj(
        const std::string& filename, 
        std::vector<glm::vec3>& positions, 
        std::vector<unsigned int>& indices,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texcoords
        );

#endif //IO_HPP
