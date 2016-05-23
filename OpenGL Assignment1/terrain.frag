#version 330 core

in vec3 fragmentColor;
out vec4 gl_FragColor;

void main(){

	gl_FragColor = vec4(fragmentColor, 1.0);

}