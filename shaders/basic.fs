#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D   texture1;
uniform samplerCube shadowMap;
uniform vec3        lightPos;
uniform vec3        viewPos;
uniform float       farPlane;

// 20-direction PCF kernel — good balance between quality and cost
vec3 kPCFOffsets[20] = vec3[](
    vec3( 1, 1, 1), vec3( 1,-1, 1), vec3(-1,-1, 1), vec3(-1, 1, 1),
    vec3( 1, 1,-1), vec3( 1,-1,-1), vec3(-1,-1,-1), vec3(-1, 1,-1),
    vec3( 1, 1, 0), vec3( 1,-1, 0), vec3(-1,-1, 0), vec3(-1, 1, 0),
    vec3( 1, 0, 1), vec3(-1, 0, 1), vec3( 1, 0,-1), vec3(-1, 0,-1),
    vec3( 0, 1, 1), vec3( 0,-1, 1), vec3( 0,-1,-1), vec3( 0, 1,-1)
);

float calcShadow(vec3 fragPos) {
    vec3  fragToLight    = fragPos - lightPos;
    float currentDepth   = length(fragToLight);
    float shadow         = 0.0;
    float bias           = 0.15;
    float diskRadius     = 0.05;

    for (int i = 0; i < 20; ++i) {
        float closestDepth = texture(shadowMap, fragToLight + kPCFOffsets[i] * diskRadius).r
                             * farPlane;
        if (currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    return shadow / 20.0;
}

void main() {
    vec3 norm     = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir  = normalize(viewPos  - FragPos);

    // Ambient — always present so shadowed faces aren't pure black
    vec3 ambient  = 0.35 * vec3(1.0);

    // Diffuse
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3  diffuse = diff * vec3(1.0);

    // Specular (Blinn-Phong half-vector)
    vec3  halfDir = normalize(lightDir + viewDir);
    float spec    = pow(max(dot(norm, halfDir), 0.0), 64.0);
    vec3  specular = 0.5 * spec * vec3(1.0);

    float shadow = calcShadow(FragPos);
    vec3  result = (ambient + (1.0 - shadow) * (diffuse + specular))
                   * texture(texture1, TexCoord).rgb;

    FragColor = vec4(result, 1.0);
}
