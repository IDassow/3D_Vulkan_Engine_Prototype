#version 450


layout(location = 0) in vec3 position; 
layout(location = 1) in vec3 color;  

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push{
	mat4 transform; //4x4 matrix
	vec3 color;
} push;


void main () {
	//gl_Position = vec4(push.transform * position + push.offset, 0.0, 1.0); 

	gl_Position = push.transform * vec4(position, 1.0);
	fragColor = color;
}

//Matrix multiplication is NOT commutative
//i.e.	AB != BA or {(push.transform * position) !=  (position * push.transform)}