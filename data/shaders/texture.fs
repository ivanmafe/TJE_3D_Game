varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;
uniform vec3 u_light_direction;

void main()
{

	vec2 uv = v_uv;
	vec4 color = u_color * texture2D( u_texture, uv );

	// skip small alpha values
	if(color.a < 0.5) discard;

	// phong
	vec3 N = normalize(v_normal);
	vec3 L = normalize(u_light_direction);
	float NdotL = clamp( dot(N,L), 0.0, 1.0);
	//NdotL = round(NdotL * 28.0) / 28.0;
	vec3 light = NdotL * vec3(1.0, 1.0, 1.0) * 0.8;
	// we add some ambient light
	light += vec3(0.3,0.3,0.3) * 1.4;
	color.xyz *= light;

	gl_FragColor = color;
}













/*
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;
uniform vec3 u_light_direction;

void main()
{

	vec2 uv = v_uv;
	vec4 color = u_color * texture2D( u_texture, uv );

	// skip small alpha values
	if(color.a < 0.5) discard;

	// phong
	vec3 N = normalize(v_normal);
	vec3 L = normalize(u_light_direction);
	float NdotL = clamp( dot(N,L), 0.0, 1.0);
	vec3 light = NdotL * vec3(1.0, 1.0, 1.0) * 0.8;
	// we add some ambient light
	light += vec3(0.3,0.3,0.3);
	color.xyz *= light;

	gl_FragColor = color;
}
*/