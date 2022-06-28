#version 460 core

out vec4 FragColor;

in vec2 TextCoords;

void main(){
	FragColor.r = 1.0-distance(TextCoords,vec2(0.5))*2.0;
}
