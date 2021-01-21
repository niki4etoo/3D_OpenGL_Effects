glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

struct Mouse {
	//Mouse parameters
	bool firstMouse = true;
	const float sensitivity = 0.1f;
	float pitch = 0.0f, yaw = -90.0f;
	float lastX = 0.0f, lastY = 0.0f;
	float fov = 45.0f;
};
