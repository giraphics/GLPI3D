// Vertex information
attribute vec3  Normal;
attribute vec4  VertexPosition;


// Model View Project matrix
uniform mat4    ModelViewProjectionMatrix;
uniform mat4    ModelViewMatrix;
uniform mat3    NormalMatrix;

varying vec3    normalCoord;
varying vec3    eyeCoord;

void main() 
{
    normalCoord = NormalMatrix * Normal;
    eyeCoord    = vec3 ( ModelViewMatrix * VertexPosition );
    gl_Position = ModelViewProjectionMatrix * VertexPosition;
}
