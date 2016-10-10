#version 430

in vec3 Position;
in vec3 Normal;

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

layout( location = 0 ) out vec4 FragColor;


void main() {
    vec3 ambient = Spot.intensity * Ka;
    //vertex position 
    //can use different color for each object 
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
    //beta is the dot product of the the spotlight direction and light direction from the vertex
    float B = acos(dot(normalize(Spot.direction), -light_direction)); 
    float attenuation_factor = pow(cos(B), Spot.exponent);
    if(B < radians(Spot.cutoff)){
        //attenuation factor is cos^spot.expo(B)
        FragColor = vec4((((attenuation_factor) * (specular + diffuse)) + ambient), 1.0);
    }
    else
    {
        FragColor = vec4(ambient, 1.0);
    }
    
}


