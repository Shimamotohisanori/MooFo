#pragma once
#include"graphics/SpriteRender.h"
namespace nsK2Engine {
    class AnimSpriteRender
    {
    public:
        /// <summary>
        /// 初期化。
        /// </summary>
        /// <param name="filePathList">DDSファイルパスの配列</param>
        /// <param name="frameCount">フレーム数</param>
        /// <param name="w">幅</param>
        /// <param name="h">高さ</param>
        /// <param name="fps">1秒あたりのフレーム数</param>
        void Init(const std::vector<std::string>& filePathList,
            int frameCount, float w, float h, float fps = 10.0f);

        /** ロードが重いので分割するための関数*/
        void PrepareFrameCount(int frameCount, float w, float h, float fps = 10.0f);

        /** 1フレーム分だけ追加ロードする関数*/
        void AddFrame(const std::string& filePath);

        /** 最終フレームより何コマ手前で止めるかを設定する */
        void SetFinishFrameOffset(int offset) { m_finishFrameOffset = offset; }

        bool IsLoadComplete() const
        {
            return static_cast<int>(m_sprites.size()) >= m_frameCount;
        }
        void Update();
        void Draw(RenderContext& rc);
        void SetPosition(const Vector3& pos);
        void SetScale(const Vector3& scale);
        /** Gif画像を最初のフレームに戻す関数*/
        void Reset();

        bool IsFinished()const
        {
            return m_isFinished;
        }
    private:
        std::vector<std::unique_ptr<SpriteRender>>   m_sprites;      // フレームごとのスプライトの配列(ポインタ)
        int     m_frameCount = 0;                                    // 総フレーム数
        int     m_currentFrame = 0;                                  // 現在のフレーム番号
        float   m_fps = 10.0f;                                       // 再生FPS
        float   m_timer = 0.0f;                                      // 経過時間
        float   m_frameInterval = 0.1f;
        float   m_width = 0.0f;                                       //幅
        float   m_height = 0.0f;                                     // 高さ
        // 1フレームの秒数
        Vector3 m_position = Vector3::Zero;                          // 座標
        Vector3 m_scale = Vector3::One;// 大きさ

        /** 全ロード完了状態で、現在最終フレームを表示しているかどうか（毎ループ1回だけtrueになる） */
        bool m_isFinished = false;

        /** 最終フレームから何コマ手前で停止させるか */
        int m_finishFrameOffset = 0;
    };
}


