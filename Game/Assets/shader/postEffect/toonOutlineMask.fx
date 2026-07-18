///////////////////////////////////////
// トゥーン輪郭線: エッジマスク生成専用パス
///////////////////////////////////////

/** b0はSpriteクラスの標準的な用途(mvp)に合わせる */
cbuffer cb : register(b0)
{
    float4x4 mvp;
    float4 mulColor;
};

cbuffer OutlineCB : register(b1)
{
    /** 1ピクセルのテクスチャ座標の大きさ */
    float2 texelSize;
    
    /** 輪郭線の検出に使用する深度差の閾値 */
    float depthThreshold;
    
    /** 輪郭線の検出に使用する法線差の閾値 */
    float normalThreshold;
    
    /** 輪郭線の色 */
    float outlineColorR, outlineColorG, outlineColorB;
    
    /** 輪郭線のアルファ値 */
    float outlineAlpha;
    
    /** 深度差の閾値を距離に応じてスケーリングするためのパワー */
    float depthDistancePower;
    
    /** 輪郭線の太さ */
    float lineThickness;
    
    /** 輪郭線の検出に使用する基準距離 */
    float referenceDistance;
};

/** 輪郭線の検出に使用する深度テクスチャと法線テクスチャ */
Texture2D<float4> depthTexture : register(t0);

/** 輪郭線の検出に使用する法線テクスチャ */
Texture2D<float4> normalTexture : register(t1);

/** サンプラーステート */
SamplerState Sampler : register(s0);

/** 頂点シェーダー入力構造体 */
struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

/** ピクセルシェーダー入力構造体 */
struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

/** 頂点シェーダー */
PSInput VSMain(VSInput In)
{
    /** 頂点座標を変換してピクセルシェーダーに渡す */
    PSInput psIn;
    
    /** 頂点座標を変換してピクセルシェーダーに渡す */
    psIn.pos = mul(mvp, In.pos);
    
    /** テクスチャ座標をそのままピクセルシェーダーに渡す */
    psIn.uv = In.uv;
    return psIn;
}

/** ピクセルシェーダー */
float4 PSMain(PSInput In) : SV_Target0
{
    /** 1ピクセルのテクスチャ座標の大きさに輪郭線の太さを掛けた値を計算 */
    float2 offset = texelSize * lineThickness;

    /** 中心の深度値と周囲の深度値を取得 */
    float depthC = depthTexture.Sample(Sampler, In.uv).b;
    float depthLeft = depthTexture.Sample(Sampler, In.uv - float2(offset.x, 0)).b;
    float depthRight = depthTexture.Sample(Sampler, In.uv + float2(offset.x, 0)).b;
    float depthUp = depthTexture.Sample(Sampler, In.uv - float2(0, offset.y)).b;
    float depthDown = depthTexture.Sample(Sampler, In.uv + float2(0, offset.y)).b;

    /** 中心の法線値と周囲の法線値を取得 */
    float depthDiff =
          abs(depthC - depthLeft)
        + abs(depthC - depthRight)
        + abs(depthC - depthUp)
        + abs(depthC - depthDown);

    /** 中心の法線値と周囲の法線値を取得 */
    float distanceRatio = max(depthC, 0.001f) / referenceDistance;
    float scaledThreshold = depthThreshold * pow(max(distanceRatio, 1.0f), depthDistancePower);

    /** 深度差が閾値を超えた場合に輪郭線を描画する */
    float edge = step(scaledThreshold, depthDiff);

    float3 outlineColor = float3(outlineColorR, outlineColorG, outlineColorB);
    return float4(outlineColor, edge * outlineAlpha);
}