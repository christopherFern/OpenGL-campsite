#version 330 core
/*
 *  Simple fragment sharder for example four
 */

in vec3 normal;
uniform vec4 objectColour;

void main() {
	vec3 N;
	vec3 L = vec3(0.0, 0.0, 1.0);
	vec4 colour = objectColour;
	float diffuse;

	N = normalize(normal);
	diffuse = dot(N,L);
	if(diffuse < 0.0) {
		diffuse = 0.0;
	}

	gl_FragColor = min(0.3*colour + 0.7*diffuse*colour, vec4(1.0));
	gl_FragColor.a = colour.a;
}