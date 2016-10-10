#version 400

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord_2;
in vec3 light_direction;
in vec3 view_direction;

uniform sampler2D Tex1;
uniform sampler2D NormalMap;

struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // A,D,S intensity
};
uniform LightInfo Light;

struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
};
uniform MaterialInfo Material;

layout( location = 0 ) out vec4 FragColor;

void phongModel( vec3 texColor, vec3 norm, out vec3 ambAndDiff, out vec3 spec ) {
    vec3 r = reflect( -light_direction, norm );
    vec3 ambient = Light.Intensity * Material.Ka;
    float sDotN = max( dot(light_direction,norm), 0.0 );
    vec3 diffuse = Light.Intensity  * sDotN  * Material.Kd;
    spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light.Intensity * Material.Ks *
               pow( max( dot(r,view_direction), 0.0 ), Material.Shininess );
    ambAndDiff = ambient + diffuse;
}


void main() {
    vec3 ambAndDiff, spec;
    vec4 texColor = texture( Tex1, TexCoord_2 );
    vec4 normal = texture( NormalMap, TexCoord_2 );

    normal = vec4(normalize(vec3(2.0*normal-1.0)), 1.0f);
    phongModel(texColor.rgb, normal.xyz, ambAndDiff, spec);
    FragColor = vec4(ambAndDiff, 1.0 ) * texColor + vec4(spec,1);
    //FragColor = vec4( phongModel(normal.xyz, texColor.rgb, ambAndDiff, spec), 1.0 );
}
