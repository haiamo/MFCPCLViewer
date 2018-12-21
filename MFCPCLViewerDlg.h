
// MFCPCLViewerDlg.h : 头文件
//

#pragma once

#undef min
#undef max

#include "afxwin.h"
#include <TlHelp32.h>

//pcl
#include <pcl\console\parse.h>
#include <pcl\io\io.h>
#include <pcl\io\ply_io.h>
#include <pcl\point_types.h>
#include <pcl\point_cloud.h>
#include <pcl\common\io.h>
#include <pcl\visualization\pcl_visualizer.h>

//vtk
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindowInteractor3D.h>
#include <vtkOutputWindow.h>

#include<vtkAutoInit.h>

using namespace std;
using namespace pcl;
using namespace Eigen;


// CMFCPCLViewerDlg 对话框
class CMFCPCLViewerDlg : public CDialogEx
{
// 构造
public:
	CMFCPCLViewerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCPCLVIEWER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_pic;

private:
	
	PointCloud<PointXYZRGB>::Ptr m_cloudrgb;
	PointCloud<PointXYZ>::Ptr m_cloud;
	pcl::visualization::PCLVisualizer* m_viewer;
	vtkSmartPointer<vtkRenderWindow> m_win;
	vtkSmartPointer<vtkRenderWindowInteractor> m_iren;
public:
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedCancel();

	BOOL KillProcessFromName(CString strProcessName);

	string m_windowNames;
};

BOOL CALLBACK GetChildrenWindowNames(HWND hwnd, LPARAM lParam);