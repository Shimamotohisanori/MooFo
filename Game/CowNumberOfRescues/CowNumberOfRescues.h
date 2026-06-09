#pragma once
/**
 * 牛の救出数クラス
 */
class CowNumberOfRescues : public IGameObject
{
public:
    CowNumberOfRescues() {};
	~CowNumberOfRescues() {};
	bool Start();
	void Update();
	void Render(RenderContext& renderContext);

	/** 救出数を加算する関数 */
	void AddRescue()
	{
		m_numberOfRescues++;
	}

	/** 救出数を減算する関数 */
	void SubRescue()
	{
		/** 0以下にはしない */
		if (m_numberOfRescues == 0)
		{
			m_numberOfRescues = 0;
			return;
		}

		m_numberOfRescues--;
	}

	/** 牛の救出数を取得 */
	int GetNumberOfRescues() const;
	
	/** セット関数 */
	void SetNumber(int number)
	{
		m_numberOfRescues = number;
	}
	
	/** リザルトをセットする関数 */
	void SetResult(bool isResult);

	/** リザルトの種類 */
	enum class ResultType
	{
		GameClear,
		GameOver,
	};

	/**リザルトの種類をセットする関数 */
	void SetResultType(ResultType type)
	{
		m_resultType = type;
	}

	ResultType m_resultType = ResultType::GameClear;


private:
	/** レイアウトの初期化 */
	void InitLayout();

	/** レイアウトの種類 */
	enum LayoutType
	{
		InGame,/**ゲーム中*/
		GameClear,/** ゲームクリア*/
		GameOver,/** ゲームオーバー*/
		LayoutTypeNum
	};
	/** レイアウトの種類ごとのUIの大きさ */
	
	/** 救出数 */
	Vector3 rescueScale[LayoutTypeNum];
	
	/** 十の位 */
	Vector3 tensScale[LayoutTypeNum];

	/** 一の位 */
	Vector3 onesScale[LayoutTypeNum];

	/** 「/」の大きさ */
	Vector3 slashScale[LayoutTypeNum];

	/** 1の大きさ */
	Vector3 oneScale[LayoutTypeNum];
	
	/** 0の大きさ */
	Vector3 zeroScale[LayoutTypeNum];

	/** +のスケール */
	Vector3 plusScale[LayoutTypeNum];

	/** 追加用十の位のスケール */
	Vector3 additionalTensScale[LayoutTypeNum];

	/** 追加用一の位のスケール */
	Vector3 additionalOnesScale[LayoutTypeNum];

	/** レイアウトの種類ごとのUIの座標 */
	
	/** 救出数 */
	Vector3 rescuePos[LayoutTypeNum];
	
	/** 十の位 */
	Vector3 tensPos[LayoutTypeNum];
	
	/** 一の位 */
	Vector3 onesPos[LayoutTypeNum];
	
	/** 「/」の位置 */
	Vector3 slashPos[LayoutTypeNum];
	
	/** 1の位置 */
	Vector3 onePos[LayoutTypeNum];
	
	/** 0の位置 */
	Vector3 zeroPos[LayoutTypeNum];

	/** +の位置 */
	Vector3 plusPos[LayoutTypeNum];

	/** 追加用十の位の位置 */
	Vector3 additionalTensPos[LayoutTypeNum];

	/** 追加用一の位の位置 */
	Vector3 additionalOnesPos[LayoutTypeNum];

	/** 救出数 */
	uint8_t m_numberOfRescues = 0;

	/**	十の位 */
	uint8_t tens = 0;

	/** 一の位 */
	uint8_t ones = 0;

	/** 追加用の十の位 */
	uint8_t m_additionalTens = 0;

	/** 追加用の一の位 */
	uint8_t m_additionalOnes = 0;

	/** ファイルパス */
	std::string m_filePath[10];

	/** 十の位専用数字画像 */
	SpriteRender m_tensSprite[10];

	/** 一の位専用数字画像 */
	SpriteRender m_onesSprite[10];

	/** 追加の一の位の画像 */
	SpriteRender m_additionalOnesSprite[10];

	/** 追加の十の位の画像 */
	SpriteRender m_additionalTensSprite[10];

	/** 救出画像 */
	SpriteRender m_rescueSprite;

	/**「/」のUI画像 */
	SpriteRender m_slashSprite;

	/** 右にある1の画像 */
	SpriteRender m_rightOneSprite;

	/** 右にある0の画像 */
	SpriteRender m_rightZeroSprite;

	/** +の画像 */
	SpriteRender m_plusSprite;

	/** リザルト */
	bool m_isResult = false;

};

