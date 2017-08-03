// PlaneGameView.cpp : CPlaneGameView 类的实现
//

//自动发射子弹
//死亡后复活与直接重新开始（分数清零）
//计分系统及显示
//飞机在x方向上的速度，正向与负向都有
//减少后方敌机数目，敌机大多从前方出现
//子弹和飞机速度，复活位置，背景图
//无关卡，但游戏难度随时间增大
//复活及重新开始时清屏
//暂停游戏与恢复
//无敌模式
//敌机子弹分三个阶段，且有x方向的偏移量
//500分出现boss，1000分删除
//自动追踪导弹（未完成）

#include "stdafx.h"
#include "PlaneGame.h"
#include "PlaneGameDoc.h"
#include "PlaneGameView.h"
#include "MyPlane.h"
#include "Enemy.h"
#include "Bomb.h"
#include "Ball.h"
#include "Boss.h"
#include "Explosion.h"
#include <atlimage.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlaneGameView

IMPLEMENT_DYNCREATE(CPlaneGameView, CView)

BEGIN_MESSAGE_MAP(CPlaneGameView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()

END_MESSAGE_MAP()

// CPlaneGameView 构造/析构

CPlaneGameView::CPlaneGameView() :m_pMe(NULL), ispause(0), GOD(0)
{
	// TODO: 在此处添加构造代码
	
}

CPlaneGameView::~CPlaneGameView()
{
}

BOOL CPlaneGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPlaneGameView 绘制

void CPlaneGameView::OnDraw(CDC* /*pDC*/)
{
	CPlaneGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CPlaneGameView 打印

BOOL CPlaneGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPlaneGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPlaneGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CPlaneGameView 诊断

#ifdef _DEBUG
void CPlaneGameView::AssertValid() const
{
	CView::AssertValid();
}

void CPlaneGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlaneGameDoc* CPlaneGameView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlaneGameDoc)));
	return (CPlaneGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlaneGameView 消息处理程序
void CPlaneGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: 在此添加专用代码和/或调用基类
	//初始化游戏
	InitGame();
}
void CPlaneGameView::StopGame()
{
	delete m_pMe;
	delete m_pMemDC;
	delete m_pDC;
	delete m_pMemBitmap;
}

BOOL CPlaneGameView::InitGame()
{
	CRect rc;
	GetClientRect(rc);

	//产生随机数种子
	srand( (unsigned)time( NULL ) );

	//建立设备DC
	m_pDC = new CClientDC(this);

	//建立内存DC
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(m_pDC);

	//建立内存位图
	m_pMemBitmap = new CBitmap;
	m_pMemBitmap->CreateCompatibleBitmap(m_pDC,GAME_WIDTH,GAME_HEIGHT);

	//将位图选入内存DC
	m_pMemDC->SelectObject(m_pMemBitmap);

	CMyPlane::LoadImage();
	CEnemy::LoadImage();
	CBomb::LoadImage();
	CBall::LoadImage();
	CExplosion::LoadImage();
    CBoss::LoadImage();

	//产生主角(战机)
	m_pMe = new CMyPlane;

	//启动游戏
	SetTimer(1,30,NULL);

	return TRUE;
}

void CPlaneGameView::UpdateFrame(CDC* pMemDC)
{
	//绘制天空
	//pMemDC->FillSolidRect(0, 0, GAME_WIDTH, GAME_HEIGHT, RGB(84, 142, 239));

	// 定义位图对象
	CBitmap bitmap;
	//导入图片
	bitmap.LoadBitmap(IDB_BACKGROUND);
	CBrush brush;
	brush.CreatePatternBrush(&bitmap);
	CBrush* ZBrush= pMemDC->SelectObject(&brush);
	pMemDC->FillRect(CRect(0, 0, GAME_WIDTH, GAME_HEIGHT), ZBrush);


	//显示分数
	wchar_t temp[99999];
	CString s1 = _T("SCORE : ");
	wsprintf(temp, L"%d", score);
	s1 += temp;
	CString s2 = _T("按F5重新开始");
	CString s3 = _T("按Z键开启无敌");
	CString s4 = _T("按P键暂停");
	CString s5 = _T("按S键继续");
	CString s6 = _T("按X键关闭无敌");
	pMemDC->SetBkMode(TRANSPARENT);
	pMemDC->SetTextAlign(TA_CENTER);
	pMemDC->SetTextColor(RGB(255, 255, 255));
	pMemDC->TextOut(90, 180, s1);
	pMemDC->TextOut(100, 50, s2);
	pMemDC->TextOut(105,75, s3);
	pMemDC->TextOut(88,100, s4);
	pMemDC->TextOut(88, 125, s5);
	pMemDC->TextOut(107, 150, s6);
	//绘制我方战机
	if(m_pMe!=NULL)
	{
	   m_pMe->Draw(m_pMemDC,FALSE);
	}

	//复活
	else if (m_pMe == NULL && GetKey(VK_SPACE)==1)
	{
		//重新计分
		score = 0;
		//重新产生战机
		m_pMe = new CMyPlane;
		m_pMe->Draw(m_pMemDC, FALSE);
		//清屏
		KillTimer(1);
	
		for (int i = 0; i < 4; i++)
		{
			m_ObjList[i].RemoveAll();
		}
		SetTimer(1, 30, NULL);
	}

	else
	{   //Game Over
		CString str = _T("Press SPACE to continue!");
		
		pMemDC->SetBkMode(TRANSPARENT);
		pMemDC->SetTextAlign(TA_CENTER);
		pMemDC->SetTextColor(RGB(255,0,0));
		pMemDC->TextOut(GAME_WIDTH/2,GAME_HEIGHT/2,str);
	}
	
	//绘制 导弹、爆炸、敌机、子弹
	for(int i=0;i<4;i++)
	{
		POSITION pos1,pos2;
		for( pos1 = m_ObjList[i].GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
		{
			CGameObject* pObj = (CGameObject*)m_ObjList[i].GetNext( pos1 );
			if(!pObj->Draw(pMemDC,FALSE))
			{
				m_ObjList[i].RemoveAt(pos2);
				delete pObj;
			}
		}
	}
	if (score > 500 && score < 1000)
	{
		m_boss.Draw(m_pMemDC,FALSE);
	}

	//复制内存DC到设备DC
	m_pDC->BitBlt(0,0,GAME_WIDTH,GAME_HEIGHT,m_pMemDC,0,0,SRCCOPY);
}
void CPlaneGameView::AI()
{
	static int nCreator = rand() % 5 + 10;

	//随机产生敌机 
	if(nCreator<=1)
	{
		if (score <= 100)
		{
			nCreator = rand() % 5 + 15;
			m_ObjList[enEnemy].AddTail(new CEnemy);
		}
		else if (score > 100 && score <= 300) 
		{
			nCreator = rand() % 5 + 10;
			m_ObjList[enEnemy].AddTail(new CEnemy);
		}
		else if (score > 300 && score <= 500)
		{
			nCreator = rand() % 5 + 7;
			m_ObjList[enEnemy].AddTail(new CEnemy);
		}
		else
		{
			nCreator = rand() % 5+3;
			m_ObjList[enEnemy].AddTail(new CEnemy);
		}
	}
	nCreator--;
		
	if(m_pMe==NULL)
		return;

	//检测四个方向键，移动战机
	for(int i=0;i<4;i++)
	{
		int nMeMotion=0;
		m_pMe->SetVerMotion(0);
		m_pMe->SetHorMotion(0);

		nMeMotion = GetKey(VK_UP);
		if(nMeMotion==1)
			m_pMe->SetVerMotion(1);
		    
		nMeMotion = GetKey(VK_DOWN);
		if(nMeMotion==1)
			m_pMe->SetVerMotion(-1);

		nMeMotion = GetKey(VK_RIGHT);
		if(nMeMotion==1)
			m_pMe->SetHorMotion(1);

		nMeMotion = GetKey(VK_LEFT);
		if(nMeMotion==1)
			m_pMe->SetHorMotion(-1);
	}
	
	//产生战机导弹
	if(1)//自动发射
	{
		if(m_pMe!=NULL && m_pMe->Fired())
		{
			CPoint pt = m_pMe->GetPoint();
			m_ObjList[enBomb].AddTail(new CBomb(pt.x+10,pt.y+10));
			m_ObjList[enBomb].AddTail(new CBomb(pt.x+30,pt.y+10));
		}
	}


	//敌机发射子弹

	CPoint PlanePt = m_pMe->GetPoint();
	for(POSITION ePos=m_ObjList[enEnemy].GetHeadPosition();ePos!=NULL;)
	{
          CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos);
		  if(!pEnemy->Fired())
			  continue;
		  CPoint  ePt = pEnemy->GetPoint();

		  BOOL by=FALSE;

		  //敌机在战机前面
		  if(pEnemy->GetMontion()==1 && ePt.y<PlanePt.y)
		         by=  TRUE;
		  //敌机在战机后面
		  if(pEnemy->GetMontion()==-1 && ePt.y>PlanePt.y)
				 by=  TRUE;	

		  if(by && ePt.x >= PlanePt.x && ePt.x<PlanePt.x+CMyPlane::PLANE_WIDTH && score<250)
		  {
			  m_ObjList[enBall].AddTail(new CBall(ePt.x + 10, ePt.y + 10, pEnemy->GetMontion(),0));
		  }
		  else if (by && ePt.x >= PlanePt.x && ePt.x<PlanePt.x + CMyPlane::PLANE_WIDTH && score>=250 &&score<500)
		  {
			  m_ObjList[enBall].AddTail(new CBall(ePt.x + 20, ePt.y + 10, pEnemy->GetMontion(),1));
			  m_ObjList[enBall].AddTail(new CBall(ePt.x - 20, ePt.y + 10, pEnemy->GetMontion(),-1));
		  }
		  else if (by && ePt.x >= PlanePt.x && ePt.x < PlanePt.x + CMyPlane::PLANE_WIDTH && score >= 500)
		  {
			  m_ObjList[enBall].AddTail(new CBall(ePt.x + 10, ePt.y + 10, pEnemy->GetMontion(),0));
			  m_ObjList[enBall].AddTail(new CBall(ePt.x + 50, ePt.y + 10, pEnemy->GetMontion(),1));
			  m_ObjList[enBall].AddTail(new CBall(ePt.x - 30, ePt.y + 10, pEnemy->GetMontion(),-1));
		  }
	}
	
	//敌机子弹炸掉战机
	POSITION bPos1=NULL,bPos2=NULL;
	CRect mRect = m_pMe->GetRect();

	//重新开始游戏
	if (GetKey(VK_F5) == 1)
	{
		//清屏
		KillTimer(1);

		for (int i = 0; i < 4; i++)
		{
			m_ObjList[i].RemoveAll();
		}
		SetTimer(1, 30, NULL);
		//重新计分
		score = 0;
		//删除战机
		delete m_pMe;
		m_pMe = NULL;
		//重新产生战机
		m_pMe = new CMyPlane;
		m_pMe->Draw(m_pMemDC, FALSE);
		//关闭无敌模式
		GOD = 0;
	}
	for(bPos1=m_ObjList[enBall].GetHeadPosition();( bPos2 = bPos1 ) != NULL;)
	{

		CBall* pBall = (CBall*)m_ObjList[enBall].GetNext(bPos1);
		CRect bRect = pBall->GetRect();
		CRect tmpRect;
		
		if(tmpRect.IntersectRect(&bRect,mRect))
		{

			//添加爆炸效果
			m_ObjList[enExplosion].AddTail(
				new CExplosion(mRect.left,mRect.top)
				);

			//删除子弹
			m_ObjList[enBall].RemoveAt(bPos2);
			delete pBall;

			//删除战机
			if (GOD == 0)
			{
				delete m_pMe;
				m_pMe = NULL;
			}
			break;
		}
	}

	//战机导弹炸掉敌机
	POSITION mPos1=NULL,mPos2=NULL;
	for(mPos1=m_ObjList[enBomb].GetHeadPosition();(mPos2=mPos1)!=NULL;)
	{
		CBomb* pBomb = (CBomb*)m_ObjList[enBomb].GetNext(mPos1);
		CRect bRect = pBomb->GetRect();

		POSITION ePos1=NULL,ePos2=NULL;
		for(ePos1=m_ObjList[enEnemy].GetHeadPosition();(ePos2=ePos1)!=NULL;)
		{
			CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos1);
			CRect mRect = pEnemy->GetRect();
			CRect tmpRect;
			if(tmpRect.IntersectRect(&bRect,mRect))
			{
				//加分
				score += 10;
				//添加爆炸效果
				m_ObjList[enExplosion].AddTail(
					new CExplosion(mRect.left,mRect.top)
					);
				//删除导弹
				m_ObjList[enBomb].RemoveAt(mPos2);
				delete pBomb;

				//删除敌机
				m_ObjList[enEnemy].RemoveAt(ePos2);
				delete pEnemy;
				break;
				
			}
		}
	}
	if (score > 500 && score < 1000)
	{
		mPos1 = NULL, mPos2 = NULL;
		for (mPos1 = m_ObjList[enBomb].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
		{
			CBomb* pBomb = (CBomb*)m_ObjList[enBomb].GetNext(mPos1);
			CRect bRect = pBomb->GetRect();
				CRect mRect = m_boss.GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bRect, mRect))
				{
					//加分
					//score += 10;
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(new CExplosion(mRect.left + 40, mRect.top + 40));
					//删除导弹
					m_ObjList[enBomb].RemoveAt(mPos2);
					delete pBomb;
					//delete pEnemy;
					break;
				}

		}
	}
	
}
void CPlaneGameView::OnTimer(UINT_PTR nIDEvent)
{
	if (GetKeyState('P') < 0)
		ispause = 1;
	if (GetKeyState('S') < 0)
		ispause = 0;
	if (GetKeyState('Z') < 0)
		GOD = 1;
	if (GetKeyState('X') < 0)
		GOD = 0;

	//刷新游戏帧画面: 在内存DC上绘图
	if (!ispause)
	{
		UpdateFrame(m_pMemDC);
		AI();
	}
	
	CView::OnTimer(nIDEvent);
}


