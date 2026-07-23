#pragma once
class Pause;
class CowLivesUI;

/** ゲームの操作説明UIクラス */
class InstructionControllerUI : public IGameObject
{
public:
	InstructionControllerUI() {};
	~InstructionControllerUI();
	bool Start();
	void Update();
	void Render(RenderContext& renderContext);

private:
	/** InstructionControllerUIのスプライトレンダラー */
	SpriteRender m_instructionControllerUI;

	/** ポーズクラス */
	Pause* m_pause = nullptr;

	/** プレイヤーの残機ポインタ*/
	CowLivesUI* m_cowLivesUI = nullptr;

};

