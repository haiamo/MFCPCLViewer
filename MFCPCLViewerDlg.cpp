
// MFCPCLViewerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCPCLViewer.h"
#include "MFCPCLViewerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCPCLViewerDlg 对话框



CMFCPCLViewerDlg::CMFCPCLViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCPCLVIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_viewer = new pcl::visualization::PCLVisualizer("PCL", false);
	m_win = m_viewer->getRenderWindow();
	m_iren = vtkRenderWindowInteractor::New();
	m_win->SetGlobalWarningDisplay(0);
	m_iren->SetGlobalWarningDisplay(0);
	
	m_cloud.reset(::new PointCloud<PointXYZ>);
	m_cloudrgb.reset(::new PointCloud<PointXYZRGB>);
}

void CMFCPCLViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Pic, m_pic);
}

BEGIN_MESSAGE_MAP(CMFCPCLViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_LOAD, &CMFCPCLViewerDlg::OnBnClickedLoad)
	ON_BN_CLICKED(IDCANCEL, &CMFCPCLViewerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMFCPCLViewerDlg 消息处理程序

BOOL CMFCPCLViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//将VTK视图加载到picture control上
	m_viewer->setBackgroundColor(255.0, 150.0, 0.0);
	CRect rect;
	m_pic.GetClientRect(&rect);
	m_win = m_viewer->getRenderWindow();
	m_win->SetSize(rect.right - rect.left, rect.bottom - rect.top);
	m_win->SetParentId(GetDlgItem(IDC_Pic)->GetSafeHwnd());
	m_iren->SetRenderWindow(m_win);
	m_viewer->createInteractor();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCPCLViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCPCLViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	//设置PCLView窗口参数

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCPCLViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCPCLViewerDlg::OnBnClickedLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	vtkOutputWindow* outWin = vtkOutputWindow::GetInstance();
	//CWnd* tmpWnd = FindWindow("", "MFCPCLViewer.exe");
	CWnd* tmpWnd = FindWindowEx(this->GetSafeHwnd(), NULL, outWin->GetClassName(), NULL);
	//EnumChildWindows(this->m_hWnd, GetChildrenWindowNames, NULL);
	EnumWindows(GetChildrenWindowNames, NULL);
	if (tmpWnd != NULL)
	{
		HWND tmphWnd = tmpWnd->GetSafeHwnd();
		CloseHandle(tmphWnd);
	}
	else
	{
		MessageBox("Can't find vtkOutputWindow.");
	}

	CFileDialog fileopendlg(TRUE);
	CString filepath;
	if (fileopendlg.DoModal() == IDOK)
	{
		filepath = fileopendlg.GetPathName();
	}
	else
	{
		MessageBox("Empty file name, please check again!", "Open file failed.");
	}

	m_cloud.reset(::new PointCloud<PointXYZ>);
	m_cloudrgb.reset(::new PointCloud<PointXYZRGB>);

	int fe;
	fe = pcl::io::loadPLYFile(filepath.GetBuffer(), *m_cloudrgb);
	if (fe ==-1)
	{
		MessageBox("PCL loading RGB file failed.", "PCL Failure");
	}
	else
	{
		PointXYZ tmpPt;
		for (size_t ii = 0; ii < m_cloudrgb->points.size(); ii++)
		{
			tmpPt.x = m_cloudrgb->points[ii].x;
			tmpPt.y = m_cloudrgb->points[ii].y;
			tmpPt.z = m_cloudrgb->points[ii].z;
			m_cloud->points.push_back(tmpPt);
		}
		MessageBox("Loaded PCL point cloud successfully.", "Load finshed.");

		if (::IsWindow(this->GetSafeHwnd()))
		{
			if (m_viewer)
			{
				if (m_cloud->points.size() > 0 || m_cloudrgb->points.size() > 0)
				{
					m_viewer->setBackgroundColor(255.0, 150.0, 0.0);
					m_viewer->removeAllPointClouds();

					//显示不同类型ply文件
					vector<PCLPointField> PCLPF;
					if (pcl::getFieldIndex(*m_cloudrgb, "rgb", PCLPF)>0)
					{
						m_viewer->addPointCloud(m_cloudrgb);
					}
					else
					{
						m_viewer->addPointCloud(m_cloud);
					}
					m_viewer->resetCamera();//使点云显示在屏幕中间，并绕中心操作
					CRect rect;
					m_pic.GetClientRect(&rect);
					m_win = m_viewer->getRenderWindow();
					m_win->SetSize(rect.right - rect.left, rect.bottom - rect.top);
					m_win->Render();
				}
			}
		}
	}

}

void CMFCPCLViewerDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_viewer->close();
	m_cloud->clear();
	m_cloud.reset();
	m_cloudrgb->clear();
	m_cloudrgb.reset();
	m_win->Delete();
	m_iren->Delete();


	CDialogEx::OnCancel();
}

BOOL CMFCPCLViewerDlg::KillProcessFromName(CString strProcessName)
{
	//创建进程快照(TH32CS_SNAPPROCESS表示创建所有进程的快照)  
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//PROCESSENTRY32进程快照的结构体  
	PROCESSENTRY32 pe;

	//实例化后使用Process32First获取第一个快照的进程前必做的初始化操作  
	pe.dwSize = sizeof(PROCESSENTRY32);

	//下面的IF效果同:  
	//if(hProcessSnap == INVALID_HANDLE_VALUE)   无效的句柄  
	if (!Process32First(hSnapShot, &pe))
	{
		return FALSE;
	}

	//将字符串转换为小写  
	strProcessName.MakeLower();

	//如果句柄有效  则一直获取下一个句柄循环下去  
	while (Process32Next(hSnapShot, &pe))
	{
		//pe.szExeFile获取当前进程的可执行文件名称  
		CString scTmp = pe.szExeFile;
		//将可执行文件名称所有英文字母修改为小写  
		scTmp.MakeLower();
		//比较当前进程的可执行文件名称和传递进来的文件名称是否相同  
		//相同的话Compare返回0  
		if (!scTmp.Compare(strProcessName))
		{
			//从快照进程中获取该进程的PID(即任务管理器中的PID)  
			DWORD dwProcessID = pe.th32ProcessID;
			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
			::TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);
			return TRUE;
		}
		scTmp.ReleaseBuffer();
	}
	strProcessName.ReleaseBuffer();
	return FALSE;
}

BOOL CALLBACK GetChildrenWindowNames(HWND hwnd, LPARAM lParam)
{
	char* name = new char[50];
	int len = GetWindowTextLength(hwnd);
	GetWindowText(hwnd, name, len + 1);
	if (strcmp(name,"vtkOutputWindow")==0)
	{
		SendMessage(hwnd, WM_CLOSE, 0, 0);
		//CloseHandle(hwnd);
		delete name;
		return 0;
	}
	delete name;
	return 1;
}
