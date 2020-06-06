
#pragma once

#include <vector>
#include <atlcoll.h>

#define UM_DROP_CLOSE	(WM_USER + 101)

class CTreeComboBox;

typedef std::vector<CString> CStringVector;
typedef CStringVector::iterator CStringVectorIter;

//内部类请不要当控件使用
class  CComboBoxTree : public CTreeCtrl
{
public:
	CComboBoxTree();
	virtual ~CComboBoxTree();

protected:
	DECLARE_MESSAGE_MAP()

public:
virtual void Display(CRect rect);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Init(CTreeComboBox* pCombo) { _pCombo = pCombo; };

public:
	void ConsistentChildCheck(HTREEITEM hItem);
	void ConsistentParentCheck(HTREEITEM hItem);

protected:
	afx_msg void OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnAcadKeepFocus(WPARAM wParam, LPARAM lParam);

private:
	CTreeComboBox*		_pCombo;		
};

//复选框组合框控件
class  CTreeComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CTreeComboBox)

public:
	CTreeComboBox();
	virtual ~CTreeComboBox();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void DisplayTree();
	CTreeCtrl& GetTreeCtrl();
	BOOL IsControlActive() const;
	int GetDroppedWidth();
	int GetDroppedHeight() const;

	void SetDroppedWidth(const int nWidth);
	void SetDroppedHeight(const int nHeight);

	bool IsCheckMode();

	//设置是否为选择框模式
	void SetCheckMode(bool bCheckModel);
	
	//加载字符串
	void LoadStringVector(CStringVector& listData, 
		bool bNoLimitOrSelAll = false, const CString strSelAll = L"");

	CString GetCheckString();
	void SetCheckString(const CString& strCheck);
	void SetSplitChar(const CString& ch);
	CString GetSplitChar() const;
	CString GetCurrentSelData();

	void SetParentOwner(CWnd* pOwner);
	CWnd* GetParentOwner();

	HTREEITEM InsertItem(const CString& strTest,
		CString strData, HTREEITEM hParent = TVI_ROOT, bool isLeafNode = false);

	//清除数据
	virtual void ResetContent();

	//设置复选框单选
	void SetSingleCheck(bool bCheck) { _bSigleCheck = bCheck; }
	bool IsSingleCheck() { return _bSigleCheck;  }

	void SetSelectByText(bool bSelectByText);
	void SetSelectData(CString strText, CString StrData);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ResetItemData();
	static const CString NoData();
	CString GetDataByText(CString &text);

protected:

	CString GetControlText();
	void InitDropTree();
	void CollapseAllTreeItem(HTREEITEM hRoot);
	HTREEITEM FindItemByText(HTREEITEM hRoot, CString strItem, CString strData);
	void GetShowItem(HTREEITEM hRoot, int& nCount);
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	CString GetDataFromText(CString curText);
	bool SearchDataByText(CTreeCtrl &treeCtrl , HTREEITEM hParent , CString &curText , CString *curData);
	bool CheckDataExist(CTreeCtrl &treeCtrl , HTREEITEM hParent , CString *curData, bool *isExist, CString strShow);

protected:
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnCloseControl(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	CWnd* _pOwner;				
	int _nDroppedWidth;		 
	int _nDroppedHeight;		
	bool _bInCreate;			
	BOOL _bControlActive;		
	CComboBoxTree _dropTree;	
	bool _bCheckMode;			//复选框模式
	bool _bShowComboList;		
	bool _bDelItemData;		
	bool _bInShowDropDown;	
	bool _bSigleCheck;			
	bool _bSelectByText;		
	CString _splitCh;			
	CString _strCurSelText;	
	CString _strCurSelData;	

private:
	static const CString					s_strNoData;
};