#shader vertex
#version 450 core

layout(Location = 0) in vec2 vertexPosition;
layout(Location = 1) in vec2 vTexCoords;

uniform vec2 centerLocation;
uniform mat4 projection;

out vec2 texCoordOut;

void main()
{
    texCoordOut = vTexCoords;
    gl_Position = projection * vec4(centerLocation + vertexPosition, 0.0f, 1.0f);
}



#shader fragment
#version 450 core


in vec2 texCoordOut;

/// texture for the slider
uniform sampler2D txtr;

out vec4 FragColor;

void main()
{
    vec4 outColor;
    if (texCoordOut.x == -1)
    {
        /// rgb(56,56,56)
        outColor = vec4(vec3(56.0/255.0), 1.0);
    }
    else
    {
        outColor = texture(txtr, texCoordOut);
    }
    FragColor = outColor;
}

