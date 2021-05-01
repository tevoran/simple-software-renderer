#include <fast_obj.h>
#include <iostream>
#include <limits>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "ssr.hpp"


ssr::vertex* ssr::renderer::load_mesh(const char * path, uint32_t *num_polygons, glm::vec3 *max_coords, glm::vec3 *min_coords)
{
	fastObjMesh *mesh = fast_obj_read(path); //first argument is the path to the mesh
	
		//creating mesh from file
		uint32_t num_faces=mesh->face_count;
		uint32_t num_vertices_done=0;
		uint32_t num_total_vertices=0;
		uint32_t num_vertices_triangulated=0;
		std::cout << "faces: " << num_faces<< std::endl;

		//counting necessary vertices for the whole mesh
		for(int i=0; i<num_faces; i++)
		{
			num_total_vertices=num_total_vertices+*(mesh->face_vertices+i)*3-6;
		}
		std::cout << "total triangles: " << num_total_vertices/3 << std::endl;

		ssr::vertex *mesh_vertices=new ssr::vertex[num_total_vertices];

		//looking for maximum and minimum along the different axes
		max_coords->x=0;
		max_coords->y=0;
		max_coords->z=0;
		min_coords->x=UINT64_MAX;
		min_coords->y=UINT64_MAX;
		min_coords->z=UINT64_MAX;

		for(int i=1; i<mesh->position_count; i++)
		{
			if(mesh->positions[i*3]>max_coords->x)
			{
				max_coords->x=mesh->positions[i*3];
			}
			if(mesh->positions[i*3+1]>max_coords->y)
			{
				max_coords->y=mesh->positions[i*3+1];
			}
			if(mesh->positions[i*3+2]>max_coords->z)
			{
				max_coords->z=mesh->positions[i*3+2];
			}
			if(mesh->positions[i*3]<min_coords->x)
			{
				min_coords->x=mesh->positions[i*3];
			}
			if(mesh->positions[i*3+1]<min_coords->y)
			{
				min_coords->y=mesh->positions[i*3+1];
			}
			if(mesh->positions[i*3+2]<min_coords->z)
			{
				min_coords->z=mesh->positions[i*3+2];
			}
		}

		for(uint32_t num_faces_done=0; num_faces_done<num_faces; num_faces_done++)
		{
			uint32_t num_current_face_vertices=*(mesh->face_vertices+num_faces_done);
			ssr::vertex *tmp_vertices=new ssr::vertex[num_current_face_vertices];

			for(int i=0; i<num_current_face_vertices; i++)
			{
				tmp_vertices[i].x=*(mesh->positions+mesh->indices[num_vertices_done+i].p*3);
				tmp_vertices[i].y=*(mesh->positions+mesh->indices[num_vertices_done+i].p*3+1);
				tmp_vertices[i].z=*(mesh->positions+mesh->indices[num_vertices_done+i].p*3+2);

				tmp_vertices[i].u=*(mesh->texcoords+mesh->indices[num_vertices_done+i].t*2);
				tmp_vertices[i].v=1-*(mesh->texcoords+mesh->indices[num_vertices_done+i].t*2+1); //using the local ssr coordinates
			}

			mesh_vertices[num_vertices_triangulated]=tmp_vertices[0];
			num_vertices_triangulated++;

			mesh_vertices[num_vertices_triangulated]=tmp_vertices[1];
			num_vertices_triangulated++;

			mesh_vertices[num_vertices_triangulated]=tmp_vertices[2];
			num_vertices_triangulated++;

			for(int i=3; i<num_current_face_vertices; i++)
			{
				mesh_vertices[num_vertices_triangulated]=tmp_vertices[0];
				num_vertices_triangulated++;
				mesh_vertices[num_vertices_triangulated]=tmp_vertices[i-1];
				num_vertices_triangulated++;
				mesh_vertices[num_vertices_triangulated]=tmp_vertices[i];
				num_vertices_triangulated++;
			}


			num_vertices_done+=num_current_face_vertices;
			delete [] tmp_vertices;
		}

		*num_polygons=num_total_vertices/3;
		fast_obj_destroy(mesh);
		return mesh_vertices;
}