precision mediump float;

// Material property
uniform vec3    MaterialAmbient;
uniform vec3    MaterialSpecular;
uniform vec3    MaterialDiffuse;
uniform float  Time;

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
//float  Time = 0.5;

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

varying float objectY;
float Frequency = 3.0;

void main() {
    float offset    = Time/10.0;

    // GENERATE fractional value 0.0, 0.1, ........, 0.9
    float sawtooth  = fract((objectY+offset) * Frequency);
    
    // Produce values in the range between [-1, 1]
    float triangle  = 2.0 * sawtooth - 1.0;
    
    // Produce continous range from [ 1.0 ..... 0.0 ..... 1.0 ]
    triangle        = abs(triangle);

    float dp        = length(vec2 (dFdx(objectY+offset), dFdy(objectY+offset)));
    float edge      = dp * Frequency * 4.0;
    float square    = 0.0;

    square          = smoothstep(0.5 - edge, 0.5 + edge, triangle);
    gl_FragColor    = vec4(vec3 (square)*PhongShading(), 1.0);
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
