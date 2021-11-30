#shader vertex
#version 330 core
layout(Location = 0) in vec2 aPos;
layout(Location = 1) in vec2 aTextureCoord;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
    TexCoords = aTextureCoord;
}




#shader fragment
#version 330 core

uniform sampler2D Scene;
uniform sampler2D CrossHair;
uniform bool HasHDR;
uniform float exposure;

const float gamma = 2.2;

in vec2 TexCoords;
out vec4 FragColor;


vec4 GetCrossHairTexel()
{
    return texture(CrossHair, 8 * TexCoords + vec2(4.5));
}


void main()
{
    //------------------------------------------------------------------------------------------------------
    // This chunk of code is a hack for implementing crossHair
    // It involves manual Blending(alpha test), UV manipulationa and UV check
    vec4 CrossHair = GetCrossHairTexel();
    if (CrossHair.a > 0.1)
    {
        vec2 test = 8 * TexCoords + vec2(4.5);
        if ((TexCoords.x > 0.45 && TexCoords.x < 0.55) && (TexCoords.y > 0.45 && TexCoords.y < 0.55))
        {
            FragColor = CrossHair;
            return;
        }
    }

    //-------------------------------------------------------------------------------------------------------

    if (HasHDR)
    {
        vec3 hdrColor = texture(Scene, TexCoords).rgb;

        // exposure tone mapping
        vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

        // gamma correction 
        mapped = pow(mapped, vec3(1.0 / gamma));

        FragColor = vec4(mapped, 1.0);
    }
    else
    {
        FragColor = vec4(texture(Scene, TexCoords));
    }
}