attribute vec4 a_bones;
attribute vec4 a_weights;

uniform mat4 u_bones[128];

void main()
{	
	v_position = a_vertex;
	v_normal = a_normal;

	//apply skinning
	vec4 v = vec4(a_vertex,1.0);
	v_position =	(u_bones[int(a_bones.x)] * a_weights.x * v + 
			u_bones[int(a_bones.y)] * a_weights.y * v + 
			u_bones[int(a_bones.z)] * a_weights.z * v + 
			u_bones[int(a_bones.w)] * a_weights.w * v).xyz;

	vec4 N = vec4(a_normal,0.0);
	v_normal =	(u_bones[int(a_bones.x)] * a_weights.x * N + 
			u_bones[int(a_bones.y)] * a_weights.y * N + 
			u_bones[int(a_bones.z)] * a_weights.z * N + 
			u_bones[int(a_bones.w)] * a_weights.w * N).xyz;
	v_normal = normalize(v_normal);

}