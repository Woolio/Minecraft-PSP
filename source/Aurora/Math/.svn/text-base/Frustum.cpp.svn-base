#include <Aurora/Math/Frustum.h>

namespace Aurora
{
	namespace Math
	{

		Frustum::Frustum()
		{

		}

		Frustum::~Frustum()
		{

		}

		void Frustum::ExtractPlanes(ScePspFMatrix4 &clip)
		{
			Plane *pPlane = 0;

			// Left clipping plane.
			/*pPlane = &planes[FRUSTUM_PLANE_NEAR];
			pPlane->set(clip.w.x + clip.z.x,clip.w.y + clip.z.y,clip.w.z + clip.z.z,clip.w.w + clip.z.w);
			pPlane->normalize();

			// Left clipping plane.
			pPlane = &planes[FRUSTUM_PLANE_FAR];
			pPlane->set(clip.w.x - clip.z.x,clip.w.y - clip.z.y,clip.w.z - clip.z.z,clip.w.w - clip.z.w);
			pPlane->normalize();

			// Left clipping plane.
			pPlane = &planes[FRUSTUM_PLANE_LEFT];
			pPlane->set(clip.w.x + clip.x.x,clip.w.y + clip.x.y,clip.w.z + clip.x.z,clip.w.w + clip.x.w);
			pPlane->normalize();

			pPlane = &planes[FRUSTUM_PLANE_RIGHT];
			pPlane->set(clip.w.x - clip.x.x,clip.w.y - clip.x.y,clip.w.z - clip.x.z,clip.w.w - clip.x.w);
			pPlane->normalize();

			pPlane = &planes[FRUSTUM_PLANE_BOTTOM];
			pPlane->set(clip.w.x + clip.y.x,clip.w.y + clip.y.y,clip.w.z + clip.y.z,clip.w.w + clip.y.w);
			pPlane->normalize();

			pPlane = &planes[FRUSTUM_PLANE_TOP];
			pPlane->set(clip.w.x - clip.y.x,clip.w.y - clip.y.y,clip.w.z - clip.y.z,clip.w.w - clip.y.w);
			pPlane->normalize();*/

			// Left clipping plane.
			pPlane = &planes[FRUSTUM_PLANE_NEAR];
			pPlane->set(clip.x.w + clip.x.z,clip.y.w + clip.y.z,clip.z.w + clip.z.z,clip.w.w + clip.w.z);
			pPlane->normalize();

			// Left clipping plane.
			pPlane = &planes[FRUSTUM_PLANE_FAR];
			pPlane->set(clip.x.w - clip.x.z,clip.y.w - clip.y.z,clip.z.w - clip.z.z,clip.w.w - clip.w.z);
			pPlane->normalize();

			// Left clipping plane.
			pPlane = &planes[FRUSTUM_PLANE_LEFT];
			pPlane->set(clip.x.w + clip.x.x,clip.y.w + clip.y.x,clip.z.w + clip.z.x,clip.w.w + clip.w.x);
			pPlane->normalize();

			pPlane = &planes[FRUSTUM_PLANE_RIGHT];
			pPlane->set(clip.x.w - clip.x.x,clip.y.w - clip.y.x,clip.z.w - clip.z.x,clip.w.w - clip.w.x);
			pPlane->normalize();

			pPlane = &planes[FRUSTUM_PLANE_BOTTOM];
			pPlane->set(clip.x.w + clip.x.y,clip.y.w + clip.y.y,clip.z.w + clip.z.y,clip.w.w + clip.w.y);
			pPlane->normalize();

			pPlane = &planes[FRUSTUM_PLANE_TOP];
			pPlane->set(clip.x.w - clip.x.y,clip.y.w - clip.y.y,clip.z.w - clip.z.y,clip.w.w - clip.w.y);
			pPlane->normalize();


		}

		Frustum::PlaneIntersectionType Frustum::BoxInFrustum(float minx,float miny,float minz,float maxx,float maxy,float maxz)
		{
			BoundingBox box(minx,miny,minz,maxx,maxy,maxz);
			bool flag = false;

			for (int i = 0; i < 6; ++i)
			{
				switch(box.intersect(planes[i]))
				{
				  case BoundingBox::Back:
				  return Outside;

				  case BoundingBox::Front:
				  case BoundingBox::Intersecting:
					flag = true;
					break;
				}
			}

			if (!flag)
			{
				return Inside;
			}


			return Intersects;
		}

		Frustum::PlaneIntersectionType Frustum::BoxInFrustum(BoundingBox &box)
		{
			int flag = 0;
			for (int i = 0; i < 6; ++i)
			{
				switch(box.intersect(planes[i]))
				{
					case BoundingBox::Front:
					case BoundingBox::Intersecting:
					{
						flag = 1;
					}break;

					case BoundingBox::Back:
					return Outside;
				}
			}

			return Intersects;
		}

		bool Frustum::BBoxInFrustum(const BoundingBox &box) const
		{
			Vector3 c((box.min + box.max) * 0.5f);
			float sizex = box.max.x - box.min.x;
			float sizey = box.max.y - box.min.y;
			float sizez = box.max.z - box.min.z;

			Vector3 corners[8] =
			{
				Vector3((c.x - sizex * 0.5f), (c.y - sizey * 0.5f), (c.z - sizez * 0.5f)),
				Vector3((c.x + sizex * 0.5f), (c.y - sizey * 0.5f), (c.z - sizez * 0.5f)),
				Vector3((c.x - sizex * 0.5f), (c.y + sizey * 0.5f), (c.z - sizez * 0.5f)),
				Vector3((c.x + sizex * 0.5f), (c.y + sizey * 0.5f), (c.z - sizez * 0.5f)),
				Vector3((c.x - sizex * 0.5f), (c.y - sizey * 0.5f), (c.z + sizez * 0.5f)),
				Vector3((c.x + sizex * 0.5f), (c.y - sizey * 0.5f), (c.z + sizez * 0.5f)),
				Vector3((c.x - sizex * 0.5f), (c.y + sizey * 0.5f), (c.z + sizez * 0.5f)),
				Vector3((c.x + sizex * 0.5f), (c.y + sizey * 0.5f), (c.z + sizez * 0.5f))
			};

			for (int i = 0; i < 6; ++i)
			{
				if (Plane::dot(planes[i], corners[0]) > 0.0f)
					continue;

				if (Plane::dot(planes[i], corners[1]) > 0.0f)
					continue;

				if (Plane::dot(planes[i], corners[2]) > 0.0f)
					continue;

				if (Plane::dot(planes[i], corners[3]) > 0.0f)
					continue;

				if (Plane::dot(planes[i], corners[4]) > 0.0f)
					continue;

				if (Plane::dot(planes[i], corners[5]) > 0.0f)
					continue;

				if (Plane::dot(planes[i], corners[6]) > 0.0f)
					continue;

				if (Plane::dot(planes[i], corners[7]) > 0.0f)
					continue;

				return false;
			}

			return true;
		}

		bool Frustum::PointInFrustum(const Vector3 &point) const
		{
			for (int i = 0; i < 6; ++i)
			{
				if (Plane::dot(planes[i], point) <= 0.0f)
					return false;
			}

			return true;
		}

		bool Frustum::PointInFrustum(int plane,const Vector3 &point,float &distance) const
		{
			distance = Plane::dot(planes[plane], point);
			if (distance <= 0.0f)
				return false;

			return true;
		}

		bool Frustum::SphereInFrustum(const BoundingSphere &sphere) const
		{
			for (int i = 0; i < 6; ++i)
			{
				if (Plane::dot(planes[i], sphere.center) <= -sphere.radius)
					return false;
			}

			return true;
		}

		//clipping

		TexturesPSPVertex Frustum::vertexIntersection(TexturesPSPVertex& a,float da,TexturesPSPVertex& b,float db)
		{
			float s = da/(da-db);   // intersection factor (between 0 and 1)

			TexturesPSPVertex intersectpoint;

			intersectpoint.x = a.x + s*(b.x-a.x);
			intersectpoint.y = a.y + s*(b.y-a.y);
			intersectpoint.z = a.z + s*(b.z-a.z);

			intersectpoint.u = a.u + s*(b.u-a.u);
			intersectpoint.v = a.v + s*(b.v-a.v);

			return intersectpoint;
		}

		void Frustum::Clip(int plane,int vertexCount,TexturesPSPVertex * in,int i,TexturesPSPVertex * out,int &outCount)
		{
			//for each vertex
			outCount = 0;

			bool ins[5];
			float dist[5];
			int index = 0;

			for(int v = i; v < (i + vertexCount);v++)
			{
				ins[index] = PointInFrustum(plane,Vector3(in[v].x,in[v].y,in[v].z),dist[index]);
				index++;
			}

			index = 0;
			for(int v = i; v < (i + vertexCount) - 1;v++)
			{
				if(ins[index])
				{
					if(ins[index+1])
					{
						out[outCount++] = in[v+1];
					}else
					{
						out[outCount++] = vertexIntersection(in[v],dist[index],in[v+1],dist[index+1]);
					}
				}else
				{
					if(ins[index+1])
					{
						out[outCount++] = vertexIntersection(in[v],dist[index],in[v+1],dist[index+1]);
						out[outCount++] = in[v+1];
					}
				}
				index++;
			}

			//last and first
			if(ins[index])
			{
				if(ins[0])
				{
					out[outCount++] = in[i];
				}else
				{
					out[outCount++] = vertexIntersection(in[(i + vertexCount) - 1],dist[index],in[i],dist[0]);
				}
			}else
			{
				if(ins[0])
				{
					out[outCount++] = vertexIntersection(in[(i + vertexCount) - 1],dist[index],in[i],dist[0]);
					out[outCount++] = in[i];
				}
			}
		}

	}
}

