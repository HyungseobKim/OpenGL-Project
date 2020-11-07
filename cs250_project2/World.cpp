#include <map>

#include "World.h"

#include "CameraRender.h"

World::World() // Initialize meshes
{
	light.SetMesh(MeshType::cube);
	light.SetCenter(Origin::light);
	
	light.SetRevCenter(Origin::light + Vector(5.f, 0, 0));
	light.SetRevAxis(EY);
	light.SetRevRate(rotation_rate*5);
	light.SetRevRadius(10.f);

	// Initialize plane
	const float size = 100.f;
	plane.SetMesh(MeshType::cube);
	plane.SetCenter(Origin::plane);
	plane.SetSize(Vector(size, 1.f, size));

	// Initialize occluder
	for (int i = 0; i < 2; i++)
	{
		occluder[i].SetMesh(MeshType::cube);
		occluder[i].SetCenter(Origin::occluder + Vector(i * 5.f, 0, 0));
	}
	for (int i = 2; i < 4; i++)
	{
		occluder[i].SetMesh(MeshType::snub);
		occluder[i].SetCenter(Origin::occluder + Vector(i * 5.f, 0, 0));
	}
}

void World::Draw(CameraRender* crender, double dt)
{
	const Point light_source = light.GetCenter();

	const Point planeCenter = plane.GetCenter();
	const Hcoord planeEquation = Hcoord(0, 1, 0, -1.f);

	light.Revolve(dt);
	crender->Draw(light.GetMesh(), light.Get_Object_to_World(), RED, light_source);

	// Plane
	crender->Draw(plane.GetMesh(), plane.Get_Object_to_World(), YELLOW, light_source);

	// Occluders
	for (int i = 0; i < 4; i++)
		crender->Draw(occluder[i].GetMesh(), occluder[i].Get_Object_to_World(), BLUE, light_source, planeEquation);
}

void World::MouseDown(const Point& pos)
{
	isMouseDown = true;
	mouse = pos;
	RayCast();
}

void World::MouseUp()
{
	isMouseDown = false;
	picked = -1;
}

void World::HandleMouseMotion(const Point& pos)
{
	if (!isMouseDown) return;
	if (picked < 0) return;

	Vector camera_to_obejct = occluder[picked].GetCenter() - Origin::camera;
	Vector camera_to_pos = pos - Origin::camera;

	Vector up = cross(camera_to_obejct, camera_to_pos);
	Vector translation = normalize(cross(up, camera_to_obejct + camera_to_pos));

	float distance = abs(pos - mouse) * (abs(camera_to_obejct) / abs(mouse - Origin::camera));

	occluder[picked].Translate(distance * translation);

	Point center = occluder[picked].GetCenter();
	if (center.y < 2.f)
		occluder[picked].SetCenter(Point(center.x, 2.f, center.z));
	mouse = pos;
}

void World::RayCast()
{
	std::map<int, float> entry_times;
	for (int i = 0; i < 4; i++)
		entry_times[i] = -1;

	const Vector ray = normalize(mouse - Origin::camera);

	for (int i = 0; i < 4; i++)
	{
		WorldMesh object = occluder[i];
		Point center = object.GetCenter();
		Vector cam_to_obj = center - Origin::camera;

		if (dot(cam_to_obj, cam_to_obj) < 3)
		{
			entry_times[i] = 0.f;
			continue;
		}

		float projection = dot(cam_to_obj, ray);
		if (projection < 0) continue;

		float distance = dot(cam_to_obj, cam_to_obj) - projection * projection;
		if (distance > 3) continue;

		entry_times[i] = projection - sqrt(3 - distance);
	}

	float entryTime = -1.f;
	for (int i = 0; i < 4; i++)
	{
		float time = entry_times[i];

		if (time < 0) continue;

		if (entryTime < 0 || entryTime > time)
		{
			entryTime = time;
			picked = i;
		}
	}
}