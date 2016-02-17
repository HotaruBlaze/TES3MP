#define MAX_LIGHTS 8

vec4 doLighting(vec3 viewPos, vec3 viewNormal, vec4 vertexColor)
{
    vec3 lightDir;
    float d;

#if @colorMode == 2
    vec4 diffuse = vertexColor;
    vec3 ambient = vertexColor.xyz;
#else
    vec4 diffuse = gl_FrontMaterial.diffuse;
    vec3 ambient = gl_FrontMaterial.ambient.xyz;
#endif
    vec4 lightResult = vec4(0.0, 0.0, 0.0, diffuse.a);

    for (int i=0; i<MAX_LIGHTS; ++i)
    {
        lightDir = gl_LightSource[i].position.xyz - (viewPos.xyz * gl_LightSource[i].position.w);
        d = length(lightDir);
        lightDir = normalize(lightDir);

        lightResult.xyz += ambient * gl_LightSource[i].ambient.xyz;
        lightResult.xyz += diffuse.xyz * gl_LightSource[i].diffuse.xyz * clamp(1.0 / (gl_LightSource[i].constantAttenuation + gl_LightSource[i].linearAttenuation * d + gl_LightSource[i].quadraticAttenuation * d * d), 0.0, 1.0)
                * max(dot(viewNormal.xyz, lightDir), 0.0);
    }

    lightResult.xyz += gl_LightModel.ambient.xyz * ambient;

#if @colorMode == 1
    lightResult.xyz += vertexColor.xyz;
#else
    lightResult.xyz += gl_FrontMaterial.emission.xyz;
#endif

    return lightResult;
}