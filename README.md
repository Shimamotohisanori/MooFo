# MooFo
<img width="500" height="300" alt="Title" src="https://github.com/user-attachments/assets/f5a60dea-35e9-4b7d-b185-9df67ec950cd" />  

### 河原電子ビジネス専門学校　ゲームクリエイター科(28卒)

### 氏名 : 島本学典(しまもとひさのり：リーダー兼プログラマー)
### 氏名 : 仙波知裕(せんばともひろ：プログラマー)
### 氏名 : 替地謙介(かえちけんすけ：プログラマー)

GithubのURL  
https://github.com/Shimamotohisanori/MooFo

YoutubeのURL  
https://www.youtube.com/watch?v=W8kVCd70rKE  

# 目次
<a id="toc"></a>
・ [MooFo](#moofo)  
・ [目次](#目次)  
・ [作品概要](#作品概要)  
・ [担当ソースコード](#担当ソースコード)  
・ [操作説明](#操作説明)  
・ [ゲーム説明](#ゲーム説明)  
　・◇[ゲーム詳細](#ゲーム詳細)  
　・◇[UIについて](#uiについて)  
　・◇[ミニマップについて](#ミニマップについて)  
　・◇[工夫した点](#工夫した点)  

# 作品概要
***タイトル***  
　MooFo(ムーフォー)  
***制作人数***  
　3人  
***制作期間***  
　2026年2月～2026年8月  
***ゲームジャンル***  
　3Dアクションゲーム  
***プレイ人数***  
　1人  
***使用言語***  
　c++  
***使用ツール***  
　・Visual Studio 2026  
　・Visual Studio Code  
　・Adobe Photoshop 2026  
　・3ds Max 2026  
　・Effekseer  
　・GitHub  
　・Fork  
　・Audio Stock  
***開発環境***  
　・学校内製エンジン(K2Engine)  
　・Windows11  

# 担当ソースコード

### 島本学典

<details>
<summary>ゲーム部分</summary>

## 核心のゲームシステム

・動的スポーンシステム
「フィールド内に常時10体の牛を維持するように処理」
「捕獲数に応じたスポーン範囲の動的変更」のロジック構築。  
Game.cpp / .h

・プレイヤー&ロープアクション
プレイヤー移動、ロープの投擲、ロープの追従・描画・挙動制御の実装。  
Player.cpp / .h  
Rope.cpp / .h  

・オブジェクト・疑似AIの制御
プレイヤー接近時の牛の逃走、UFOにより連れ去り処理、及び基盤となるActor / Characterクラスの設計。
Cow.cpp / .h  
UFO.cpp / .h  
Actor.cpp / .h  
Character.cpp / .h  

</details>  

<details>  
<summary>UI & 演出・カメラ</summary>  

## UI & 演出・カメラ  

・カメラシステム  
ゲーム内カメラの追従及び挙動処理。  
GameCamera.cpp / .h  

・動的UI演出  
カウントダウン時のサイズ・α値アニメーション、スコアのカウンター演出、コンボUI・救出数UIの実装。  
CountDown.cpp / .h  
Score.cpp / .h  
Combo.cpp / .h  
CowRescues.cpp / .h  

・状態遷移UI  
タイトル、ゲームクリア、ゲームオーバー、タイムアウト画面のUI制御及び画像の制作。  
Title.cpp / .h  
GameClear.cpp / .h  
GameOver.cpp / .h  
Game.cpp / .h  

・マップUI制御  
ミニマップ上の牛アイコンの表示・非表示切り替え処理。  
Map.cpp / .h  

・牛の餌UI・クールタイム  
牛が禁止エリアに入った際に表示される餌UIの実装、および餌のクールタイム処理を実装。  
CowFood.cpp / .h  

・照準システム  
ゲーム中に表示される照準の表示・制御システムを実装。  
Aiming.cpp / .h  

・操作説明UI  
ゲーム画面上に操作説明を表示するUIを実装。
InstructionControllerUI.cpp / .h  

・UIパネル  
ゲーム中に表示される各種UIの背面に配置する半透明パネルを実装。  
UIPanels.cpp / .h  

・難易度説明UI  
難易度選択画面において、難易度ごとの説明画像を表示する機能を実装。  
Difficulty.cpp / .h  
DifficultySetting.cpp / .h  

</details>

<details>  
<summary>エンジン拡張 & システム基盤</summary>  

## エンジン拡張 & システム基盤  

・シーン遷移・ロード処理  
ロード中のオブジェクト事前生成、及びポーズ終了後にロード画面を経由してタイトルへ戻る処理を実装。  
LoadingScene.cpp / .h  
Pause.cpp / .h  

・データ永続化  
音量設定をゲームループ間で保持し、次回起動時にも反映される仕組みを実装。  
SoundPause.cpp / .h  

・エフェクトシステム  
エフェクトの読み込み・再生処理を共通化し、UFO追従エフェクトを実装。  
EffectManager.cpp / .h  
CowCaptureController.cpp / .h  

・エンジンの改造  
ライトの明るさ設定の変更、およびエフェクトを扱いやすくするためのエンジン改造。  
SceneLight.cpp / .h  
EffectEmitter.cpp / .h  

・シャドウマップ機能の改造  
ゲーム内の影表現を実現するため、シャドウマップに関連する処理を改造。  
CascadeShadowMapMatrix.cpp / .h  
ShadowMapRender.cpp / .h  

・トゥーンアウトライン機能  
ゲーム内のオブジェクトに輪郭線を表示するため、シェーダー及びレンダリング処理を改造し、トゥーンアウトライン機能を実装。  
toonOutlineMask.fx  
ToonOutlineEffect.cpp / .h  
RenderingEngine.cpp / .h  

</details>  

<details>  
<summary>サウンド</summary>  

## サウンド  

・ボイス・SE管理  
プレイヤーのボイスやSEなど、ゲーム内で使用する音声を管理するVoiceManagerを実装。  
VoiceManager.cpp / .h  

</details>  

### 仙波知裕

<details>
<summary>ゲーム部分</summary>

## ゲーム進行 & システム管理

・ゲームフロー制御  
タイトル、ゲームクリア、ゲームオーバー、ロードクラスの追加及びシーン遷移処理を実装。  
Title.cpp / .h  
GameClear.cpp / .h  
GameOver.cpp / .h  
LoadingScene.cpp / .h  

・スコア・制限時間管理  
スコア表示・増減処理、制限時間システム、アウトゲームへのスコア引継ぎを実装。  
Score.cpp / .h  
Timer.cpp / .h  
Game.cpp / .h  
GameClear.cpp / .h  
GameOver.cpp / .h  

・ゲーム開始演出  
ゲーム開始時のカウントダウン処理を実装。  
CountDown.cpp / .h  

・時間減少システム  
牛の救出に失敗した際に制限時間を減少させる処理を設計・実装。  
DecreaseTimerUI.cpp / .h  

・残機システム  
牛の救出に3回失敗するとゲームオーバーとなる残機システムを設計・実装。  
CowLivesUI.cpp / .h  

・チュートリアルシステム  
難易度「はじめて」をチュートリアルとして機能させるため、チュートリアルの進行やゲームルール、難易度に関するシステムを設計・実装。  
TutorialManager.cpp / .h  

</details>  

<details>  
<summary>オブジェクト配置 & ゲームバランス</summary>  

## オブジェクト配置 & ゲームバランス  

・ランダムスポーン制御  
現在のスポーン範囲を基準として、牛が毎回ランダムな位置に配置される処理を実装。  
LoadingScene.cpp / .h  

・牛の行動制御  
牛の行動パターンとして、フィールド内をランダムに移動する牛と、UFOに向かって移動する牛を実装。  
Cow.cpp / .h  
LoadingScene.cpp / .h  

・UFO制御  
UFOの移動制限処理、及びUFOの光出現前のUI演出を実装。  
UFO.cpp / .h  
UFOLightManager.cpp / .h  
UFOLightUI.cpp / .h  

・リスク & リワード設計  
牛救出失敗時のスコア減少や、コンボ時のバフシステムを実装。  
Score.cpp / .h  
Combo.cpp / .h  
AddTimer.cpp / .h  
Timer.cpp / .h  

・牛の餌数調整  
ゲームバランスの調整として、牛の餌の最大数を2個から3個へ変更。  
CowFood.cpp / .h  

</details>  

<details>  
<summary>演出 & UI</summary>  

## 演出 & UI  

・救出演出  
牛救出時に再生される牛のアニメーション演出及び音声を追加。  
DummyCow.cpp / .h  

・シーン遷移演出  
シーン遷移時に必ずFadeOut及びFadeInが行われる仕組みを実装。  
FadeManager.cpp / .h  
LoadingScene.cpp / .h  

・マップ外牛の誘導UI  
マップ外で牛がUFOに連れ去られている際、マップ内に方向を示す矢印を表示し、連れ去られている牛の位置を把握しやすくするUIを実装。  
Map.cpp / .h  

・操作説明UI  
ロード画面に操作説明の画像を表示する機能を実装。
LoadingScene.cpp / .h  

・制限時間警告UI  
制限時間が一定以下になった際、カウンターアニメーションを再生するように改善。
Timer.cpp / .h  

・チュートリアルUI  
難易度「はじめて」をチュートリアルとして、プレイヤーがゲームの操作やルールを理解できるようにUI及びゲーム進行を設計・実装。  
TutorialManager.cpp / .h  

・牛の餌UI・ゲームルール調整  
牛の餌の数を2個から3個へ変更し、ゲーム中の餌に関するUI及びゲームルールを調整。  
CowFood.cpp / .h  

</details>  

<details>  
<summary>エフェクト & エンジン拡張</summary>  

## エフェクト & エンジン拡張  

・救出失敗時エフェクト  
UFOによる牛の連れ去りに失敗した際に再生されるエフェクトを追加。  
EffectManager.cpp / .h  

・GIFアニメーション再生機能  
GIF画像をゲーム内で再生・表示するための仕組みを設計・実装。  
AnimSpriteRender.cpp / .h  

</details>  

### 替地謙介  

<details>  
<summary>ゲーム部分</summary>  

## ゲームルール & 難易度  

・難易度システム  
難易度選択画面を作成し、プレイヤーが任意の難易度を選択できるシステムを設計・実装。  
難易度ごとにノルマ、UFOの数、各種類の牛の出現率、制限時間、牛の最大出現数などのゲームパラメータをテーブル管理し、難易度に応じてゲーム内容が変化する仕組みを実装。  
Difficulty.cpp / .h  
DifficultySetting.cpp / .h  

・牛の行動・出現制御  
プレイヤーを追いかける牛や、通常の牛とは異なる報酬を持つボーナス牛を実装し、それぞれの出現率を設定。  
Cow.cpp / .h  
LoadingScene.cpp / .h  
Game.cpp / .h  

</details>  

<details>  
<summary>UI & オプション機能</summary>  

## UI & オプション機能  

・MapUIシステム  
マップ全体の表示・制御処理を実装。  
Map.cpp / .h  

・ポーズ・設定画面  
ポーズ画面、音量設定画面のUI及び制御処理を実装。  
Pause.cpp / .h  
SoundPause.cpp / .h  

・牛の餌UI  
牛の餌に関するモデル表示やUI表示を実装し、餌を設置することで牛が餌についてくる仕組みを実装。  
CowFood.cpp / .h  

・難易度選択UI  
難易度ごとの説明を表示する難易度選択画面を作成し、プレイヤーが任意の難易度を選択できるUIを実装。  
Difficulty.cpp / .h  
DifficultySetting.cpp / .h  

</details>  

<details>  
<summary>サウンド & ビジュアル</summary>  

## サウンド & ビジュアル  

・サウンド実装  
BGM、SEの追加及びゲーム内への組み込み。  
SoundManager.cpp / .h  

・ステージ制作  
ステージモデルの制作及び当たり判定、描画処理を担当。  
Stage.cpp / .h  

・エフェクト追加  
ダミー用の牛が生成された際にハートエフェクトを再生するように追加。  
DummyCow.cpp / .h  
EffectManager.cpp / .h  

・ゲーム内エフェクト拡張  
ゲーム内の各種状況に応じたエフェクトを追加。  
救出された牛のハートエフェクト、牛の餌を設置した際のエフェクト、餌の補充場所に近づいた際のAボタンエフェクト、牛舎付近のエリアエフェクト、牛が餌を食べた後の満腹エフェクト、プレイヤーが牛を引っ張る際の汗エフェクト、UFOに連れ去られている牛の汗エフェクト、牛が餌を食べ終わった後のエフェクトなどを実装。  
EffectManager.cpp / .h  

</details>  

<details>  
<summary>キャラクター制御 & アニメーション</summary>  

## キャラクター制御 & アニメーション  

・UFO挙動制御  
UFOの移動関連処理を実装。  
UFO.cpp / .h  

・ロープ演出モデル  
プレイヤーがロープを引く際の専用モデル制作及び表示処理を実装。  
Player.cpp / .h  

・モーションブレンド  
プレイヤーのアニメーションの補完処理（モーションブレンド）を実装。  
Player.cpp / .h  

・プレイヤーアニメーション更新  
プレイヤーのアニメーションを新規モデル・モーションへ更新し、ゲーム内へ追加実装。  
Player.cpp / .h  

・ロープ操作UI  
プレイヤーがロープを引っ張る際に表示されるUIを新調し、ゲーム内へ追加実装。  
Player.cpp / .h  

・牛の行動制御  
牛の休憩時間・移動時間をランダム化する疑似AI処理を実装。  
Cow.cpp / .h  

・牛の餌追従システム  
牛が設置された餌を追いかける処理を実装。  
CowLuring.cpp / .h  

・餌の消費演出  
牛が餌を食べた際に餌が縮む処理、及び餌を食べた際のエフェクト再生を設計・実装。  
CowShrinkHay.cpp / .h  

</details>  

<details>  
<summary>オブジェクト & システム管理</summary>  

## オブジェクト & システム管理  

・牛の餌管理システム  
牛の餌の生成・消去処理を実装し、ゲーム内に存在する餌の情報を管理するシステムを設計・実装。  
CowFoodManager.cpp / .h  

・牛の餌システム  
牛の餌のモデル・UI表示、餌の設置、牛の追従、餌の消費など、牛の餌に関する一連のシステムを実装。  
CowFood.cpp / .h  
CowLuring.cpp / .h  
CowShrinkHay.cpp / .h  
CowFoodManager.cpp / .h  

</details>  

# 操作説明  
><img width="500" height="300" alt="画像" src="https://github.com/user-attachments/assets/bfdadabf-2e52-4d08-821c-c87f26a5199c" />

[↑目次に戻る](#toc)  

# ゲーム説明  

### ◇ゲーム詳細
>このゲームは、カウボーイ風のヒーローが、牧場でUFOが連れ去ろうとする牛をロープで助けるアクションゲームです。

>時間制限内に、クリアに必要な数以上牛を助けることが目的です。  

>難易度が存在しており、それぞれ「はじめて」「かんたん」「ふつう」「むずかしい」の4つが存在します。

>UFOは難易度ごとに0～4機存在しており、光を出しているときは近くにいる牛を追跡し、一定距離内にいる牛を連れ去ろうとしてきます。

>プレイヤーは連れ去らわれている牛に対してRBボタンでロープを投げることができます。  
ロープが一定の距離近づくと牛にロープが付きます。  
ロープが付いた状態でRT,LTボタン連打をすることによりロープを引っ張り、一定の距離まで近づけることで牛を救出できます。

>時間制限が0になる、または牛が3匹UFOに連れ去られるとゲームは終了し、リザルト画面(ゲームオーバー、ゲームクリア)に移ります。
>救出数がノルマに達していない場合はゲームオーバー、達成していればゲームクリアとなります。  

><img width="500" height="300" alt="矢印" src="https://github.com/user-attachments/assets/f2388424-5b58-424d-8233-2c68581e3b38" />  

>牛を助けるとコンボ数×100のスコアが加算されます。  

><img width="500" height="300" alt="Score" src="https://github.com/user-attachments/assets/bacd9bc7-a31e-4b2e-9d8e-f8e2a00e7188" />  

>コンボ中は1コンボごとにゲーム時間が+3秒されます。  
><img width="500" height="300" alt="AddTimer" src="https://github.com/user-attachments/assets/84bc2bbf-6631-4c25-ba14-bff131e8b2db" />　　


[↑目次に戻る](#toc) 

# ◇UIについて

### ①ゲーム中のUIについて
>ゲーム中に使うUIは統一感、見栄えの良さなどを考え、基本的に同じ白基調のUIにしています。  
><img width="500" height="300" alt="GameUI" src="https://github.com/user-attachments/assets/9c3b3ef8-e764-4b1e-aef6-e2093a2ca6cb" />

[↑目次に戻る](#toc) 

### ②ゲームスタート時のUI表現について
>ゲームスタート時のUIに関してはこのようにfmodf関数を使い、カウントダウンからの残りの時間を1で割った余りを取得し、それをα値やスケールの数値の変更に使うことによって、UIが透明で小さい状態から不透明で大きい状態になりを繰り返しながらカウントダウンのUIが表示されているようになっております。  
><img width="500" height="300" alt="カウントダウン" src="https://github.com/user-attachments/assets/ba663c4a-b305-4f7e-9a7a-d78e286193ca" />　　

```c++
m_countDownTime -=
g_gameTime->GetFrameDeltaTime();

/** 最初は全てOFF */  m_Show1 = false;

  m_Show2 = false;
  m_Show3 = false;
  m_ShowStart = false;

if (m_countDownTime > 3.0f)
{
    m_isCountDown = true;
    m_Show3 = true;
}

else if (m_countDownTime > 2.0f)
{
    m_isCountDown = true;
    m_Show2 = true;
}

else if (m_countDownTime > 1.0f)
{
   m_isCountDown = true;
   m_Show1 = true;
}

else if (m_countDownTime > 0.0f)
{
   m_ShowStart = true;
}

/** カウントダウン終了 */  
else  
{
    m_isCountDown = false;
}
  

/** カウントダウンのフェードイン処理 */
/** カウントダウンの残り時間を1秒で割った余りを取得して、0から1の範囲にする */
float localTime = fmodf(m_countDownTime, 1.0f);


/** カウントダウンの残り時間が0から1の範囲にあるときだけフェードイン処理を行う */
/** α値は1からフェードインの進行度に応じて0に近づいていく */
m_countDownAlpha =
 1.0f - ( localTime / FADE_TIME);


/** カウントダウンのスケール処理 */
/** この式はスケールの初期スケール + フェードインの進行度に応じたスケールの変化量 * α値 */  m_countDownScale =
0.5f + (localTime / FADE_TIME) * m_countDownAlpha;

m_countDown1.SetScale(Vector3(m_countDownScale, m_countDownScale, 1.0f));

m_countDown2.SetScale(Vector3(m_countDownScale, m_countDownScale, 1.0f));

m_countDown3.SetScale(Vector3(m_countDownScale, m_countDownScale, 1.0f));

m_countDownStart.SetScale(Vector3(m_countDownScale, m_countDownScale, 1.0f));

m_countDown1.Update();
m_countDown2.Update();
m_countDown3.Update();
m_countDownStart.Update();
```
[↑目次に戻る](#toc) 

### ③スコアのUIについて
>スコアのUIでは、この様にカウンターアニメーションを入れ、救出数UIではバウンドアニメーションを入れることにより、数字がリアルタイムで増えていく面白さを表現しました。  
><img width="500" height="300" alt="Score" src="https://github.com/user-attachments/assets/434b6890-1f04-4e68-a2c3-39df4bbf1e1f" />  

## CowNumberOfRescues.cpp
```c++
 /** 救出数が変化した場合 */
 if (m_oldNumberOfRescues != m_numberOfRescues)
 {
     /** バウンドの値を設定 */
     m_bounceValue.y = 30.0f;

     /** 古い救出数を更新 */
     m_oldNumberOfRescues = m_numberOfRescues;

     /** 数字がバウンドしたフラグを立てる */
     m_numberBound = true;
 }

 /** 数字がバウンドしている場合 */
 if (m_numberBound)
 {
     m_bounceValue.Lerp(0.25f, m_bounceValue, Vector3::Zero);
     /** 救出数のUIの座標にバウンドの値を加算 */
     for (int i = 0; i < LayoutTypeNum; i++)
     {
         m_tensPos[i].y += m_bounceValue.y;
         m_onesPos[i].y += m_bounceValue.y;
     }
     /** バウンドの値が0.1以下になったらバウンド終了 */
     if (m_bounceValue.y <= 0.1f)
     {
         m_bounceValue = Vector3::Zero;
         m_numberBound = false;
     }
 }
```  

## Score.cpp

```c++
/** Lerp関数で滑らかに値を近づける */
/** 表示するスコアを徐々にスコアに近づける */
/** 型変換をすることで、整数の計算を行う */m_displayScore = m_displayScore + (int)((m_score - m_displayScore) * LERP_SPEED);

/** 絶対値が5未満ならスコアを到達させる */
if (abs(m_score - m_displayScore) < 5)
{
  m_displayScore = m_score;
}
```
[↑目次に戻る](#toc) 

### ④アウトゲーム内のUIについて
>アウトゲーム内で、ボタンを押すとPRESS ANY BUTTONのUIが点滅するようにしました。  
ボタンを押したことを分かりやすく表現するためです。
>
><img width="500" height="300" alt="タイトル点滅" src="https://github.com/user-attachments/assets/7d11b80a-cb18-4250-a3eb-06333acee737" />  

[↑目次に戻る](#toc) 

### ⑤コンボした際に表示されるUIについて
>コンボをした際に秒数が+3されてるUIが画面上部に表示されたり、画面右下にComboのUIが表示されたりするように追加しました。  
今何コンボしていて、秒数がどれくらい変化したかを視覚で分かるようにすることによって、プレイヤーが次にどの牛を救出すればいいかを判断できるようにするためです。
><img width="500" height="300" alt="AddTimer" src="https://github.com/user-attachments/assets/fe6c0351-e362-4064-a21d-090722da9058" />  
><img width="500" height="300" alt="Score" src="https://github.com/user-attachments/assets/225484b0-06ae-4bef-aaea-708b87da3926" />  

[↑目次に戻る](#toc)  

### ⑥餌UIについて  
>牛をおびき寄せる際に使う餌のUIは、餌を使うごとにUIが一つずつ灰色になっていき、残りの所持している餌の数を分かりやすく表現しています。  
>また、牛舎の救出範囲内に餌を置き、何もしなくてもゲームがクリアできないように、牛舎の救出範囲内では餌が使えないようなアイコンになります。
>例：餌を一つ使った状態で救出範囲内に入った時の画像  
><img width="500" height="300" alt="TabooCowFood1" src="https://github.com/user-attachments/assets/284e5041-2332-4da8-a469-1968527803be" />

[↑目次に戻る](#toc)  

### ⑦牛救出失敗の許容数  
>牛の救出に3回失敗すると強制的にゲームオーバーになるようにしました。  
>牛の救出に失敗すると左上の牛のアイコンが一つずつ灰色になっていきます。  
>2回失敗すると、牛のアイコンが赤く点滅します。  
>3回失敗すると「救助失敗...」というUIが出た後に、ゲームオーバー画面に遷移します。  
>これによって連れ去られている牛を助けなければいけないという緊張感を表現できました。
><img width="500" height="300" alt="CowLivesUI" src="https://github.com/user-attachments/assets/e6a31ad8-b3bd-4139-a748-43274baefba1" />  

[↑目次に戻る](#toc)  


# ミニマップについて  

>ミニマップは「牛」「UFO」「プレイヤー」のアイコンが出ます。  
牛がUFOに連れ去られていると、危険のマークになり、現在連れ去られている牛の場所を分かりやすくしています。  
アイコンはミニマップの範囲外に行くと、非表示になります。  
><img width="500" height="300" alt="MapIcon3" src="https://github.com/user-attachments/assets/8bc0896e-6ae5-4080-b66c-b134646ec02b" />  

ミニマップで表示する範囲を「円形の範囲」として定義し、  
カメラやプレイヤーの位置に基づき、座標を変換します。  
また、ワールド座標系をミニマップ上のスクリーン座標系に変換します。

[↑目次に戻る](#toc)   

# 工夫した点

### ①異なる種類の牛の実装及び挙動について  
>4種類の牛を実装いたしました。  
>種類が分かりやすいように種類ごとに色を分けています。  
>種類は次の通りです。  
・ランダム移動を行う牛(茶色)  
・通常時はランダム移動だが、UFOの光が出ている時のみ光の方向に向かって移動を行う牛(薄茶色)  
・常時プレイヤーのいる方向について行く牛(ピンク色)  
・救助に時間がかかるが、救助した際に高得点が得られるボーナス牛(黄色)  
>これらの牛の生成に関してはLoadingScene.cpp内、及びGame.cpp内で行っています。  
>これらの牛を追加したことにより、ステージ全体に満遍なく牛を配置できるような状態を作り、常にプレイヤーが考えて動かないと助けられないような環境にしました。  
>また、この牛達は難易度ごとに出現率が異なるようになっているので、プレイヤーの腕前に合ったプレイを実現させています。  

### LoadingScene.cpp  
```c++  
/** チュートリアル中はLoadingクラスの牛を生成しない */
if (!m_isTutorial)
{
	/** 0～9の数字を割り当てる*/
	int cowIndex = m_loadStep - 2;

	/** 難易度ごとの最大出現数を取得し、それ未満の時だけ生成する */
	int maxCowCount = GameDifficultyManager::GetParam().maxCowCount;

	if (cowIndex < maxCowCount)
	{
		Cow* cow = NewGO<Cow>(0, "cow");
		cow->SetPosition(RandomCowPos());

		/** 難易度が簡単なら追いかける牛は生成しない */
		bool isEasy = (GameDifficultyManager::GetDifficulty() == EnDifficulty::en_Easy);

		if (!isEasy && rand() % 100 < INITIAL_CHASE_COW_RATE)
		{
			cow->SetCowType(Cow::EnCowType::en_Chase);
		}
		else
		{
			cow->SetCowType(cowIndex % 2 == 0 ? Cow::EnCowType::en_Light : Cow::EnCowType::en_Random);
		}

		m_tempCows.push_back(cow);
	}
}
```

### Game.cpp
```c++ 
void Game::SpawnCow()
{
	/** チュートリアル中はランダムスポーンさせない*/
	if (m_isTutorialMode)
	{
		return;
	}

	if (m_timer->GetTimer() <= 4.0f || m_isTimeOut)
	{
		/** タイマーが4秒以下なら牛を補充しない */
		return;
	}

	/** 現在の牛の数が10体未満なら補充 */
	const int maxCowCount = GameDifficultyManager::GetParam().maxCowCount;
	if (static_cast<int>(m_aliveCows.size()) < maxCowCount)
	{

		m_spawnTimer += g_gameTime->GetFrameDeltaTime();

		/** 3秒ごとに1体補充 */
		if (m_spawnTimer >=NEW_SPAWN_TIMER)
		{
			
			int currentrescues = m_cowNumberOfRescues->GetNumberOfRescues();

			/** もし牛の救出数が一定以上ならスポーン範囲を大きくする。 */
			int chaseCowRate = CHASE_COW_TABLE_A;

			/** もし牛の救出数が一定以上ならスポーンする範囲を大きくする */
			if (currentrescues >= 10)
			{
				m_difficultyLevelSpawnRange = 600;
				chaseCowRate = CHASE_COW_TABLE_C;
			}

			else if (currentrescues >= 5)
			{
				m_difficultyLevelSpawnRange = 400;
				chaseCowRate = CHASE_COW_TABLE_B;
			}

			else
			{
				m_difficultyLevelSpawnRange = 0;
				chaseCowRate = CHASE_COW_TABLE_A;
			}

			/** 簡単モードなら追いかける牛の確率を強制的に0にする */
			if (GameDifficultyManager::GetDifficulty() == EnDifficulty::en_Easy)
			{
				chaseCowRate = 0;
			}
			else
			{
				chaseCowRate += GameDifficultyManager::GetParam().chaseCowRateOffset;
			}

			chaseCowRate = max(0, min(chaseCowRate, 100));

			m_spawnTimer = 0.0f;


			/** 新しい牛を生成 */
			Cow* newCow = NewGO<Cow>(0, "cow");

			
			/** スポーン位置 */
			Vector3 pos;

			/** ランダムスポーン範囲に難易度による調整を加える */
			int range = RANDOM_SPAWN_RANGE + m_difficultyLevelSpawnRange;

			/** ランダムな位置を生成する */
			/** 難易度(牛の救出数)に応じてスポーン範囲を調整 */
			pos.x = (rand() % (range * 2)) - range;
			pos.y = 0.0f;
			pos.z = (rand() % (range * 2)) - range;

			/** 難易度に応じたパラメーターを取得 */
			const DifficultyParam difficultyParam = GameDifficultyManager::GetParam();

			/** 難易度に応じて牛のタイプを決定する */
			newCow->SetCowType(DecideCowType(chaseCowRate,difficultyParam.bonusCowRate,difficultyParam.UFOCowRate));
			/** UFOに向かって歩く牛の速度も適応させる*/
			newCow->IsMoving();

			newCow->SetPosition(pos);
			/** 生きている牛リストに追加 */
			m_aliveCows.push_back(newCow);


		}
	}
}
```  

[↑目次に戻る](#toc)  

### ②UFOが牛を見つける挙動について  
UFOは自身に一番近い牛を標的とし、追いかけることで、より敵らしい動きを再現しました。  

```c++  
auto cow = FindGOs<Cow>("cow");

/** 最も近い牛 */
Cow* nearestCow = nullptr;

/** 最も近い牛を見つけるために
 * FLT_MAXを利用して一度最大数にしておく */
float nearestDistSq = FLT_MAX;

/** 牛を一匹ずつ見る */
for (auto c : cow)
{
	/** すでに他のUFOが狙っている牛は無視 */
	if (c->GetTakingUFO() != nullptr && c->GetTakingUFO() != this)
	{
		continue;
	}

	/** 連れ去られている牛も無視 */
	if (c->GetIsTakeAwayed())
	{
		continue;
	}

	/** UFOと牛の距離を計算 */
	Vector3 diff = c->GetPosition() - m_transform.GetPosition();
	
	/** y軸は考慮しない */
	diff.y = 0.0f;

	float distSq = diff.LengthSq();

	if (distSq < nearestDistSq)
	{
		nearestDistSq = distSq;
		nearestCow = c;
	}
}

/** 近くに牛が見つかっているかどうか */
if (nearestCow)
{
	/** 追尾開始した瞬間に TakingUFO をセットする */
	if (nearestCow->GetTakingUFO() == nullptr)
	{
		nearestCow->SetTakingUFO(this);
	}

	/**  追尾中だったらtrue */
	m_isChasing = true;

	/** 牛の方向を作る */
	Vector3 dir = nearestCow->GetPosition() - m_transform.GetPosition();
	dir.Normalize();

	/** dirの方向に進むようにセットする */
	m_moveDir = dir;


	/** もし牛を牛を見つけるときの距離のほうが大きかったら */
	if (sqrtf(nearestDistSq) < CAPTURE_RANGE)
	{
		/** この牛を捕まえる(true) */
		m_isCowTakeAwayed = true;
		m_targetCow = nearestCow;

		/** いま追っている牛をtrueにして他のUFOは追尾しないようにする */
		m_targetCow->SetIsTakeAwayed(true);
	}
}

/** それいがいは追尾しない */
else
{
	m_isChasing = false;
}

```  

[↑目次に戻る](#toc)  

### ③gif画像の実装及び再生について  
>gif画像が再生及び表示されるように、設計、実装を行いました。
>AnimSpriteRenderというクラスをK2Engine内で作り、その中で常に画像を再生するような処理や、表示を実装しました。  
>再生に関しましては、1フレームごとの静止画を連続で何枚も見せることにより、gif画像の様な滑らかな再生を実現しています。  

### AnimSpriteRender.h  
```c++  
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

```  

### AnimSpriteRender.cpp  
```c++  
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
```  

### LoadingScene.cpp
```c++  
void LoadingScene::PrepareAnimResources()
{
	/** 既に準備されている場合は何もしない */
	if (!m_foodAnimLoadPaths.empty())
	{
		return;
	}

	/** ロープで引っ張るGifのパスリストを準備する */
	for (int h = 10; h <= 93; h += 2)
	{
		char buf[256];
		sprintf(buf, "Assets/Gif/PullRope/anim_%02d.DDS", h);
		m_ropeAnimLoadPaths.push_back(buf);
	}
	/** 餌のGifのパスリストを準備する */
	for (int i = 1; i <=74 ; i ++)
	{
		char buf[256];
		sprintf(buf, "Assets/Gif/Food/anim_%02d.DDS", i);
		m_foodAnimLoadPaths.push_back(buf);
	}

	/** 歩く牛のローディングアニメーションを常にループ再生する*/
	for (int i = 1; i <= COWWALK_FREAM_COUNT; i++)
	{
		char buf[256];
		sprintf(buf, COWWALK_FILEPATH, i);
		m_CowWalkAnimLoadPaths.push_back(buf);
	}

	/** ロープで引っ張るGifのフレーム数を準備する */
	m_ropeAnimSpriteRender.PrepareFrameCount(
		static_cast<int>(m_ropeAnimLoadPaths.size()), 610.0f, 590.0f, 10.0f);
	m_ropeAnimSpriteRender.SetPosition({ -535.0f, 10.0f, 0.0f });

	/** 餌のGifのフレーム数を準備する */
	m_foodAnimSpriteRender.PrepareFrameCount(
		static_cast<int>(m_foodAnimLoadPaths.size()), 610.0f, 590.0f, 10.0f);
	m_foodAnimSpriteRender.SetPosition({ -535.0f, 10.0f, 0.0f });

	/** 25枚程度なので一括ロードする*/
	m_cowWalkSpriteRender.Init(
		m_CowWalkAnimLoadPaths,
		static_cast<int>(m_CowWalkAnimLoadPaths.size()),
		COWWALK_WIDTH, COWWALK_HEIGHT, COWWALK_FPS);
	m_cowWalkSpriteRender.SetPosition(Vector3(800.0f, -400.0f, 0.0f));
}
```  
[↑目次に戻る](#toc)  

### ④ロード中にオブジェクトが生成される処理  
>ロード中に重たいオブジェクトをSwitch文で制御し、生成することにより、ゲームが開始した際のカクつきを改善し、ゲームを始めるのが早くなりました。  
>これにより、ゲーム中のFPSが7～10FPS程改善されました。

```c++  
void LoadingScene::LoadGameObjectsStepByStep()
{
	switch (m_loadStep)
	{
		/** ロードするゲームオブジェクトをステップバイステップで生成する */
	case 0:
	{


		/** 1フレームでロードに使ってよい時間(ms) */
		constexpr double ROPE_LOAD_TIME_BUDGET_MS = 2.0;

		/** ロープを最優先でロードし切るまではこのステップに留まる*/
		if (m_ropeLoadIndex < static_cast<int>(m_ropeAnimLoadPaths.size()))
		{
			auto loadStart = std::chrono::high_resolution_clock::now();

			/** 予算内に収まる限り、1枚ずつロードを続ける */
			while (m_ropeLoadIndex < static_cast<int>(m_ropeAnimLoadPaths.size()))
			{
				m_ropeAnimSpriteRender.AddFrame(m_ropeAnimLoadPaths[m_ropeLoadIndex]);
				m_ropeLoadIndex++;

				double elapsedMs = std::chrono::duration<double, std::milli>(
					std::chrono::high_resolution_clock::now() - loadStart).count();

				if (elapsedMs >= ROPE_LOAD_TIME_BUDGET_MS)
				{
					break;
				}
			}
			return; // ロープのロードが終わるまで次のステップに進まない
		}

		// ロープのロードが完了した時点でこのcaseを抜ける
		break;
	}
	/** プレイヤーを生成 */
	case 1:NewGO<Player>(0, "player");
		break;

	case 2:/** ステージを生成 */
	{
		if (m_stage == nullptr)
		{
			m_stage = NewGO<Stage>(0, "stage");
		}
		if (!m_stage->LoadStepByStep())
		{
			/** ステージのロードが終わるまで次のステップに進まない*/
			return;
		}
		break;
	}
	///** 牛を生成(10体分) */
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	{
		/** チュートリアル中はLoadingクラスの牛を生成しない */
		if (!m_isTutorial)
		{
			/** 0～9の数字を割り当てる*/
			int cowIndex = m_loadStep - 2;

			/** 難易度ごとの最大出現数を取得し、それ未満の時だけ生成する */
			int maxCowCount = GameDifficultyManager::GetParam().maxCowCount;

			if (cowIndex < maxCowCount)
			{
				Cow* cow = NewGO<Cow>(0, "cow");
				cow->SetPosition(RandomCowPos());

				/** 難易度が簡単なら追いかける牛は生成しない */
				bool isEasy = (GameDifficultyManager::GetDifficulty() == EnDifficulty::en_Easy);

				if (!isEasy && rand() % 100 < INITIAL_CHASE_COW_RATE)
				{
					cow->SetCowType(Cow::EnCowType::en_Chase);
				}
				else
				{
					cow->SetCowType(cowIndex % 2 == 0 ? Cow::EnCowType::en_Light : Cow::EnCowType::en_Random);
				}

				m_tempCows.push_back(cow);
			}
		}

		
	} break;

	/** もしUFOが消えていなかったら残っているUFOを消す */
	case 13:
	{
		auto ufos = FindGOs<UFO>("UFO");
		for (auto ufo : ufos)
		{
			if (ufo && !ufo->IsDead())
			{
				DeleteGO(ufo);
			}
		}
		break;
	}

	/** UFOLightManagerを生成 */
	case 14:
		NewGO<UFOLightManager>(0, "ufolightmanager");
		break;

		/** UFOを生成(4体分) */
	case 15:
	case 16:
	case 17:
	case 18:
	{
		/** チュートリアル中はLoadingクラスのUFOを生成しない */
		if (!m_isTutorial)
		{
			int index = m_loadStep - 15;

			/** 難易度に応じたUFO数を取得し、その数までしか生成しないようにする */
			int ufoCount = GameDifficultyManager::GetParam().ufoCount;

			if (index >= 0 && index < 4 && index < ufoCount)
			{
				UFO* ufo = NewGO<UFO>(0, "UFO");
				ufo->SetPosition(UFO_INFOMATIONS[index].pos);
				ufo->SetUFOMoveState();
				ufo->SetSlotIndex(index);
				m_tempUFOs.push_back(ufo);
			}
		}
	} break;

	/** ゲームカメラを生成 */

	case 19: NewGO<GameCamera>(0, "gameCamera");
		break;

	case 20: NewGO<UIPanels>(0, "uipanels");
		break;

	case 21:
	{
		/** 牛の餌を生成 */
		NewGO<CowFood>(0, "cowfood");

		NewGO<CowFoodManager>(0, "cowfoodmanager");
	}
	break;
	/** スカイキューブを生成 */
	case 22:
	{
		/** SkyCube を生成 */
		m_skyCube = NewGO<SkyCube>(0, "skyCube");

		/** タイプ設定 */
		m_skyCube->SetType(EnSkyCubeType::enSkyCubeType_Day);

		/** スケール設定 */
		m_skyCube->SetScale(10000.0f);
	}
	break;

	/** 方向光・IBL設定（ここが一番重いはず） */
	case 23:
	{
		/** 方向光(ほぼ真上から差し込む光) */
		Vector3 sunDir(0.0f, -1.0f, 0.0f);
		sunDir.Normalize();
		g_renderingEngine->SetDirectionLight(0, sunDir, Vector3(5.0f, 5.0f, 5.0f));

		/** IBL 設定 */
		g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.95f);
	}
	break;
	case 24:
		/** ブルームを抑制 */
		g_renderingEngine->SetBloomThreshold(3.0f);
		break;

	case 25:
	{
		if (m_game == nullptr)
		{
			m_game = NewGO<Game>(0, "game");
		}
		if (!m_game->LoadStepByStep())
		{
			/** Gameの初期化が終わるまで次に進まない */
			return;
		}
		for (auto cow : m_tempCows)
		{
			m_game->GetAliveCows().push_back(cow);
		}

		m_game->SetUFOList(m_tempUFOs);
	
		/** ロード完了フラグを立てる */
		m_isLoadingEnd = true;
		return;
	}
	}
	m_loadStep++;
}

```  

[↑目次に戻る](#toc)  

### ⑤チュートリアルの実装について  
>難易度の実装に伴い、チュートリアルも実装しました。  
>難易度「はじめて」がチュートリアルになっています。  
>チュートリアルでは  
・ロープの投げ方  
・ロープでの牛の救出  
・餌を置く方法及び餌の補充方法  
・牛を牛舎までの誘導  
以上の五つの操作をプレイヤーに行わせ、ゲームの基本操作を学習させるようにしています。  
>これにより、初めてのプレイヤーが操作に困ることが無いようになります。

[↑目次に戻る](#toc)  

### ⑥難易度の実装  

>「はじめて」「かんたん」「ふつう」「むずかしい」  
>以上の4つの難易度を追加いたしました。  
>難易度が変わると  
・救出数のノルマ  
・UFOの数  
・各牛ごとの出現率  
・制限時間  
・牛の最大出現数  
の5種類のゲーム要素が変化します。  
>これにより、初心者から上級者までのプレイヤーが自分に合った難易度を選択し、プレイできるようになりました。  

### DifficultySetting.h  
```c++  
#pragma once
namespace
{
	constexpr int DIFFICULTY_GRID_ROWS = 2;
	constexpr int DIFFICULTY_GRID_COLS = 2;
}
enum class EnDifficulty
{
	en_Tutorial,
	en_Easy,
	en_Normal,
	en_Hard,
	en_DifficultyNum
};

struct DifficultyParam
{
	/** クリア条件となる牛の救出ノルマ数 */
	int normaCount;

	/** 同時に出現させるUFOの数 */
	int ufoCount;

	/** 追いかけてくる牛の出現確率 */
	int chaseCowRateOffset;

	/** 制限時間 */
	int timeLimit;

	/** 牛の最大数 */
	int maxCowCount;

	/** ボーナス牛の出現率 */
	int bonusCowRate;

	/** UFOに向かって走る牛の出現確率 */
	int UFOCowRate;
};

namespace GameDifficultyManager
{
	/** 難易度を設定する */
	void SetDifficulty(EnDifficulty difficulty);

		/** 現在設定されている難易度を取得する */
		EnDifficulty GetDifficulty();

	/** 現在の難易度に対応するパラメーターの取得する */
		const DifficultyParam GetParam();

}

class DifficultySetting : public  IGameObject
{

};


```  

### DifficultySetting.cpp  
```c++  
#include "stdafx.h"
#include "DifficultySetting.h"

namespace
{
	/** 難易度ごとのパラメーターテーブル */
	const DifficultyParam DIFFICULTY_PARAM_TABLE[static_cast<int>(EnDifficulty::en_DifficultyNum)] =
	{
		/** ノルマ 　UFO少なめ　追いかける牛の出現確率　 制限時間　牛最大出現数 ボーナス牛の出現確率  UFOに向かって歩く牛の出現確率 */

		/** ※追いかける牛がマイナス数値の理由は出現確率の定義ですでに20％未満なためそこから引き算方式で出現確率を調整しています。 */

		/** チュートリアル用のため未使用 */
		{ 0, 0,    0 ,  0, 0, 0, 0},
		/** 簡単 */
		{ 5, 2 , 0 ,120, 6, 0, 50},

		/** 普通 */
		{10, 3,   -10, 110, 8, 4,40},

		/** 難しい */
		{20, 4,  -5 ,100,10, 6,35},
	};

	EnDifficulty g_currentDifficulty = EnDifficulty::en_Easy;
}

namespace GameDifficultyManager
{
	/** 難易度の設定 */
	void SetDifficulty(EnDifficulty difficulty)
	{
		g_currentDifficulty = difficulty;
	}

	/** 現在設定している難易度の取得 */
	EnDifficulty GetDifficulty()
	{
		return g_currentDifficulty;
	}

	/** 現在の難易度のパラメーターを設定する */
	const DifficultyParam GetParam()
	{
		return DIFFICULTY_PARAM_TABLE[static_cast<int>(g_currentDifficulty)];

	}
}
```

### LoadingScene.cpp内  
```c++  
/** チュートリアル中はLoadingクラスのUFOを生成しない */
if (!m_isTutorial)
{
	int index = m_loadStep - 15;

	/** 難易度に応じたUFO数を取得し、その数までしか生成しないようにする */
	int ufoCount = GameDifficultyManager::GetParam().ufoCount;

	if (index >= 0 && index < 4 && index < ufoCount)
	{
		UFO* ufo = NewGO<UFO>(0, "UFO");
		ufo->SetPosition(UFO_INFOMATIONS[index].pos);
		ufo->SetUFOMoveState();
		ufo->SetSlotIndex(index);
		m_tempUFOs.push_back(ufo);
	}
}
```  

[↑目次に戻る](#toc)  
