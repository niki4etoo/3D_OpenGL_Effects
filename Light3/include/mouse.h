struct Mouse {
	//Mouse parameters
	bool firstMouse = true;
	const float sensitivity = 0.1f;
	float pitch = 0.0f, yaw = -90.0f;
	float lastX = 0.0f, lastY = 0.0f;
	float fov = 45.0f;
};
