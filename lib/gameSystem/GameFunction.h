#include<DirectXMath.h>
#include"Enemy.h"
#pragma once

// クラス間での情報交換や値の受け渡しをobjectManagerを通さず行う
// また処理をまとめるために使用
namespace GameFunction
{
	void AddEXP(float exp);

	void LoadGameSceneTexture();

	DirectX::XMFLOAT3 GetPlayerPos();
	bool GetPlayerIsSpecial();


	// 念のため処理区分用
	void LoadPlayerTexture();
	void LoadBossTexture();
	void LoadNumberTexture();
}
