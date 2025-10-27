#pragma once
#include "Common/Transform.h"

class Map;
class Capsule;

class Player
{
public:

	enum class STATE
	{
		IDLE,
		WALK,
		RUN,
		FATIGUE,
		SNEAK
	};

	enum class PASSIVE
	{
		NONE,
		STRAW_DOLL,
		TABI,
		GREEN_PURSE,
		HAND_MIRROR
	};

	enum class ACTIVE
	{
		NONE,
		BELL,
		LIGHT_STONE,
		KALEIDOSCOPE,
		COMPASS,
	};

	static constexpr float SPEED = 4.0f;
	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -100.0f, 500.0f };
	static constexpr int STAMINA_POS_X = 500;
	static constexpr int STAMINA_POS_Y = 560;
	static constexpr int STAMINA_SIZE_X = 250;
	static constexpr int STAMINA_SIZE_Y = 20;
	static constexpr int STAMINA_MAX = 255;

	// コンストラクタ
	Player(void);

	// デストラクタ
	~Player(void);

	void Init(void);

	void Update(void);
	void AfterUpdate(void);

	void Draw(void);

	void Release(void);

	Transform& GetTransform(void);
	VECTOR GetMoveDir(void) const;
	STATE GetState(void);

	//パッシブアイテム
	void ChangePassive(PASSIVE passive);
	PASSIVE GetPassive(void);

	//アクティブアイテム
	void ChangeActive(ACTIVE active);
	ACTIVE GetActive(void);

	void SelectActiveItem(void);

	//藁人形を所持しているとき（ワープ）
	void StrawDollUpdate(void);

	//鈴
	void PickBell(void);
	void UseBell(void);
	int GetBellCount(void);

	//光る石
	void PickLightStone(void);
	void UseLightStone(void);
	int GetLightStoneCount(void);

	//万華鏡
	void PickKaleidoScope(void);
	void UseKaleidoScope(void);
	int GetKaleidoScopeCount(void);

	//コンパス
	void PickCompass(void);
	int GetCompassCount(void);

	//お守り
	void PickCharm(void);
	int GetCharmCount(void);

	void SetIsAlive(bool isAlive);
	bool GetIsAlive(void);

	void Collision(std::vector<int> modelIds);
private:

	// モデル制御の基本情報
	Transform transform_;

	STATE state_;

	PASSIVE passive_;

	ACTIVE active_;

	VECTOR targetPos_;

	VECTOR moveDir_;

	VECTOR movedPos_;

	int warpSe_;
	int stoneSe_;

	bool isAlive_;

	bool povFlag_;

	//スピード
	float speed_;
	float tabiSpeed_;

	float staminaRecoverySpeed_;

	int bellCount_;
	int lightStoneCount_;
	int kaleidoScopeCount_;
	int compassCount_;
	int charmCount_;

	int mouseWheelRotVol_;

	void Move(void);
	void DrawStamina(float stamina, int staminaMax);
	void ChangeState(STATE state);
};

