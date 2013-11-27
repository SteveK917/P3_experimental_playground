/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:
*/

#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include "shader.h"
#include "ilcontainer.h"

class PepsiCan : public Object
{
public:
	PepsiCan();
	void RenderPepsiCan(int slices);
	bool Initialize();
	virtual void Draw(const glm::ivec2 & size);
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::vec3 & eye, const glm::ivec2 & size, const float time = 0, const int CameraMan = 0);
	void StepShader();
	void StepTexture();
	void TakeDown();
	Shader shader;
	ILContainer * pepsi_texture;
	std::vector<Shader *> shaders;
	std::vector<ILContainer *> textures;

private:
	std::vector<VertexAttributesPCNT> vertices;
	typedef Object super;
};
