/* Vertex information */
attribute vec3  Normal;
attribute vec4  VertexPosition;
attribute vec2 VertexTexCoord;
                                   
uniform vec3    LightDiffuse;
uniform vec3    MaterialDiffuse;
uniform vec3    LightPosition;
                     
uniform mat4    ModelViewProjectionMatrix;
uniform mat4    ModelViewMatrix;
uniform mat3    NormalMatrix;
varying vec4    FinalColor;
varying vec2 TexCoord;
                                                     
void main()
{       
    TexCoord = VertexTexCoord;
    vec3 nNormal   = normalize ( NormalMatrix * Normal );
    vec3 eyeCoord  = vec3 ( ModelViewMatrix * VertexPosition );
	vec3 nLight    = normalize( LightPosition - eyeCoord );
	
	/* Calculate cosine between Normal and Light vector */      
    float cosAngle = max( 0.0, dot( nNormal, nLight ));

    /* Compute diffuse Light and Material interaction */
    vec3 diffuse   = MaterialDiffuse  * LightDiffuse;
                                 
    /* Calculate net intensity */
    vec3 Color 	   = cosAngle * diffuse;
    FinalColor 	   = vec4(Color, 1.0);
    //gl_Position    = ModelViewProjectionMatrix * VertexPosition;
    gl_Position    = ModelViewProjectionMatrix * VertexPosition;
}
