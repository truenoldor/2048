varying mediump vec4 result_color;
varying mediump vec2 result_uv;
uniform lowp sampler2D base_texture;
uniform lowp sampler2D alpha_texture;

void main()
{
	gl_FragColor = result_color * texture2D(base_texture, result_uv);
	gl_FragColor[0] = 0.0;
	gl_FragColor[1] = 0.0;
	gl_FragColor[2] = 0.0;
}