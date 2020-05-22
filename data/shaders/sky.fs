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

	// we add some ambient light
	vec3 light = vec3(1.0, 1.0, 1.0) * 0.7;
	light += vec3(0.3,0.3,0.3) * 0.8;
	color.xyz *= light;

	gl_FragColor = color;
}
