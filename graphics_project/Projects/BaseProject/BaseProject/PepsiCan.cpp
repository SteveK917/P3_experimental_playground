/*	CS 559 Project 2
	Mitchell Lutzke & Steve Krejci

	The Mars class creates our Mars given an input file, as well as generates
	textures for the Red Planet.
*/

#include "PepsiCan.h"
#include "mesh.h"
#include "ilcontainer.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace glm;

PepsiCan::PepsiCan() : Object()
{

}

bool PepsiCan::Initialize()
{
	if (this->GLReturnedError("PepsiCan::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	mat4 m;
	m = translate(m, vec3(0.0f, -2.0f, 0.0f));
	m = scale(m, vec3(1.0f, 4.0f, 1.0f));
	
	// This is the can!
	MeshPack * the_can = Mesh::Cylinder(m, 1.0, 1.0, 10, 36, vec3(1.0f, 0.0f, 0.0f));
	the_can->addToScene(this->vertices, this->vertex_indices);
	delete the_can;

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCNT), &this->vertices[0]))
		return false;

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 2));	// Note offset - legacy of older code
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 1));	// Same
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(vec3) * 3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (this->normal_vertices.size() > 0)
	{
		if (!this->PostGLInitialize(&this->normal_array_handle, &this->normal_coordinate_handle, this->normal_vertices.size() * sizeof(VertexAttributesP), &this->normal_vertices[0]))
			return false;

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesP), (GLvoid *) 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// Here's the Pepsi can texture!
	this->pepsi_texture = new ILContainer();

	if( ! this->pepsi_texture->Initialize("pepsi.jpg") )
		return false;
	
	this->textures.push_back(this->pepsi_texture);

	if (!this->shader.Initialize("mars_shader.vert", "mars_shader.frag"))
		return false;
	
	this->shaders.push_back(&this->shader);

	if (this->GLReturnedError("Background::Initialize - on exit"))
		return false;

	return true;
}

void PepsiCan::TakeDown()
{
	delete this->pepsi_texture;

	this->vertices.clear();
	this->vertex_indices.clear();

	this->shader.TakeDown();

	super::TakeDown();
}

void PepsiCan::Draw(const ivec2 & size)
{
	assert(false);
}

void PepsiCan::Draw(const mat4 & projection, mat4 modelview, const vec3 & eye, const ivec2 & size, const float time, const int CameraMode)
{
	if (this->GLReturnedError("PepsiCan::Draw - on entry"))
		return;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));


	this->textures[0]->Bind();
	this->shaders[0]->Use();

	glViewport(0, 0, size.x, size.y);

	this->GLReturnedError("PepsiCan::Draw - after use");
	this->shaders[0]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm), CameraMode, value_ptr(eye));
	this->GLReturnedError("PepsiCan::Draw - after common setup");
	glBindVertexArray(this->vertex_array_handle);
	
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	this->GLReturnedError("PepsiCan::Draw - after draw");
	glUseProgram(0);

	glDisable(GL_TEXTURE_2D);

	if (this->GLReturnedError("PepsiCan::Draw - on exit"))
		return;
}
