/* Vertex information */
attribute vec3  Normal;
attribute vec4  VertexPosition;
attribute vec2 VertexTexCoord;

// Material property
uniform vec3    MaterialAmbient;
uniform vec3    MaterialSpecular;
uniform vec3    MaterialDiffuse;
uniform float   ShininessFactor;

// Light property
uniform vec3    LightAmbient;
uniform vec3    LightSpecular;
uniform vec3    LightDiffuse;
                                   
uniform vec3    LightPosition;
                     
uniform mat4    ModelViewProjectionMatrix;
uniform mat4    ModelViewMatrix;
uniform mat3    NormalMatrix;
varying vec4    FinalColor;
varying vec2 TexCoord;

vec3 nNormal, eyeCoord, nLight, V, R, ambient, diffuse, specular;
float sIntensity, cosAngle;
                          
vec3 GouraudShading()
{
    nNormal   = normalize ( NormalMatrix * Normal );
    eyeCoord  = vec3 ( ModelViewMatrix * VertexPosition );
    nLight    = normalize( LightPosition - eyeCoord );
    
    // Diffuse Intensity
    cosAngle = max( 0.0, dot( nNormal, nLight ));
    
    // Specular Intensity
    V       = normalize( -eyeCoord );
    R       = reflect( -nLight, nNormal );
    sIntensity=pow( max(0.0, dot(R, V) ), ShininessFactor);
    
    // ADS color as result of Material & Light interaction
    ambient = MaterialAmbient  * LightAmbient;//Ambient light
    diffuse = MaterialDiffuse  * LightDiffuse;//Diffuse light
    specular = MaterialSpecular* LightSpecular;//Specular light
    
    return ambient+ (cosAngle*diffuse) + (sIntensity*specular);
}
                    
void main()
{       
    FinalColor = vec4(GouraudShading(), 1);
    gl_Position = ModelViewProjectionMatrix * VertexPosition;
return;
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
