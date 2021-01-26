#version 420 core

out vec4 FragColor;

void main()
{
	if(gl_FragCoord.x < 400 && gl_FragCoord.y < 300)
		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	else if(gl_FragCoord.x > 400 && gl_FragCoord.y > 300)
		FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	else if(gl_FragCoord.x < 400 && gl_FragCoord.y > 300)
		FragColor =  vec4(0.0, 0.0, 1.0, 1.0);
	else if(gl_FragCoord.x > 400 && gl_FragCoord.y < 300)
		FragColor = vec4(0.0, 1.0, 1.0, 1.0);
}
