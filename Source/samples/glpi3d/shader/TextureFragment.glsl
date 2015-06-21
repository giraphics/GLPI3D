precision mediump float;
varying vec2 TexCoord;
uniform sampler2D Tex1;

void main() {
	// vec4(1.0, 0.0, 0.0, 1.0);
	gl_FragColor = texture(Tex1, TexCoord);
}
