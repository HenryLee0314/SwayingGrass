#version 410 core
out vec4 FragColor;

in vec3 gsFragPos;
in vec3 gsEn;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform int specularPower;

void main()
{
	vec3 ambient = ambientStrength * lightColor;

    vec3 normalDir = normalize(gsEn);
    vec3 lightDir = normalize(lightPos - gsFragPos);
    float diff = max(dot(normalDir, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diff * lightColor;

    vec3 viewDir = normalize(viewPos - gsFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfwayDir, normalDir), 0.0), specularPower);
    vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * vec3(0, 1, 0);//objectColor;

    FragColor = vec4(result, 1.0);
}