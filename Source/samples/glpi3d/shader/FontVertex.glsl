attribute vec3  VertexPosition;

attribute vec2  VertexTexCoord;

varying vec2 TexCoord;

uniform mat4 ModelViewProjectMatrix;

void main( void ) {
  TexCoord    = VertexTexCoord;
  gl_Position = ModelViewProjectMatrix * vec4(VertexPosition,1.0);
}