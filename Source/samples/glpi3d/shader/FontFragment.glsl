precision mediump float;
varying vec2 TexCoord;
uniform sampler2D FontTexture;
uniform vec4 TextColor;
void main() {  
   vec4 texcol = texture(FontTexture, TexCoord);
   gl_FragColor = vec4(vec3(TextColor.r, TextColor.g, TextColor.b), texcol.a);
}