#pragma once
#include "Source/Actor/Actor.h"

/**
 * キャラクタークラス
 */
class Character : public Actor
{
public:
	Character();
	~Character();


public:
	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;

	/**
	 * キャラクターを動かすために必要な関数
	 */
	virtual void Move();

};

