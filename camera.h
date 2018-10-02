#if !defined(CAMERA_H_201806282338)
#define CAMERA_H_201806282338

struct Player;
struct Stage;

struct Camera
{
	int x;
	unsigned long last_direction;
};

void Camera_Construct(struct Camera* camera);
void Camera_Update(struct Camera* camera, unsigned long pad, struct Player const* player, struct Stage const* stage);

#endif // !defined(CAMERA_H_201806282338)
