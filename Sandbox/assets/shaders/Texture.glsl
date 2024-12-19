#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	v_TexCoord = a_TexCoord;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main(){
	FragColor = texture(u_Texture, v_TexCoord);
}