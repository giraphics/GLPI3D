precision mediump float;

// Material property
uniform vec3    MaterialAmbient;
uniform vec3    MaterialSpecular;
uniform vec3    MaterialDiffuse;

// Light property
uniform vec3    LightAmbient;
uniform vec3    LightSpecular;
uniform vec3    LightDiffuse;
uniform float   ShininessFactor;

uniform vec3    LightPosition;

varying vec3    normalCoord;
varying vec3    eyeCoord;

vec3 normalizeNormal, normalizeEyeCoord, normalizeLightVec, V, R, ambient, diffuse, specular;
float sIntensity, cosAngle;

vec3 PhongShading()
{
    normalizeNormal   = normalize( normalCoord );
    normalizeEyeCoord = normalize( eyeCoord );
    normalizeLightVec = normalize( LightPosition - eyeCoord );
    
    // Diffuse Intensity
    cosAngle = max( 0.0, dot( normalizeNormal, normalizeLightVec ));

    // Specular Intensity
    V = -normalizeEyeCoord; // Viewer's vector
    R = reflect( -normalizeLightVec, normalizeNormal ); // Reflectivity
    sIntensity 	= pow( max( 0.0, dot( R, V ) ), ShininessFactor );

    // ADS color as result of Material & Light interaction
    ambient    = MaterialAmbient  * LightAmbient;
    diffuse    = MaterialDiffuse  * LightDiffuse;
    specular   = MaterialSpecular * LightSpecular;

    return ambient + ( cosAngle * diffuse ) + ( sIntensity * specular );
}

void main() {
    gl_FragColor = vec4(PhongShading(), 1.0);
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
