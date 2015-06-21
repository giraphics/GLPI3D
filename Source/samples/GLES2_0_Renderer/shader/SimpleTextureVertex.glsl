attribute vec4 VertexPosition;
attribute vec2 VertexTexCoord;
uniform mat4 ModelViewProjectionMatrix;
uniform vec4 RectColor;
varying vec2 TexCoord;
varying vec4 Color;

void main() {
    TexCoord = VertexTexCoord;
	Color = RectColor;//vec4(1.0, 0.0, 0.0, 1.0);
    gl_Position = ModelViewProjectionMatrix * VertexPosition;
}