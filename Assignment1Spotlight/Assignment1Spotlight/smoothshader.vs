#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

struct SpotLight {
    vec4 position;   // Position in eye coords
    vec3 intensity;
    vec3 direction;  // Direction of the spotlight in eye coords.
    float exponent;  // Angular attenuation exponent
    float cutoff;    // Cutoff angle
};
uniform SpotLight Spot;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;            // Ambient reflectivity
uniform vec3 Ks;            // Specular reflectivity
uniform float Shininess;    // Specular shininess factor
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;



void main()
{
  	// Get the position and normal in eye space
    vec3 Normal = normalize( NormalMatrix * VertexNormal);
    vec3 Position = vec3( ModelViewMatrix * vec4(VertexPosition,1.0) );

    // Calculate diffuse and specular reflection and add to the light intensity
    vec3 ambient = Spot.intensity * Ka;

    vec3 view_direction =  normalize(-Position);
    vec3 light_direction = normalize(vec3(Spot.position) - Position);
	vec3 H = normalize(light_direction + view_direction);  //slide 49 lec2_wk7
	vec3 diffuse = Kd * Spot.intensity * max(dot(Normal,light_direction), 0.0);
    vec3 specular;
	if(diffuse == vec3(0.0))
        specular = vec3(0.0);
	else{
        specular = Ks * Spot.intensity * pow(max(dot(Normal,H), 0.0),Shininess);  //specular reflection coefficient * Intensity of light source * cos(angle between viewer and reflection of light source) ^ glossiness of material 
    }

	float B = acos(dot(normalize(Spot.direction), -light_direction)); 
    float attenuation_factor = pow(cos(B), Spot.exponent);
    if(B < radians(Spot.cutoff)){
        //attenuation factor is cos^spot.expo(B)
        LightIntensity = ((attenuation_factor) * (specular + diffuse)) + ambient;
    }
    else
    {
        LightIntensity = ambient;
    }

    gl_Position = MVP * vec4(VertexPosition,1.0);
}

