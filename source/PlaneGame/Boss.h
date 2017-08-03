
#pragma once
#include "GameObject.h"

class CBoss :public CGameObject
{
public:
	CBoss(void);
	~CBoss(void);

	BOOL Draw(CDC* pDC, BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos, CPoint(m_ptPos.x + BOSS_HEIGHT, m_ptPos.y + BOSS_HEIGHT));
	}

	int GetMontion() const
	{
		return m_nMotion;
	}


private:
	static const int BOSS_HEIGHT = 146;
	static const int BOSS_WIDTH = 86;
	static CImageList m_Images;
	int    m_nMotion;//方向 1->向下 0->停止 -1->向上
	//图像索引
	int m_nImgIndex;
	//速度
	int m_V;
	int    m_nWait;//发射延时
	int x, y;
};
