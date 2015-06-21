precision mediump float;
varying vec2 TexCoord;
varying vec4 Color;
uniform sampler2D Tex1;

void main() {
	gl_FragColor = texture(Tex1, TexCoord);
	//gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);//Color;
}
