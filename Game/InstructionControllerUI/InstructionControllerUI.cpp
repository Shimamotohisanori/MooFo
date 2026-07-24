#include "stdafx.h"
#include "InstructionControllerUI.h"
#include "Pause/Pause.h"
#include "GameScene/LoadingScene.h"
#include"CowLivesUI.h"
namespace
{
	/** InstructionControllerUIのスプライトのパス */
	const char* INSTRUCTION_CONTROLLER_UI_PATH = "Assets/sprite/InstructionUI/InstructionController3.dds";

	/** InstructionControllerUIのスプライトの位置 */
	const Vector3 INSTRUCTION_CONTROLLER_UI_POSITION = Vector3(800.0f, -410.0f, 0.0f);

	/** InstructionControllerUIのスプライトの大きさ */
	const Vector3 INSTRUCTION_CONTROLLER_UI_SCALE = Vector3(1.2f, 1.2f, 1.2f);
}

InstructionControllerUI::~InstructionControllerUI()
{}

bool InstructionControllerUI::Start()
{
	m_instructionControllerUI.Init(INSTRUCTION_CONTROLLER_UI_PATH, 250.0f, 250.0f);
	m_instructionControllerUI.SetPosition(INSTRUCTION_CONTROLLER_UI_POSITION);
	m_instructionControllerUI.SetScale(INSTRUCTION_CONTROLLER_UI_SCALE);
	m_instructionControllerUI.Update();

	m_pause = FindGO<Pause>("pause");
	return true;
}

void InstructionControllerUI::Update()
{
	m_cowLivesUI = FindGO<CowLivesUI>("cowlivesui");

	m_instructionControllerUI.Update();
}

void InstructionControllerUI::Render(RenderContext & renderContext)
{
	/**フェード完了までUIの表示を遅らす*/
	LoadingScene* lodingScene = FindGO<LoadingScene>("loading");
	if (lodingScene != nullptr && !lodingScene->GetLoadingEnd())
	{
		return;
	}

	/** ポーズ中なら描画しない */
	if (m_pause && m_pause->GetIsPause())
	{
		return;
	}

	if (m_cowLivesUI && m_cowLivesUI->IsFadeInComplete())
	{
		return;
	}

	m_instructionControllerUI.Draw(renderContext);
}
