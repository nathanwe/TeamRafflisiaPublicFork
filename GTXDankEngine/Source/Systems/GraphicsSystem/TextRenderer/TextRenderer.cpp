#include "pch.h"

#include "TextRenderer.h"

#include "../Core/Shader.h"
#include "../utils/Log.h"


void TextRenderer::Init(std::string fontPath)
{
    FT_Library ft;
    bool val = FT_Init_FreeType(&ft);
    if (val)
    {
        LOG_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
    }
	
	// load font as face
    FT_Face face;
    val = FT_New_Face(ft, fontPath.c_str(), 0, &face);
    if (val)
    {
        LOG_ERROR("ERROR::FREETYPE: Failed to load font {0}", fontPath);
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                LOG_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    
    /// VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /// VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_STATIC_DRAW);

    /// vertex: xyzw
    /// xy - 2d position
    /// zw - 2d texture coords
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void TextRenderer::RenderText(Shader& textShader, std::string text, float x, float y, float scale, glm::vec3 color)
{

    /// Draw outline first
    textShader.setVec3("textColor", glm::vec3(0));
    float outScale = (scale*100.0f + 30.0f) / 100.0f;
    float startx = x;
    // iterate through all characters
    for (auto& c : text)
    {
        Character ch = characters[c];

        textShader.setTexture("text", ch.textureID);

        /// setup vertices and indices
        float xpos = startx + ch.bearing.x * outScale;
        float ypos = y + (ch.bearing.y - ch.size.y) * outScale;
        float wdth = ch.size.x * outScale;
        float hght = ch.size.y * outScale;
        std::vector<float> vertices = {
            /// CLOCKWISE
            xpos,        ypos + hght, 0, 0,   /// bottom left
            xpos,        ypos,        0, 1,   /// top left
            xpos + wdth, ypos,        1, 1,   /// top right

            xpos,        ypos + hght, 0, 0,   /// bottom left
            xpos + wdth, ypos,        1, 1,   /// top right
            xpos + wdth, ypos + hght, 1, 0    /// bottom right      
        };

        /// bind vertex values
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);


        // draw button
        textShader.Bind();
        glBindVertexArray(VAO);
        glDisable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);

        glEnable(GL_DEPTH_TEST);
        glBindVertexArray(0);
        textShader.unBind();

        startx += (ch.advance >> 6) * outScale - 5;
    }



    /// Draw text above outline second
    textShader.setVec3("textColor", color);
    // iterate through all characters
    for (auto& c : text)
    {
        Character ch = characters[c];

        textShader.setTexture("text", ch.textureID);

        /// setup vertices and indices
        float xpos = x + ch.bearing.x * scale;
        float ypos = y + (ch.bearing.y - ch.size.y) * scale;
        float wdth = ch.size.x * scale;
        float hght = ch.size.y * scale;
        std::vector<float> vertices = {
            /// CLOCKWISE
            xpos,        ypos + hght, 0, 0,   /// bottom left
            xpos,        ypos,        0, 1,   /// top left
            xpos + wdth, ypos,        1, 1,   /// top right

            xpos,        ypos + hght, 0, 0,   /// bottom left
            xpos + wdth, ypos,        1, 1,   /// top right
            xpos + wdth, ypos + hght, 1, 0    /// bottom right      
        };

        /// bind vertex values
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);


        // draw button
        textShader.Bind();
        glBindVertexArray(VAO);
        glDisable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);

        glEnable(GL_DEPTH_TEST);
        glBindVertexArray(0);
        textShader.unBind();

        x += (ch.advance >> 6) * scale + 5;
    }
}

