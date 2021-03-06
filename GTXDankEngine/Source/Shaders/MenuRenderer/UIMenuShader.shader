#shader vertex
#version 450 core

layout(Location = 0) in vec2 aPos;
layout(Location = 1) in vec4 aRGBA;
layout(Location = 2) in vec2 aTexCoord;


out vec4 rgbaOut;
out vec2 texCoordOut;

void main()
{    
    rgbaOut = aRGBA;
    gl_Position = vec4(aPos, 0.0f, 1.0f);
    texCoordOut = aTexCoord;
}



#shader fragment
#version 450 core


in vec4 rgbaOut;
in vec2 texCoordOut;

out vec4 FragColor;

uniform bool haveTxtr;
uniform sampler2D txtr;

uniform vec3 shade;

void main()
{
    // no texture at the moment
    //gl_FragColor = texture(txtr, texCoordOut) * rgbaOut;
    vec4 outColor;
    if (haveTxtr)
    {
        outColor = texture(txtr, texCoordOut);
    }
    else
    {
        outColor = rgbaOut;
    }
    FragColor = outColor * vec4(shade,1.0f);
    //FragColor = vec4(1.0, 0.0, 0.0, 1.0f); /// debug
}


