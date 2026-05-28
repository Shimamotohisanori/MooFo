# MooFo
<img width="1919" height="1057" alt="Title" src="https://github.com/user-attachments/assets/f5a60dea-35e9-4b7d-b185-9df67ec950cd" />  

カッパ杯サマーの展示作品です
### 河原電子ビジネス専門学校　ゲームクリエイター科(28卒)

### 氏名 : 島本学典
### 氏名 : 仙波知裕
### 氏名 : 替地謙介

GithubのURL
https://github.com/Shimamotohisanori/MooFo


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
　2026年2月～2026年5月  
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
プレイヤー接近時の牛の逃走、UFOにより連れ去り処理、及び基盤となるActor / Character クラスの設計。  
Cow.cpp / .h  
UFO.cpp / .h  
Actor.cpp / .h  
Character.cpp / .h  

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

## エンジン拡張 & システム基盤  
※エンジン部分での変更は改造のみとなっております。

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
CowCaputureController.cpp / .h  

・エンジンの改造  
ライトの明るさ設定の変更、およびエフェクトを扱いやすくするためのエンジン改造。  
SceneLight.cpp / .h  
EffectEmitter.cpp / .h  

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
ゲーム開始時のカウントダウン処理を実装  
CountDown.cpp / .h  

## オブジェクト配置 & ゲームバランス  
・ランダムスポーン制御  
現在のスポーン範囲を基準として、牛が毎回ランダムな位置に配置される処理を実装。  
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

## 演出 & UI  
・救出演出  
牛救出時に再生され牛アニメーション演出及び音声の追加  
DummyCow.cpp / .h  

</details>  

### 替地謙介  
<details>
<summary>ゲーム部分</summary>  

## UI & オプション機能  
・MapUIシステム  
マップ全体の表示・制御処理を実装。  
Map.cpp / .h  

・ポーズ・設定画面  
ポーズ画面、音量設定画面のUI及び制御処理を実装。  
Pause.cpp / .h  
SoundPause.cpp / .h  

## サウンド & ビジュアル  
・サウンド実装  
BGM,SEの追加及びゲーム内への組み込み。  
SoundManager.cpp / .h  

・ステージ制作  
ステージモデルの制作及び描画処理を担当。  
Stage.cpp / .h  

・エフェクト追加  
ダミー用の牛が生成された際にハートエフェクトを再生するように追加。  
DummyCow.cpp / .h  
EffectManager.cpp / .h  

## キャラクター制御 & アニメーション  
・UFO挙動制御  
UFOの移動関連処理を実装。  
UFO.cpp / .h  

・ロープ演出モデル  
プレイヤーがロープを引く際の専用モデル制作及び表示処理を実装。  
Player.cpp / .h  

・モーションブレンド  
プレイヤーのアニメーションの補完処理(モーションブレンド)を実装。  
Player.cpp / .h  

・牛AI行動制御  
牛の休憩時間・移動時間をランダム化する疑似AI処理を実装。  
Cow.cpp / .h

</details>  

# 操作説明
[↑目次に戻る](#toc)  

# ゲーム説明  

### ◇ゲーム詳細
>このゲームは、カウボーイ風のヒーローが、牧場でUFOが連れ去ろうとする牛をロープで助けるアクションゲームです。

>時間制限内に、クリアに必要な数以上牛を助けることが目的です。

>UFOは4機存在しており、光を出しているときは近くにいる牛を追跡し、一定距離内にいる牛を連れ去ろうとしてきます。

>プレイヤーは連れ去らわれている牛に対してRBボタンでロープを投げることができます。  
ロープが一定の距離近づくと牛にロープが付きます。  
ロープが付いた状態でRT,LTボタン連打をすることによりロープを引っ張り、一定の距離まで近づけることで牛を救出できます。

>牛を助けるとスコアが+100され、  
5コンボごとにスコアが+200されます。  

><img width="1919" height="1057" alt="Score" src="https://github.com/user-attachments/assets/08cc5c56-7780-45b2-8795-f1a2103b5173" />  

>コンボ中は1コンボごとにゲーム時間が+3秒されます。  
><img width="1919" height="1057" alt="AddTimer" src="https://github.com/user-attachments/assets/ecabf35b-3553-4765-bef4-d2ecd3604c16" />


[↑目次に戻る](#toc) 

# ◇UIについて

### ①ゲーム中のUIについて
>ゲーム中に使うUIは統一感、見栄えの良さなどを考え、基本的に同じ青色基調のUIにしています。  
><img width="1919" height="1057" alt="gameUI" src="https://github.com/user-attachments/assets/c00f9d88-f831-4623-86a7-9c7a7cc0a22f" />

[↑目次に戻る](#toc) 

### ②ゲームスタート時のUI表現について
>ゲームスタート時のUIに関してはこのようにfmodf関数を使い、カウントダウンからの残りの時間を1で割った余りを取得し、それをα値やスケールの数値の変更に使うことによって、UIが透明で小さい状態から不透明で大きい状態になりを繰り返しながらカウントダウンのUIが表示されているようになっております。  
><img width="1919" height="1057" alt="カウントダウン" src="https://github.com/user-attachments/assets/d8cd7d5f-d46b-4f02-89f6-9b377b33bb7f" />

```
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
>スコアのUIでは、この様にカウンターアニメーションを入れることにより、数字がリアルタイムで増えていく面白さを表現しました。

```
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

### ④アウトゲーム内のUiについて
>アウトゲーム内で、ボタンを押すとPRESS ANY BUTTONのUIが点滅するようにしました。  
ボタンを押したことを分かりやすく表現するためです。
>
><img width="1919" height="1057" alt="タイトル点滅" src="https://github.com/user-attachments/assets/7d11b80a-cb18-4250-a3eb-06333acee737" />

[↑目次に戻る](#toc) 

### ⑤コンボした際に表示されるUIについて
>コンボをした際に秒数が+3されてるUIが画面上部に表示されたり、画面右下にComboのUIが表示されたりするように追加しました。  
今何コンボしていて、秒数がどれくらい増えたかを視覚で分かるようにすることによって、プレイヤーが次にどの牛を救出すればいいかを判断できるようにするためです。
><img width="1919" height="1057" alt="AddTimer" src="https://github.com/user-attachments/assets/741adadc-e3ea-4197-bad0-e08ebc780755" />  
><img width="1919" height="1057" alt="Score" src="https://github.com/user-attachments/assets/64a987b1-b6ec-480a-801a-7e050d9704cc" />


[↑目次に戻る](#toc)  

# ミニマップについて  

>ミニマップは「牛」「UFO」「プレイヤー」のアイコンが出ます。  
牛がUFOに連れ去られていると、危険のマークになり、現在連れ去られている牛の場所を分かりやすくしています。  
アイコンはミニマップの範囲外に行くと、非表示になります。
><img width="1919" height="1057" alt="MapIcon" src="https://github.com/user-attachments/assets/9ecaab46-c3fd-428a-adda-699ff93e27ee" />  

ミニマップで表示する範囲を「円形の範囲」として定義し、  
カメラやプレイヤーの位置に基づき、座標を変換します。  
また、ワールド座標系をミニマップ上のスクリーン座標系に変換します。

[↑目次に戻る](#toc)   

# 工夫した点

### ①牛の挙動  
牛の移動処理で休憩時間と移動時間を追加し、さらにそれをランダムにすることにより、より自然な牛に近い動きを再現しました。  
※RANDOMCOW_TIMERは定数で240になっています。

```  
/** タイマーが0以上なら新しい方向を決める */
if (m_moveTimer <= 0)
{
	if (m_isMove)
	{
		Vector3 dir
		(
			/** (0, 1, 2, から - 1を引いているので) - 1, 0, 1の範囲でランダムな値を生成 */
			rand() % 3 - 1,//x
			0,             //yは常に0
			rand() % 3 - 1//z
		);

		/** 0,0,0になったら一秒休む */
		if (dir.LengthSq() == 0)
		{
			dir = Vector3(1, 0, 0);
		}
		dir.Normalize();
		m_moveDir = dir;
		/** 1～4秒間ランダムに方向を変える */
		m_moveTimer = rand() % RANDOMCOW_TIMER;
		m_isMove = false;
	}

		/** 移動した後に必ず休む */
		else
		{
			m_moveDir = Vector3::Zero;
			/** 1～4秒間のランダム時間に休む */
			m_moveTimer = rand() % RANDOMCOW_TIMER;
			m_isMove = true;
		}
	}
/** 移動 */
Vector3 pos = m_transform.GetPosition();
/** 少しづつ位置を動かしている */
pos += m_moveDir * m_moveSpeed * g_gameTime->GetFrameDeltaTime();
/** ポジションを更新 */
m_transform.SetPosition(pos);
/** モデルに位置を反映 */
m_cowmodelRender.SetPosition(m_transform.GetPosition());
/** タイマーを減らす */
m_moveTimer--;

```  

[↑目次に戻る](#toc)  

### ②UFOが牛を見つける挙動について  
UFOは自身に一番近い牛を標的とし、追いかけることで、より敵らしい動きを再現しました。  

```  
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

### ③牛がロープに引っ張られる処理  
プレイヤー側がロープを引っ張るボタンを押したとき、捕まっている牛がプレイヤーの位置に近づくような処理を書くことにより、ロープを左右で交互に引っ張っているような感覚を再現しました。  

### プレイヤー側の処理
```  
if (m_rope)
{
	//ロープが牛に当たっているとき
	if (m_rope->GetIsHitCow())
	{
        /** 右ボタンを押したら */
		if (g_pad[0]->IsTrigger(enButtonRB1))
		{
			m_isRightButton1 = true;
			m_isLeftButton1 = false;

			m_isRightButton1_Trigger = true;
			m_isLeftButton1_Trigger = false;
		}

        /** 左ボタンを押したら */
		else if (g_pad[0]->IsTrigger(enButtonLB1))
		{
			m_isLeftButton1 = true;
			m_isRightButton1 = false;

			m_isLeftButton1_Trigger = true;
			m_isRightButton1_Trigger = false;
		}
	}
}
```  

### 牛の処理  
※PULL_ROPEは定数で、8.0fという値になっています。
```  
/** プレイヤー側の左右ボタンを押したというフラグがどちらか立っていれば */
if (m_player->GetIsRightButton1() or m_player->GetIsLeftButton1())
{
	/** プレイヤーの位置を取得 */
	Vector3 playerPos = m_player->GetPosition();

	/** 牛の位置 */
	Vector3 cowPos = m_transform.GetPosition();

	/** プレイヤーへの方向 */
	Vector3 dir = playerPos - cowPos;

	/** 正規化 */
	dir.Normalize();

	/** 牛をプレイヤーのいる位置まで徐々に移動 */
	cowPos += dir * PULL_POWER;

	m_transform.SetPosition(cowPos);

	m_cowmodelRender.SetPosition(m_transform.GetPosition());

    /** 牛は引っ張られると左右ボタンのフラグを折る */
	m_player->SetGetLeftButton1(false);
	m_player->SetGetRightButton1(false);
}
```  

[↑目次に戻る](#toc)  

### ④ロード中にオブジェクトが生成される処理  
ロード中に重たいオブジェクトをSwitch文で制御し、生成することにより、ゲームが開始した際のカクつきを改善し、ゲームを始めるのが早くなりました。  

```  
void LoadingScene::LoadGameObjectsStepByStep()
{
    switch (m_loadStep)
    {
	    /** ロードするゲームオブジェクトをステップバイステップで生成する */

	    /** プレイヤーを生成 */
        case 0: NewGO<Player>(0, "player"); break;

	    /** ステージを生成 */
        case 1: NewGO<Stage>(0, "stage"); break;

	    /** 牛を生成(10体分) */
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
	    {
	        Cow* cow = NewGO<Cow>(0, "cow");
	        cow->SetPosition(RandomCowPos());
	        m_tempCows.push_back(cow);
        } break;

	    /** もしUFOが消えていなかったら残っているUFOを消す */
        case 12:
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

	    /** UFOの生成前にマネージャーを生成*/
        case 13:
	    NewGO<UFOLightManager>(0, "ufolightmanager");
	    break;

	    /** UFOを生成(4体分) */
        case 14:
        case 15:
        case 16:
        case 17:
        {

	        int index = m_loadStep - 14;
	        if (index >= 0 && index < 4)
	        {
		    UFO* ufo = NewGO<UFO>(0, "UFO");
		    ufo->SetPosition(UFO_INFOMATIONS[index].pos);
		    m_tempUFOs.push_back(ufo);
	        }
        } break;

	    /** ゲームカメラを生成 */
        case 18: NewGO<GameCamera>(0, "gameCamera"); break;
	    /** スカイキューブを生成 */
        case 19:
        {
	
	        /** SkyCube を生成 */
	        SkyCube* sky = NewGO<SkyCube>(0, "skyCube");

	        /** タイプ設定 */
	        sky->SetType(EnSkyCubeType::enSkyCubeType_Night);

	        /** スケール設定 */
	        sky->SetScale(10000.0f);

	        /** IBL 設定 */
	        g_renderingEngine->SetAmbientByIBLTexture(sky->GetTextureFilePath(), 0.05f);
        } break;

	    /** ゲーム本体を生成 */
        case 20:
	    Game* game =NewGO<Game>(0, "game");

	    /** ロードした牛をゲームに渡す */
	    for (auto cow : m_tempCows)
	    {
		    game->GetAliveCows().push_back(cow);
	    }
	
	    /** ロードしたUFOをゲームに渡す */
	    game->SetUFOList(m_tempUFOs);

	    DeleteGO(this);
	    return;
    }

m_loadStep++;
}
```  

[↑目次に戻る](#toc)  

### ⑤タイトルからゲーム画面に遷移する際のロードとアウトゲームかやポーズ画面からタイトルに戻る際のロード画面  
ローディングシーンのヘッダー内でenumを使ったタイプを作り、次のシーンに遷移する際にロード画面のタイプを指定することにより、タイトル画面からゲームに遷移する際はオブジェクトを生成し、そのほかのシーンからタイトルなどに戻る際はオブジェクトを生成しないようにしました。  

###  LoadingScene.h内  
```  

/** 次のシーンをセットする関数*/
void SetNextScene(std::function<void()>next);

/** ロードするシーンのタイプ*/
enum LoadType
{
	/** ゲームシーンに移行するタイプ*/
	ToGameScene,

	/** タイトルシーンに移行するタイプ*/
	ToTitleScene,
};

/** ロードするシーンのタイプ*/
LoadType m_loadType = ToGameScene;

/** ロードするシーンのタイプを設定する関数 */
void SetLoadType(LoadType loadType)
{
	m_loadType = loadType;
}

/** タイトルのみをロードする関数 */
void LoadTitleOnly();

/** ローディング中の処理を行う関数*/
void InLoading();

//これは、LoadingSceneを呼び出すときに、次のシーンをロードする関数を引数で渡してもらうための変数。
/** 次のシーンをロードする関数*/
std::function<void()> m_nextSceneLoading;
```  

### LoadingScene.cpp内  
```
void LoadingScene::InLoading()
{
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	m_timer += deltaTime;
	m_totalTime += deltaTime;

	/** 画像を切り替える処理 */
	if (m_timer >= m_changeTime)
	{
		/** 画像を切り替えるたびにタイマーを初期化する */
		m_timer = 0.0f;

		/** 画像を順番に切り替える */
		m_currentImage = (m_currentImage + 1) % 3;
	}

	/** 一定時間で次のシーンへ移行 */
	if (m_totalTime >= m_loadingTime)
	{
		if (m_loadType == LoadType::ToGameScene)
		{
			/** ゲームシーンに移行するタイプのロード処理 */
			LoadGameObjectsStepByStep(); 
		}

		else
		{
			/** タイトルシーンに移行するタイプのロード処理 */
			LoadTitleOnly();
		}
	}
}

void LoadingScene::LoadTitleOnly()
{
	m_nextSceneLoading();
	DeleteGO(this);
}

void LoadingScene::SetNextScene(std::function<void()>next)
{
	m_nextSceneLoading = next;
}
```

### Title.cpp内  
```  
m_loadingScene = NewGO<LoadingScene>(0, "loading");
m_loadingScene->SetLoadType(LoadingScene::LoadType::ToGameScene);
DeleteGO(m_titleBGM);
m_loadingScene->SetNextScene([]()
	{
		NewGO<Game>(0, "game");
	});
DeleteGO(this);
```  

### GameClear.cpp内  
```  
m_loadingScene = NewGO<LoadingScene>(0, "loading");
m_loadingScene->SetLoadType(LoadingScene::LoadType::ToTitleScene);
DeleteGO(m_clearBGM);
m_loadingScene->SetNextScene([]()
	{
		/** タイトルの画像を呼び出す*/
		NewGO<Title>(0, "title");
	});

DeleteGO(this);
```  

### GameOver.cpp内  
```  
m_loadingScene = NewGO<LoadingScene>(0, "loading");
m_loadingScene->SetLoadType(LoadingScene::LoadType::ToTitleScene);
DeleteGO(m_deathBGM);
m_loadingScene->SetNextScene([]()
	{
		/** タイトルの画像を呼び出す*/
		NewGO<Title>(0, "title");
	});
DeleteGO(this);
```  

### Pause.cpp内  
```  
m_loadingScene = NewGO<LoadingScene>(0, "loading");
m_loadingScene->SetLoadType(LoadingScene::LoadType::ToTitleScene);
m_loadingScene->SetNextScene([]()
	{
		/** タイトルの画像を呼び出す*/
		NewGO<Title>(0, "title");
	});

DeleteGO(m_game);
DeleteGO(this);
```  

[↑目次に戻る](#toc)  

### ⑥アニメーションの補完(モーションブレンド)  

>アニメーションを再生する際に補完時間を設定することにより、アニメーションのカクつきを無くしました。  

> ### 補完無し  
> <img width="1919" height="1057" alt="補完無し" src="https://github.com/user-attachments/assets/5f3ce869-ec7d-4a45-a14a-2db822809b77" />  

> ### 補完あり
> <img width="1919" height="1057" alt="補完あり" src="https://github.com/user-attachments/assets/6e2f9aff-da75-4524-82cb-d4033d3aca78" />  


[↑目次に戻る](#toc)  

### ⑦ロード中の生成  

>switch文を使うことによって、ロード中に1つづつオブジェクトを生成するようにしました。  
これにより、ゲーム開始時に一気に生成しようとして重たくなる状況を解決しました。

[↑目次に戻る](#toc)  
