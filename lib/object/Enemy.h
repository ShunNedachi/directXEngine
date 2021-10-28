#pragma once
#include"Object.h"

// 基底クラス
class Enemy
{

protected:
	// エイリアス
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	// 関数
	Enemy() {};
	~Enemy() { Destroy(); }

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	void Destroy();

	/// <summary>
	/// ダメージを受けた時の処理
	/// </summary>
	/// <param name="pos">攻撃者の位置</param>
	/// <param name="damage"></param>
	void RecieveDamage(XMFLOAT3 pos, float damage);


	// setter
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	void SetSize(XMFLOAT3 size) { obj->SetSize(size); }
	void SetRadius(float r) { obj->SetRadius(r); }

	// getter
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetScale() { return scale; }
	XMFLOAT3 GetRotation() { return rotation;}
	XMFLOAT3 GetSize() { return obj->GetSize(); }
	float GetRadius() { return obj->GetRadius(); }

	int GetDamage() { return damage; }
	bool GetIsDamage() { return isDamage; }

	int GetHealth() { return health; }

	// 攻撃判定用
	//AttackBase* GetAttack() { return attack; }
	Object* GetOBJ() { return obj; }


protected:
	Object* obj = nullptr;

	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 scale = { 0,0,0 };
	XMFLOAT3 rotation = { 0,0,0 };

	// ステータス用
	float health = 10;
	float experience;

	// ダメージをくらっているのかの判定
	bool isDamage = false;
	// ダメージ状態の経過フレーム数
	int damageCount;
	// ダメージ状態の最大フレーム数
	const int DAMAGE_FRAME = 20;

	// 仮設定のダメージ量
	int damage = 1;

};

