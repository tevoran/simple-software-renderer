#include <fast_obj.h>
#include <iostream>

#include "ssr.hpp"


ssr::vertex* ssr::renderer::load_mesh(const char * path, uint32_t *num_polygons)
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


		for(uint32_t num_faces_done=0; num_faces_done<num_faces; num_faces_done++)
		{
			uint32_t num_current_face_vertices=*(mesh->face_vertices+num_faces_done);
			ssr::vertex *tmp_vertices=new ssr::vertex[num_current_face_vertices];

			for(int i=0; i<num_current_face_vertices; i++)
			{
				tmp_vertices[i].x=*(mesh->positions+mesh->indices[num_vertices_done+i].p*3);
				tmp_vertices[i].y=*(mesh->positions+mesh->indices[num_vertices_done+i].p*3+1);
				tmp_vertices[i].z=*(mesh->positions+mesh->indices[num_vertices_done+i].p*3+2)+10;

				tmp_vertices[i].u=0;
				tmp_vertices[i].v=0;
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
		return mesh_vertices;
}