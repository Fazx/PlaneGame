#include "StdAfx.h"
#include "Enemy.h"
#include "resource.h"

CImageList CEnemy::m_Images;

CEnemy::CEnemy(void)
{
	//随机确定X位置
	m_ptPos.x = rand()%(GAME_WIDTH-ENEMY_HEIGHT)+1;

	//随机确定图像索引
	//从上往下飞
	if (rand() % 5 != 0)
	{
		m_nImgIndex = 0;
		m_nMotion = 1;
		m_ptPos.y = -ENEMY_HEIGHT;
	}
	//若rand是5的倍数，从下往上飞（数量较少）
	else
	{
		m_nImgIndex = 1;
		m_nMotion = -1;
		m_ptPos.y = GAME_HEIGHT + ENEMY_HEIGHT;
	}
	//随机确定速度
	m_Vy = rand() % 6 + 2;
	//x方向，左右两边
	if (rand() % 2 == 0)
	    m_Vx = rand() % 3 - 2;
	else
		m_Vx = rand() % 3 ;

	m_nWait=0;
}

CEnemy::~CEnemy(void)
{
}
BOOL CEnemy::LoadImage()
{
	return CGameObject::LoadImage(m_Images,IDB_ENEMY,RGB(0,0,0),35,35,2);
}
BOOL CEnemy::Draw(CDC* pDC,BOOL bPause)
{
	m_nWait++;
	if(m_nWait>20)
		m_nWait=0;

	if(!bPause)
	{
		m_ptPos.y = m_ptPos.y + m_nMotion * m_Vy;
		m_ptPos.x = m_ptPos.x + m_nMotion * m_Vx;
	}

	if(m_ptPos.y > GAME_HEIGHT+ENEMY_HEIGHT )
		return FALSE;
	if(m_ptPos.y < -ENEMY_HEIGHT)
		return FALSE;

	m_Images.Draw(pDC,m_nImgIndex,m_ptPos,ILD_TRANSPARENT);

	return TRUE;
}
BOOL CEnemy::Fired()
{
	if(m_nWait==0)
		return TRUE;
	else
		return FALSE;
}