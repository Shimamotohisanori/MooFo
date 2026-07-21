#pragma once

/** トゥーン輪郭線エフェクトクラス */
namespace nsK2Engine {
    class ToonOutlineEffect : public Noncopyable
    {
    public:
		/** 初期化 */
        void Init(Texture& normalTex, Texture& depthTex);

		/** 描画 */
        void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

		/** デバッグ用: マスクをフレームバッファに描画 */
        void DebugRenderMaskToFrameBuffer(RenderContext& rc);

    private:
		/** 輪郭線エフェクト用の定数バッファ構造体 */
        struct OutlineCB
        {
			/** 輪郭線の色 */
            Vector2 texelSize;

			/** 輪郭線の深度差の閾値 */
            float depthThreshold;

			/** 輪郭線の法線差の閾値 */
            float normalThreshold;

			/** 輪郭線の色 */
            float outlineColorR, outlineColorG, outlineColorB;

			/** 輪郭線のアルファ値 */
            float outlineAlpha;

			/** 輪郭線の深度距離の強さ */
            float depthDistancePower;

			/** 輪郭線の太さ */
            float lineThickness;

			/** 輪郭線の参照距離 */
            float referenceDistance;
        };

		/** 輪郭線エフェクト用の定数バッファ */
        OutlineCB m_cbData;
        // ConstantBuffer m_cb; ← 削除。Spriteクラスが内部で自前のGPUバッファを持つので不要。

		/** 輪郭線マスクを生成するスプライト */
        Sprite m_maskSprite;

		/** 輪郭線マスクを描画するレンダーターゲット */
        RenderTarget m_maskRenderTarget;

		/** 輪郭線マスクをフレームバッファに描画するスプライト */
        Sprite m_compositeSprite;
    };
}