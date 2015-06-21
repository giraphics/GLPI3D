precision mediump float;
varying vec2 TexCoord;
varying vec4        FinalColor;
uniform sampler2D Tex1;
void main() {      
    //gl_FragColor = FinalColor;
	//gl_FragColor = texture(Tex1, TexCoord);
	gl_FragColor = texture(Tex1, vec2(TexCoord.x,1.0-TexCoord.y));
} 
