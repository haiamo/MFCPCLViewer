
// MFCPCLViewerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCPCLViewer.h"
#include "MFCPCLViewerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFCPCLViewerDlg �Ի���



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


// CMFCPCLViewerDlg ��Ϣ�������

BOOL CMFCPCLViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//��VTK��ͼ���ص�picture control��
	m_viewer->setBackgroundColor(255.0, 150.0, 0.0);
	CRect rect;
	m_pic.GetClientRect(&rect);
	m_win = m_viewer->getRenderWindow();
	m_win->SetSize(rect.right - rect.left, rect.bottom - rect.top);
	m_win->SetParentId(GetDlgItem(IDC_Pic)->GetSafeHwnd());
	m_iren->SetRenderWindow(m_win);
	m_viewer->createInteractor();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCPCLViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	//����PCLView���ڲ���

}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCPCLViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCPCLViewerDlg::OnBnClickedLoad()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

					//��ʾ��ͬ����ply�ļ�
					vector<PCLPointField> PCLPF;
					if (pcl::getFieldIndex(*m_cloudrgb, "rgb", PCLPF)>0)
					{
						m_viewer->addPointCloud(m_cloudrgb);
					}
					else
					{
						m_viewer->addPointCloud(m_cloud);
					}
					m_viewer->resetCamera();//ʹ������ʾ����Ļ�м䣬�������Ĳ���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//�������̿���(TH32CS_SNAPPROCESS��ʾ�������н��̵Ŀ���)  
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//PROCESSENTRY32���̿��յĽṹ��  
	PROCESSENTRY32 pe;

	//ʵ������ʹ��Process32First��ȡ��һ�����յĽ���ǰ�����ĳ�ʼ������  
	pe.dwSize = sizeof(PROCESSENTRY32);

	//�����IFЧ��ͬ:  
	//if(hProcessSnap == INVALID_HANDLE_VALUE)   ��Ч�ľ��  
	if (!Process32First(hSnapShot, &pe))
	{
		return FALSE;
	}

	//���ַ���ת��ΪСд  
	strProcessName.MakeLower();

	//��������Ч  ��һֱ��ȡ��һ�����ѭ����ȥ  
	while (Process32Next(hSnapShot, &pe))
	{
		//pe.szExeFile��ȡ��ǰ���̵Ŀ�ִ���ļ�����  
		CString scTmp = pe.szExeFile;
		//����ִ���ļ���������Ӣ����ĸ�޸�ΪСд  
		scTmp.MakeLower();
		//�Ƚϵ�ǰ���̵Ŀ�ִ���ļ����ƺʹ��ݽ������ļ������Ƿ���ͬ  
		//��ͬ�Ļ�Compare����0  
		if (!scTmp.Compare(strProcessName))
		{
			//�ӿ��ս����л�ȡ�ý��̵�PID(������������е�PID)  
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
