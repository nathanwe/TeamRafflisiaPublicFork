#shader vertex
#version 450 core

layout(Location = 0) in vec2 aPos;
//layout(Location = 1) in vec4 aRGBA;
layout(Location = 1) in vec2 aTexCoord;


//out struct VS_OUT
//{
//    vec4 RGBA;
//    vec2 TexCoord;
//} vs_out;

uniform mat4 orthoMat;

//out vec4 rgbaOut;
out vec2 texCoordOut;

void main()
{
    //gl_Position = orthoMat * vec4(aPos, 0.0f, 1.0f);

  //  vs_out.RGBA     = aRGBA;
   // vs_out.TexCoord = aTexCoord;
    
//    rgbaOut = aRGBA;

    gl_Position = vec4(aPos, 0.0f, 1.0f);
    texCoordOut = aTexCoord;
}



#shader fragment
#version 450 core

//in struct VS_OUT
//{
//    vec4 RGBA;
//    vec2 TexCoord;
//} vs_out;

//in vec4 rbgaOut;
in vec2 texCoordOut;

out vec4 FragColor;

//uniform sampler2D txtr;

void main()
{
    // no texture at the moment
    //gl_FragColor = texture(txtr, vs_in.TexCoord) * vs_in.RGBA;
    //FragColor = vs_out.RGBA;
    //FragColor = rgbaOut;
    FragColor = vec4(0.0, 1.0, 0.0, 1.0f); /// debug
}


