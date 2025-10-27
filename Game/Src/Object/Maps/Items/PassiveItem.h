#pragma once
#include "ItemBase.h"

class PassiveItem : public ItemBase
{
public:

	enum class PASSIVE
	{
		NONE,
		STRAW_DOLL,
		TABI,
		GREEN_PURSE,
		HAND_MIRROR
	};

	PassiveItem(VECTOR pos, float axis, PASSIVE passive);
	~PassiveItem(void);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	PASSIVE GetPassive(void);
	void ChangePassive(PASSIVE passive);
protected:

	PASSIVE passive_;

	// ÉpÉâÉÅÅ[É^ê›íË
	void SetParam(void) override;
};
