#include "StdAfx.h"
#include "Boss.h"
#include "resource.h"

CImageList CBoss::m_Images;

CBoss::CBoss(void)
{
	//随机确定X位置
	m_ptPos.x = rand() % (GAME_WIDTH - BOSS_HEIGHT) + 1;

	//随机确定图像索引
	m_nImgIndex = 0;

	//根据图像索引确定方向
	m_nMotion = 1;
	m_ptPos.y = 0;
	//随机确定速度
	m_V = 3;
	m_nWait = 0;
	y = 2;
	x = 1;
}

CBoss::~CBoss(void)
{
}
BOOL CBoss::LoadImage()
{
	return CGameObject::LoadImage(m_Images, IDB_BOSS, RGB(0, 0, 0), 146,86, 1);
}
BOOL CBoss::Draw(CDC* pDC, BOOL bPause)
{
	m_nWait++;
	if (m_nWait>10)
		m_nWait = 0;
	if (!bPause)
	{
		m_ptPos.y = m_ptPos.y + y;
		m_ptPos.x = m_ptPos.x + x;
	}
	if (m_ptPos.x <= 0)
	{
		m_ptPos.x = 0;
		x = 1;
	}
	if (m_ptPos.y <= 15)
	{
		m_ptPos.y = 15;
		y = 2;
	}
	if (m_ptPos.y >= 250)
	{
		m_ptPos.y = 250;
		y = -2;
	}
	if (m_ptPos.x >= GAME_WIDTH - BOSS_WIDTH - 15)
	{
		m_ptPos.x = GAME_WIDTH - BOSS_WIDTH - 15;
		x = -1;
	}
	m_Images.Draw(pDC, m_nImgIndex, m_ptPos, ILD_TRANSPARENT);

	return TRUE;
}

