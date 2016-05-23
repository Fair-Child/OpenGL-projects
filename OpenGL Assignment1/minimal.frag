#version 330 core

in vec3 fragmentColor;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

out vec4 gl_FragColor;

void main(){

	gl_FragColor = vec4(texture(texture_diffuse1, TexCoords));

}