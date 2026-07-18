#include "k2EnginePreCompile.h"
#include "ToonOutlineEffect.h"

namespace nsK2Engine {

    void ToonOutlineEffect::Init(Texture& normalTex, Texture& depthTex)
    {
		/** 画面サイズを取得 */
        int screenWidth = g_graphicsEngine->GetFrameBufferWidth();
        int screenHeight = g_graphicsEngine->GetFrameBufferHeight();

		/** 輪郭線マスク用のレンダーターゲットを作成 */
        float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        m_maskRenderTarget.Create(
            screenWidth, screenHeight, 1, 1,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_FORMAT_UNKNOWN,
            clearColor
        );

		/** 輪郭線エフェクト用の定数バッファを初期化 */
        m_cbData.texelSize = Vector2(1.0f / screenWidth, 1.0f / screenHeight);
        m_cbData.depthThreshold = 30.0f;
        m_cbData.depthDistancePower = 0.13f;
		m_cbData.lineThickness = 1.2f;
		m_cbData.referenceDistance = 0.01f;
        m_cbData.normalThreshold = 0.8f;
        m_cbData.outlineColorR = 0.0f;
        m_cbData.outlineColorG = 0.0f;
        m_cbData.outlineColorB = 0.0f;
        m_cbData.outlineAlpha = 1.0f;
        // m_cb.Init(...) ← 削除。この呼び出し自体が不要かつ有害だった。

		/** 輪郭線マスクを生成するスプライトを初期化 */
        SpriteInitData maskInitData;
        maskInitData.m_textures[0] = &depthTex;
        maskInitData.m_textures[1] = &normalTex;
        maskInitData.m_width = screenWidth;
        maskInitData.m_height = screenHeight;
        maskInitData.m_fxFilePath = "Assets/shader/postEffect/toonOutlineMask.fx";
        maskInitData.m_vsEntryPointFunc = "VSMain";
        maskInitData.m_psEntryPoinFunc = "PSMain";
        maskInitData.m_alphaBlendMode = AlphaBlendMode_None;
        maskInitData.m_expandConstantBuffer = &m_cbData;
        maskInitData.m_expandConstantBufferSize = sizeof(OutlineCB);
        maskInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        m_maskSprite.Init(maskInitData);

        // (以下、copyInitDataの部分は変更なし)
		/** 輪郭線マスクをフレームバッファに描画するスプライトを初期化 */
        SpriteInitData compositeInitData;
        compositeInitData.m_textures[0] = &m_maskRenderTarget.GetRenderTargetTexture();
        compositeInitData.m_width = screenWidth;
        compositeInitData.m_height = screenHeight;
        compositeInitData.m_fxFilePath = "Assets/shader/sprite.fx";
        compositeInitData.m_psEntryPoinFunc = "PSMain";
        compositeInitData.m_alphaBlendMode = AlphaBlendMode_Trans; /** 重要 透明部分を透過させる */
        compositeInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT; /** メインRTのフォーマットに合わせる */
        m_compositeSprite.Init(compositeInitData);
    }

    void ToonOutlineEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
    {
        /** 1. マスクを生成(深度・法線から輪郭線マスクを作る) */
        rc.WaitUntilToPossibleSetRenderTarget(m_maskRenderTarget);
        rc.SetRenderTargetAndViewport(m_maskRenderTarget);
        m_maskSprite.Draw(rc);
        rc.WaitUntilFinishDrawingToRenderTarget(m_maskRenderTarget);

        /** 2. マスクをメインレンダーターゲットに重ねて合成(輪郭以外は透明) */ 
        rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
        rc.SetRenderTargetAndViewport(mainRenderTarget);
        m_compositeSprite.Draw(rc);
        rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
    }

    void ToonOutlineEffect::DebugRenderMaskToFrameBuffer(RenderContext& rc)
    {
        /** 1. マスクを生成。 */
        rc.WaitUntilToPossibleSetRenderTarget(m_maskRenderTarget);
        rc.SetRenderTargetAndViewport(m_maskRenderTarget);
        m_maskSprite.Draw(rc);
        rc.WaitUntilFinishDrawingToRenderTarget(m_maskRenderTarget);

        /** 2. マスクをフレームバッファに直接コピー。 */
        rc.SetRenderTarget(
            g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
            g_graphicsEngine->GetCurrentFrameBuffuerDSV()
        );

		/** ビューポートとシザリング矩形をフレームバッファのサイズに設定 */
        D3D12_VIEWPORT viewport;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferWidth());
        viewport.Height = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferHeight());
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        rc.SetViewportAndScissor(viewport);

        m_compositeSprite.Draw(rc);
    }
}