#include <Aurora/Graphics/Mesh.h>

namespace Aurora
{
	namespace Graphics
	{
		Mesh::Mesh() : Node()
		{
		}

		Mesh::Mesh(std::string name,Math::Vector3 position): Node( name,position)
		{
		}

		Mesh::Mesh(std::string name,Math::Vector3 position,Math::Vector3 scale): Node( name,position,scale)
		{
		}

		void Mesh::AddVertex(Vertex vertex)
		{
			Vertices.push_back(vertex);
		}

		void Mesh::AddFace(Face face)
		{
			Faces.push_back(face);
		}

		void Mesh::FinishMesh()
		{
			meshvertices = (SimplePSPVertex*)memalign(16,( Faces.size() * 3) * sizeof(SimplePSPVertex));

			int number = 0;
			for(unsigned int i = 0;i< Faces.size();i++)
			{
				meshvertices[number].color = GU_RGBA((int)Faces[i].color.R * 255,(int)Faces[i].color.G * 255,(int)Faces[i].color.B * 255,255);
				meshvertices[number].x = Vertices[Faces[i].V1].Position.x;
				meshvertices[number].y = Vertices[Faces[i].V1].Position.y;
				meshvertices[number].z = Vertices[Faces[i].V1].Position.z;
				number++;

				meshvertices[number].color = GU_RGBA((int)Faces[i].color.R * 255,(int)Faces[i].color.G * 255,(int)Faces[i].color.B * 255,255);
				meshvertices[number].x = Vertices[Faces[i].V2].Position.x;
				meshvertices[number].y = Vertices[Faces[i].V2].Position.y;
				meshvertices[number].z = Vertices[Faces[i].V2].Position.z;
				number++;

				meshvertices[number].color = GU_RGBA((int)Faces[i].color.R * 255,(int)Faces[i].color.G * 255,(int)Faces[i].color.B * 255,255);
				meshvertices[number].x = Vertices[Faces[i].V3].Position.x;
				meshvertices[number].y = Vertices[Faces[i].V3].Position.y;
				meshvertices[number].z = Vertices[Faces[i].V3].Position.z;
				number++;
			}
		}
	}
}
