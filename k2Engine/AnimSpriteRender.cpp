#include "k2EnginePreCompile.h"
#include "AnimSpriteRender.h"
namespace nsK2Engine {
    void AnimSpriteRender::Init(
        const std::vector<std::string>& filePathList,
        int frameCount, float w, float h, float fps)
    {
        m_frameCount = frameCount;
        m_fps = fps;
        m_frameInterval = 1.0f / fps;

        // resizeではなくmake_uniqueで初期化することで
        // SpriteRenderのコンストラクタが呼ばれる
        for (int i = 0; i < frameCount; i++)
        {
            auto sprite = std::make_unique<SpriteRender>();
            sprite->Init(filePathList[i].c_str(), w, h, AlphaBlendMode_Trans);
            m_sprites.push_back(std::move(sprite));
        }
    }

    void AnimSpriteRender::PrepareFrameCount(int frameCount, float w, float h, float fps)
    {
        m_frameCount = frameCount;
        m_fps = fps;
        m_frameInterval = 1.0f / fps;
        m_width = w;
        m_height = h;
        m_currentFrame = 0;
        m_timer = 0.0f;
        m_sprites.clear();
        m_sprites.reserve(frameCount);
    }


    void AnimSpriteRender::AddFrame(const std::string& filePath)
    {
        if (static_cast<int>(m_sprites.size()) >= m_frameCount)
        {
            return;
        }

        auto sprite = std::make_unique<SpriteRender>();
        sprite->Init(filePath.c_str(), m_width, m_height, AlphaBlendMode_Trans);


        sprite->SetPosition(m_position);
        sprite->SetScale(m_scale);
        sprite->Update();
        m_sprites.push_back(std::move(sprite));
    }
    void AnimSpriteRender::Reset()
    {
        m_currentFrame = 0;
        m_timer = 0.0f;
        m_isFinished = false;
    }





    void AnimSpriteRender::Update()
    {
        /** スプライトが初期化されていない場合は何もしない */
        if (m_sprites.empty())
        {
            return;
        }

        const int loadedCount = static_cast<int>(m_sprites.size());
        const bool isFullyLoaded = (loadedCount >= m_frameCount);

        if (!m_isFinished)
        {
            m_timer += g_gameTime->GetFrameDeltaTime();

            /** 実際に止めたい対象フレーム(最終フレームより手前) */
            const int targetStopFrame =
                (m_frameCount - 1 - m_finishFrameOffset < 0)
                ? 0
                : (m_frameCount - 1 - m_finishFrameOffset);

            while (m_timer >= m_frameInterval)
            {
                m_timer -= m_frameInterval;
                m_currentFrame++;

                if (isFullyLoaded && m_currentFrame >= targetStopFrame)
                {
                    m_currentFrame = targetStopFrame;
                    m_isFinished = true;
                    m_timer = 0.0f;
                    break;
                }

                m_currentFrame %= loadedCount;
            }


        }

        /** 現在表示するフレームの座標を必ず反映させる*/
        if (m_currentFrame < loadedCount)
        {
            m_sprites[m_currentFrame]->SetPosition(m_position);
            m_sprites[m_currentFrame]->SetScale(m_scale);
            m_sprites[m_currentFrame]->Update();
        }
    }

    void AnimSpriteRender::Draw(RenderContext& rc)
    {
        if (m_sprites.empty())
        {
            return;
        }

        int drawIndex = m_currentFrame;
        if (drawIndex >= static_cast<int>(m_sprites.size()))
        {
            drawIndex = static_cast<int>(m_sprites.size()) - 1;
        }

        m_sprites[drawIndex]->Draw(rc);
    }

    void AnimSpriteRender::SetPosition(const Vector3& pos) { m_position = pos; }
    void AnimSpriteRender::SetScale(const Vector3& scale) { m_scale = scale; }
}