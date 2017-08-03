#include "StdAfx.h"
#include "Ball.h"
#include "resource.h"

CImageList CBall::m_Images;

CBall::CBall(int x, int y, int nMontion, int i) :CGameObject(x, y), m_nMotion(nMontion), ii(i)
{

}

CBall::~CBall(void)
{
}
BOOL CBall::Draw(CDC* pDC,BOOL bPause)
{
	if(!bPause)
	{
		m_ptPos.y = m_ptPos.y + m_nMotion * 8;
		if (ii==1)
		{
			m_ptPos.x = m_ptPos.x + m_nMotion * 3;
		}
		if (ii == -1)
		{
			m_ptPos.x = m_ptPos.x - m_nMotion * 3;
		}
	}

	if(m_ptPos.y > GAME_HEIGHT+BALL_HEIGHT )
		return FALSE;
	if(m_ptPos.y < -BALL_HEIGHT)
		return FALSE;

	m_Images.Draw(pDC,0,m_ptPos,ILD_TRANSPARENT);

	return TRUE;
}

BOOL CBall::LoadImage()
{
	return CGameObject::LoadImage(m_Images,IDB_BALL,RGB(0,0,0),8,8,1);
}