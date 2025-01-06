#type vertex
#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main(){
	v_TexCoord = a_TexCoord;
	gl_Position = u_projection * u_view * u_model * vec4(a_Pos, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform float u_TilingFactor;
uniform sampler2D u_Texture;

void main(){
	FragColor = texture(u_Texture, v_TexCoord * u_TilingFactor) * u_Color;
}