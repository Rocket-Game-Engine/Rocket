#pragma once

#if 0

#include "glad/gl.h"

#include "Coordinator.h"
#include "Components/Transform.h"
#include "Components/Sprite.h"

class RenderSpriteSystem : public System
{
public:
    public:
    void Do()
    {
        Coordinator* cd = Coordinator::Get();
        for (Entity e : mEntities)
        {
            Sprite& s = cd->GetComponent<Sprite>(e);
            Transform& t = cd->GetComponent<Transform>(e);
            TextureInfo info = Textures::TextureMap.at(s.texture_key);
            Shader shader = Shaders::ShaderMap[s.shader_key];

            s.SetOutputData(t.x, t.y);

            glBindBuffer(GL_ARRAY_BUFFER, s.VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(s.vertices), s.vertices, GL_STATIC_DRAW);

            glBindTexture(GL_TEXTURE_2D, info.tex_id);

            glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 5 * sizeof(GLdouble), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 5 * sizeof(GLdouble), (void*)(3 * sizeof(GLdouble)));
            glEnableVertexAttribArray(1);

            shader.Use();
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    Signature GetSignature() override
    {
        Signature sig;
        Coordinator* cd = Coordinator::Get();
        sig[cd->GetComponentType<Transform>()].flip();
        sig[cd->GetComponentType<Sprite>()].flip();
        return sig;
    }
};

#endif
