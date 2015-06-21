attribute vec4 VertexPosition;
attribute vec2 VertexTexCoord;
uniform mat4    ModelViewProjectionMatrix;
varying vec2 TexCoord;
void main() {
    TexCoord = VertexTexCoord;
    gl_Position = ModelViewProjectionMatrix * VertexPosition;
}
