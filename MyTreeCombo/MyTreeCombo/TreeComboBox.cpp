#include "StdAfx.h"
#include "TreeComboBox.h"

#define WM_ACAD_KEEPFOCUS 9999


void SplitString(const CString& strNeedSplit, CAtlArray<CString>& strSplitStrings, CString strSep)
{
	if (strNeedSplit.IsEmpty())
		return;

	int nSepLen = strSep.GetLength();

	int nTextStart = 0;
	int nTextEnd = strNeedSplit.Find(strSep, nTextStart);
	while (-1 != nTextEnd)
	{
		CString strText = strNeedSplit.Mid(nTextStart, nTextEnd - nTextStart);
		strSplitStrings.Add(strText);

		nTextStart = nTextEnd + nSepLen;
		nTextEnd = strNeedSplit.Find(strSep, nTextStart);
	}

	CString strLastText = strNeedSplit.Mid(nTextStart, strNeedSplit.GetLength() - nTextStart);
	if (!strLastText.IsEmpty())
		strSplitStrings.Add(strLastText);
}

CString BuildSplitString(const CAtlArray<CString>& strSplitStrings, CString strSep)
{
	CString strSplitString;
	size_t nSize = strSplitStrings.GetCount();
	for (size_t i = 0; i < nSize; ++i)
	{
		if (i != 0)
		{
			strSplitString += strSep;
		}

		strSplitString += strSplitStrings[i];
	}

	return strSplitString;
}


CComboBoxTree::CComboBoxTree() : _pCombo(NULL)
{
}

CComboBoxTree::~CComboBoxTree()
{
}

BEGIN_MESSAGE_MAP(CComboBoxTree, CTreeCtrl)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(NM_KILLFOCUS, OnKillfocus)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
END_MESSAGE_MAP()

void CComboBoxTree::Display(CRect rect)
{
	if (NULL != GetSafeHwnd())
	{
		CWnd* pTopParent = GetParent();
		SetWindowPos(NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);

		if (NULL != pTopParent)
		{
			pTopParent->SendMessage(WM_NCACTIVATE, TRUE);
		}
	}
}

BOOL CComboBoxTree::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->hwnd == m_hWnd && WM_SYSKEYDOWN == pMsg->message && VK_F4 == pMsg->wParam)
		return TRUE;

	if ((pMsg->hwnd == m_hWnd) && (WM_KEYDOWN == pMsg->message) && (VK_ESCAPE == pMsg->wParam))
	{
		::PostMessage(_pCombo->m_hWnd, UM_DROP_CLOSE, (WPARAM)0, (LPARAM)0);
		return TRUE;
	}

	return CTreeCtrl::PreTranslateMessage(pMsg);
}

void CComboBoxTree::OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult)
{
	::PostMessage(_pCombo->m_hWnd, UM_DROP_CLOSE, (WPARAM)0, (LPARAM)0);
}

void CComboBoxTree::OnMouseMove(UINT nFlags, CPoint point)
{
	CTreeCtrl::OnMouseMove(nFlags, point);

	if (_pCombo->IsCheckMode())
		return;

	UINT uFlags;
	HTREEITEM hItem = HitTest(point, &uFlags);
	if (NULL != hItem)
	{
		SelectItem(hItem);
	}
}

void CComboBoxTree::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	ScreenToClient(&point);

	UINT uFlags;
	HTREEITEM hItem = HitTest(point, &uFlags);
	if (hItem == NULL)
		return;

	if (_pCombo->IsCheckMode())
	{
		SelectItem(hItem);

		if (!(uFlags & TVHT_ONITEMSTATEICON))
			return;

		BOOL bCheck = !GetCheck(hItem);
		if (_pCombo->IsSingleCheck() && ItemHasChildren(hItem))
		{
			*pResult = 1;
			return;
		}

		SetCheck(hItem, bCheck);

		ConsistentChildCheck(hItem);
		ConsistentParentCheck(hItem);

		// 进行选择
		SetCheck(hItem, !bCheck);
	}
	else
	{
		//只能选择叶子节点
		if ((NULL != _pCombo) && (hItem != NULL) && (uFlags & TVHT_ONITEM) && !ItemHasChildren(hItem))
		{
			DWORD_PTR pData = GetItemData(hItem);
			CString strText = GetItemText(hItem);

			int nIndex = CB_ERR, subIndex = CB_ERR;
			do
			{
				subIndex = _pCombo->FindStringExact(nIndex, strText);
				
				if (subIndex <= nIndex)
					break;

				if (_pCombo->GetItemData(subIndex) == pData)
					break;

				nIndex = subIndex;
			} while (true);

			if (subIndex > nIndex)
				_pCombo->SetCurSel(subIndex);

			::PostMessage(_pCombo->m_hWnd, UM_DROP_CLOSE, (WPARAM)1, (LPARAM)0);
		}
	}

	*pResult = 0;
}

void CComboBoxTree::ConsistentChildCheck(HTREEITEM hItem)
{
	BOOL bCheck = GetCheck(hItem);

	// 当勾选一个时把其他所有选择项去除
	if (_pCombo->IsSingleCheck())
	{
		HTREEITEM hParentItem = GetParentItem(hItem);
		if (hParentItem == NULL)
			return;

		HTREEITEM hChildItem = GetChildItem(hParentItem);
		while (hChildItem != NULL)
		{
			if (hChildItem != hItem)
				SetCheck(hChildItem, FALSE);
			hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
		}
	}
	else if (ItemHasChildren(hItem))
	{
		HTREEITEM hChildItem = GetChildItem(hItem);
		while (hChildItem != NULL)
		{
			SetCheck(hChildItem, bCheck);

			ConsistentChildCheck(hChildItem);

			hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
		}
	}
}

void CComboBoxTree::ConsistentParentCheck(HTREEITEM hItem)
{
	HTREEITEM hParentItem = GetParentItem(hItem);
	if (hParentItem != NULL)
	{
		HTREEITEM hChildItem = GetChildItem(hParentItem);
		while (hChildItem != NULL)
		{
			if (GetCheck(hChildItem) == FALSE)
			{
				SetCheck(hParentItem, FALSE);

				return ConsistentParentCheck(hParentItem);
			}

			hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
		}

		SetCheck(hParentItem, TRUE);

		return ConsistentParentCheck(hParentItem);
	}
}

void CComboBoxTree::OnDestroy()
{
	CImageList *pImageNormal = GetImageList(TVSIL_NORMAL);
	if (pImageNormal != NULL)
		pImageNormal->DeleteImageList();

	CImageList *pImageState = GetImageList(TVSIL_STATE);
	if (pImageState != NULL)
		pImageState->DeleteImageList();

	CTreeCtrl::OnDestroy();
}

LRESULT CComboBoxTree::OnAcadKeepFocus(WPARAM wParam, LPARAM lParam)
{
	return 1;
}



const CString CTreeComboBox::s_strNoData = L"NODATA";

IMPLEMENT_DYNAMIC(CTreeComboBox, CComboBox) 

CTreeComboBox::CTreeComboBox() :_nDroppedHeight(150), _nDroppedWidth(-1)
{
	_pOwner = NULL;
	_bInCreate = false;
	_bControlActive = FALSE;
	_bCheckMode = false;
	_bShowComboList = false;
	_bDelItemData = false;
	_bInShowDropDown = false;
	_bSigleCheck = false;
	_bSelectByText = true;
}

CTreeComboBox::~CTreeComboBox()
{
}

BEGIN_MESSAGE_MAP(CTreeComboBox, CComboBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(UM_DROP_CLOSE, OnCloseControl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CTreeComboBox::PreSubclassWindow()
{
	CComboBox::PreSubclassWindow();

	if (!_bInCreate)
		InitDropTree();
}

BOOL CTreeComboBox::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message && VK_DOWN == pMsg->wParam)
	{
		DisplayTree();
		return TRUE;
	}
	
	if (WM_MOUSEWHEEL == pMsg->message && IsCheckMode())
	{
		return TRUE;
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

static void GetTreeLeafValueString(CTreeCtrl& treeCtrl, HTREEITEM hParent, CAtlArray<CString>& leftValues, bool bAll = false)
{
	// 遍历子节点
	if (treeCtrl.ItemHasChildren(hParent))
	{
		HTREEITEM hChildItem = treeCtrl.GetChildItem(hParent);
		while (hChildItem != NULL)
		{
			// 递归子级
			GetTreeLeafValueString(treeCtrl, hChildItem, leftValues, bAll);
			hChildItem = treeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
		}
	}
	else if (treeCtrl.GetCheck(hParent) || bAll)
	{
		CString strText = treeCtrl.GetItemText(hParent);
		leftValues.Add(strText);
	}
}

static void GetTreeLeafDataString(CTreeCtrl& treeCtrl, HTREEITEM hParent, CAtlArray<CString>& leftDatas)
{
	// 遍历子节点
	if (treeCtrl.ItemHasChildren(hParent))
	{
		HTREEITEM hChildItem = treeCtrl.GetChildItem(hParent);
		while (hChildItem != NULL)
		{
			// 递归子级
			GetTreeLeafDataString(treeCtrl, hChildItem, leftDatas);
			hChildItem = treeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
		}
	}
	else if (treeCtrl.GetCheck(hParent))
	{
		CString *pData = (CString*)(treeCtrl.GetItemData(hParent));
		
		if (pData)
			leftDatas.Add(*pData);
		else
			leftDatas.Add(L"");
	}
}

static void GetTreeLeafValueString(CTreeCtrl& treeCtrl, CAtlArray<CString>& leftValues, bool bAll = false)
{
	CString strText;
	HTREEITEM rootItem = treeCtrl.GetRootItem();
	while (rootItem != NULL)
	{
		GetTreeLeafValueString(treeCtrl, rootItem, leftValues, bAll);
		rootItem = treeCtrl.GetNextItem(rootItem, TVGN_NEXT);
	}
}

static void GetTreeLeafDataString(CTreeCtrl& treeCtrl, CAtlArray<CString>& leftDatas)
{
	CString strText;
	HTREEITEM rootItem = treeCtrl.GetRootItem();
	while (rootItem != NULL)
	{
		GetTreeLeafDataString(treeCtrl, rootItem, leftDatas);
		rootItem = treeCtrl.GetNextItem(rootItem, TVGN_NEXT);
	}
}


CString CTreeComboBox::GetControlText()
{
	CString strText;
	if (!_bCheckMode || !_bShowComboList)
	{
		GetWindowText(strText);
	}
	else
	{
		CAtlArray<CString> leatValues;
		GetTreeLeafValueString(_dropTree, leatValues);
		if (leatValues.IsEmpty() && !((GetStyle() & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST))
		{
			GetWindowText(strText);

			if (!strText.IsEmpty())
			{
				CAtlArray<CString> strSplitStrings;
				SplitString(strText, strSplitStrings, GetSplitChar());
				
				GetTreeLeafValueString(_dropTree, leatValues, true);
				CString strLeafValue = BuildSplitString(leatValues, GetSplitChar());
				
				for (size_t i = 0; i < strSplitStrings.GetCount();)
				{
					if (strLeafValue.Find(strSplitStrings[i]) != -1)
						strSplitStrings.RemoveAt(i);
					else
						++i;
				}
				
				strText = BuildSplitString(strSplitStrings, GetSplitChar());
			}

		}
		else
		{
			strText = BuildSplitString(leatValues, GetSplitChar());
		}
	}

	return strText;
}

bool CTreeComboBox::SearchDataByText(CTreeCtrl &treeCtrl , HTREEITEM hParent , CString &curText , CString *curData)
{
	// 遍历子节点
	if(treeCtrl.ItemHasChildren(hParent))
	{
		HTREEITEM hChildItem = treeCtrl.GetChildItem(hParent);
		while(hChildItem != NULL)
		{
			// 递归子级
			if(!SearchDataByText(treeCtrl , hChildItem , curText , curData))
				hChildItem = treeCtrl.GetNextItem(hChildItem , TVGN_NEXT);
			else
				return true;
		}

		return false;
	}
	else
	{
		CString Text = treeCtrl.GetItemText(hParent);

		if((Text == curText))
		{
			DWORD_PTR data = (treeCtrl.GetItemData(hParent));
			if(data != NULL)
			{
				*curData = *((CString*)data);
				return true;
			}
		}

		return false;
	}
}

bool CTreeComboBox::CheckDataExist(CTreeCtrl &treeCtrl, HTREEITEM hParent, CString *curData, bool *isExist, CString strShow)
{
	if(treeCtrl.ItemHasChildren(hParent))
	{
		HTREEITEM hChildItem = treeCtrl.GetChildItem(hParent);
		while(hChildItem != NULL)
		{
			// 递归子级
			if (!CheckDataExist(treeCtrl, hChildItem, curData, isExist, strShow))
				hChildItem = treeCtrl.GetNextItem(hChildItem , TVGN_NEXT);
			else
				return true;
		}

		return false;
	}
	else
	{
		DWORD_PTR data = (treeCtrl.GetItemData(hParent));
		CString strText = treeCtrl.GetItemText(hParent);
		if (data && *((CString*)data) == *curData && (strText == strShow))
		{
			*isExist = true;
			return true;
		}

		return false;
	}
}

CString CTreeComboBox::GetDataFromText(CString curText)
{
	// 判断data是否存在，且对应的展现值是否控件显示值一致
	if (!_strCurSelData.IsEmpty())
	{
		bool bDataExistCheck = false;
		HTREEITEM rootItem = _dropTree.GetRootItem();
		while(rootItem != NULL)
		{
			if (!CheckDataExist(_dropTree, rootItem, &_strCurSelData, &bDataExistCheck, curText))
				rootItem = _dropTree.GetNextItem(rootItem , TVGN_NEXT);
			else
				break;
		}

		// 如果一致，则直接返回
		if(bDataExistCheck)
			return _strCurSelData;
	}

	CString Data;
	if (_bSelectByText)
	{
		Data = GetDataByText(curText);

	}
	return Data;
}

CString CTreeComboBox::GetCurrentSelData()
{
	if (!_strCurSelData.IsEmpty())
		return _strCurSelData;

	CString strText;
	if (!_bCheckMode || !_bShowComboList)
	{
		int nSel = GetCurSel();
		if (LB_ERR != nSel)
		{
			CString* pStrData = (CString*)GetItemData(nSel);
			if (NULL != pStrData)
				strText = *pStrData;
		}
	}
	else
	{
		CAtlArray<CString> leftDatas;
		GetTreeLeafDataString(_dropTree, leftDatas);
		strText = BuildSplitString(leftDatas, GetSplitChar());
	}

	return strText;
}

void CTreeComboBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	_bControlActive = !_bControlActive;

	if (_bControlActive)
		DisplayTree();
}

void CTreeComboBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags, point);
}

LRESULT CTreeComboBox::OnCloseControl(WPARAM wParam, LPARAM lParam)
{
	SetFocus();

	if (_bCheckMode)
	{
		int nLast = GetCount();
		if (nLast > 0)
			DeleteString(nLast - 1);

		int nSel = AddString(GetControlText());
		if (LB_ERR != nSel)
		{
			SetCurSel(nSel);
		}

		// 暂时不处理多选时附加数据，如果需要则改造
	}

	if (wParam == 1)
	{
		_strCurSelText = L"";
		_strCurSelData = L"";
	}

	CWnd* pParent = GetParent();
	if ((pParent != NULL) && (NULL != pParent->GetSafeHwnd()))
	{
		WPARAM wParam = MAKEWPARAM(GetDlgCtrlID(), CBN_SELCHANGE);
		pParent->SendMessage(WM_COMMAND, wParam, (LPARAM)m_hWnd);
	}

	if (NULL != _dropTree.GetSafeHwnd())
	{
		_dropTree.ShowWindow(SW_HIDE);
	}

	_bControlActive = FALSE;

	return 1;
}

CString CTreeComboBox::GetCheckString()
{
	if (!_strCurSelText.IsEmpty())
		return _strCurSelText;

	int nSel = GetCurSel();
	CString strSelText;
	if (LB_ERR != nSel)
	{
		GetLBText(nSel, strSelText);
	}

	return strSelText;
}

void CTreeComboBox::SetCheckString(const CString& strCheck)
{
	int nSel = AddString(strCheck);
	if (LB_ERR != nSel)
	{
		SetCurSel(nSel);
	}
}

void CTreeComboBox::SetSplitChar(const CString& ch)
{
	_splitCh = ch;
}

CString CTreeComboBox::GetSplitChar() const
{
	return _splitCh.IsEmpty() ? L"," : _splitCh;
}

HTREEITEM CTreeComboBox::FindItemByText(HTREEITEM hRoot, CString strItem, CString strData)
{
	while (NULL != hRoot)
	{
		CString strFindData;
		CString strFindItem = _dropTree.GetItemText(hRoot);
		CString *pStrFindData = (CString*)(_dropTree.GetItemData(hRoot));
			
		if (pStrFindData != NULL)
			strFindData = *pStrFindData;

		if (strData == strFindData && (!_bSelectByText || (_bSelectByText && strFindItem == strItem)))
			return hRoot;

		if (strItem.IsEmpty() && !strData.IsEmpty() && strData == strFindData)
			return hRoot;
		
		// 查找子节点
		HTREEITEM hFindItem = FindItemByText(_dropTree.GetNextItem(hRoot, TVGN_CHILD), strItem, strData);
		if (hFindItem != NULL)
			return hFindItem;

		hRoot = _dropTree.GetNextSiblingItem(hRoot);
	}

	return NULL;
}

void CTreeComboBox::CollapseAllTreeItem(HTREEITEM hRoot)
{
	while (NULL != hRoot)
	{
		_dropTree.Expand(hRoot, TVE_COLLAPSE);
		CollapseAllTreeItem(_dropTree.GetNextItem(hRoot, TVGN_CHILD));
		hRoot = _dropTree.GetNextSiblingItem(hRoot);
	}
}

void CTreeComboBox::GetShowItem(HTREEITEM hRoot, int& nCount)
{
	while (NULL != hRoot)
	{
		if (TVIS_EXPANDED & _dropTree.GetItemState(hRoot, TVIS_EXPANDED))
		{
			GetShowItem(_dropTree.GetNextItem(hRoot, TVGN_CHILD), nCount);
		}

		nCount++;
		hRoot = _dropTree.GetNextSiblingItem(hRoot);
	}
}

void CTreeComboBox::DisplayTree()
{
	_bInShowDropDown = true;

	CRect rect;
	GetWindowRect(rect);

	CAtlArray<HTREEITEM> checkItems;

	if (!IsCheckMode())
	{
		// 收缩所有树节点
		CollapseAllTreeItem(_dropTree.GetRootItem());

		CString strSelText = GetControlText();
		CString strSelData = GetDataFromText(strSelText);
		HTREEITEM hSelItem = FindItemByText(_dropTree.GetRootItem(), strSelText, strSelData);
		if (NULL == hSelItem)
		{
			GetWindowText(strSelText);
			strSelData = GetCurrentSelData();
			hSelItem = FindItemByText(_dropTree.GetRootItem(), strSelText, strSelData);
		}

		if (NULL == hSelItem)
		{
			hSelItem = FindItemByText(_dropTree.GetRootItem(), GetCheckString(), strSelData);
		}

		if (NULL != hSelItem)
		{
			_dropTree.EnsureVisible(hSelItem);
			_dropTree.SelectItem(hSelItem);
		}
		else
		{
			_dropTree.PostMessage(WM_VSCROLL, SB_TOP, NULL);
		}
	}
	else
	{
		int nSel = GetCurSel();
		CString strSelText, strSelData;
		if (LB_ERR != nSel)
			GetLBText(nSel, strSelText);

		CAtlArray<CString> strSelects, strDatas;
		strSelData = GetCurrentSelData();
		SplitString(strSelText, strSelects, GetSplitChar());
		SplitString(strSelData, strDatas, GetSplitChar());

		size_t nSize = strSelects.GetCount();
		for (size_t i = 0; i < nSize; ++i)
		{
			CString strData = i >= strDatas.GetCount() ? _T("") : strDatas[i];
			HTREEITEM hSelItem = FindItemByText(_dropTree.GetRootItem(), strSelects[i], strData);
			if (NULL != hSelItem)
			{
				_dropTree.EnsureVisible(hSelItem);
				checkItems.Add(hSelItem);
			}
		}

		if (nSize == 0)
			_dropTree.PostMessage(WM_VSCROLL, SB_TOP, NULL);

		_dropTree.SendMessage(WM_HSCROLL, SB_LEFT, NULL);
	}

	// 计算下拉框高度
	CRect rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rt, 0);
	int nScreenHeight = rt.Height();

	int nCtrlHeight = rect.Height();
	int nDroppedHeight = _nDroppedHeight;
	if (NULL != _dropTree.GetSafeHwnd())
	{
		int nCount = 0;
		GetShowItem(_dropTree.GetRootItem(), nCount);

		if (nCount > 1)
		{
			int nDesiredDroppedHeight = nCount * _dropTree.GetItemHeight();
			if (nDesiredDroppedHeight > (nScreenHeight - nCtrlHeight) / 2)
				nDesiredDroppedHeight = (nScreenHeight - nCtrlHeight) / 2;
			if (nDroppedHeight < nDesiredDroppedHeight)
				nDroppedHeight = nDesiredDroppedHeight;
		}
	}

	if (rect.bottom + nDroppedHeight > nScreenHeight)
	{
		rect.top = rect.top - nDroppedHeight;
		rect.bottom = nDroppedHeight + 1;
	}
	else
	{
		rect.top = rect.bottom;
		rect.bottom = nDroppedHeight;
	}

	rect.right = GetDroppedWidth();

	if (!_dropTree.IsWindowEnabled())
	{
		_dropTree.EnableWindow(TRUE);
	}	
	
	_dropTree.Display(rect);

	size_t nSize = checkItems.GetCount();
	HTREEITEM hSelItemInTree = _dropTree.GetSelectedItem();

	if (nSize == 0)
	{
		_dropTree.SetCheck(_dropTree.GetRootItem(), false);
	}

	for (size_t i = 0; i < nSize; ++i)
	{
		_dropTree.SelectItem(checkItems[i]);
		_dropTree.SetCheck(checkItems[i], true);
		_dropTree.ConsistentParentCheck(checkItems[i]);
	}

	_bShowComboList = true;
	_bInShowDropDown = false;
}

void CTreeComboBox::LoadStringVector(CStringVector& listData, bool bNoLimitOrSelAll, const CString strSelAll)
{
	if (IsCheckMode())
	{
		ResetContent();
		_dropTree.DeleteAllItems();

		HTREEITEM hRootItem = TVI_ROOT;

		if (bNoLimitOrSelAll)
		{
			if (strSelAll.IsEmpty())
			{
				hRootItem = _dropTree.InsertItem(_T("全选"), TVI_ROOT);
			}
			else
			{
				hRootItem = _dropTree.InsertItem(strSelAll, TVI_ROOT);
			}
						
			_dropTree.SetCheck(hRootItem);
		}

		CStringVectorIter endIter = listData.end();
		for (CStringVectorIter beginIter = listData.begin(); beginIter != endIter; ++beginIter)
		{
			const CString& strString = (*beginIter);
			_dropTree.InsertItem(strString, hRootItem);
		}
	}
	else
	{
		ResetContent();
		_dropTree.DeleteAllItems();

		CStringVectorIter endIter = listData.end();
		for (CStringVectorIter beginIter = listData.begin(); beginIter != endIter; ++beginIter)
		{
			const CString& strString = (*beginIter);
			_dropTree.InsertItem(strString, TVI_ROOT);
		}

		if (bNoLimitOrSelAll)
		{
			if (strSelAll.IsEmpty())
			{
				AddString(L"不限");
			}
			else
			{
				AddString(strSelAll);
			}
			
			HTREEITEM hNewItem = _dropTree.InsertItem(L"不限", TVI_ROOT);
		}
	}
}

HTREEITEM CTreeComboBox::InsertItem(const CString& strText,
	CString strData, HTREEITEM hParent, bool isLeafNode)
{
	_bDelItemData = true;

	HTREEITEM hItem = _dropTree.InsertItem(strText, hParent);

	if (isLeafNode)
	{
		int nIndex = AddString(strText);
		CString *pData = new CString(strData);
		SetItemData(nIndex, (DWORD_PTR)pData);
		_dropTree.SetItemData(hItem, (DWORD_PTR)pData);
	}

	return hItem;
}

BOOL CTreeComboBox::PreCreateWindow(CREATESTRUCT& cs)
{
	_bInCreate = true;
	return CComboBox::PreCreateWindow(cs);
}

void CTreeComboBox::InitDropTree()
{
	CWnd *pParentWnd = this->GetParent();
	do
	{
		if (pParentWnd == NULL)
			break;

		if (!(pParentWnd->GetStyle() & WS_CHILD))
			break;

		pParentWnd = pParentWnd->GetParent();
	} while (TRUE);

	CRect rcClient(0, 0, 0, 0);

	// 创建控件
	DWORD dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_BORDER | TVS_DISABLEDRAGDROP | TVS_HASLINES 
	| TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT;

	if (_bCheckMode)
		dwStyle |= TVS_CHECKBOXES;

	CWnd* pWnd = &_dropTree;

	pWnd->CreateEx(0, WC_TREEVIEW, NULL, dwStyle, rcClient, pParentWnd, 0, NULL);

	_dropTree.Init(this);

	_bShowComboList = false;
}

int CTreeComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitDropTree();

	_bInCreate = false;

	return 0;
}

void CTreeComboBox::ResetContent()
{
	if (!_bDelItemData)
	{
		CComboBox::ResetContent();
		return;
	}

	// 删除附加数据
	int nCount = GetCount();
	for (int i = 0; i < nCount; ++i)
	{
		DWORD_PTR pData = GetItemData(i);

		if (pData != NULL)
		{
			CString* pStrData = (CString*)(pData);
			delete pStrData;
		}
	}
	CComboBox::ResetContent();
}

void CTreeComboBox::OnDestroy()
{
	CComboBox::OnDestroy();
	_dropTree.DestroyWindow();
	
	ResetContent();

	_bControlActive = FALSE;
}

CTreeCtrl& CTreeComboBox::GetTreeCtrl()
{
	return _dropTree;
}

BOOL CTreeComboBox::IsControlActive() const
{
	return _bControlActive;
}

int GetTreeMaxWidth(CTreeCtrl& treeCtrl, CDC* pDc, HTREEITEM hParent, int nLevel)
{
	CString strText = treeCtrl.GetItemText(hParent);
	
	// 计算宽度
	CSize size = pDc->GetTextExtent(strText, strText.GetLength());
	int nMaxWidth = size.cx + nLevel * 40 + 4;

	HTREEITEM hChildItem = treeCtrl.GetChildItem(hParent);
	while (hChildItem != NULL)
	{
		int nWidth = GetTreeMaxWidth(treeCtrl, pDc, hChildItem, nLevel + 1);
		if (nWidth > nMaxWidth)
			nMaxWidth = nWidth;

		hChildItem = treeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
	}

	return nMaxWidth;
}

int GetTreeMaxWidth(CTreeCtrl& treeCtrl)
{
	int nMaxWidth = 0;
	
	CFont* font = treeCtrl.GetFont();
	CDC* dc = treeCtrl.GetDC();
	CFont* oldFont = dc->SelectObject(font);

	HTREEITEM rootItem = treeCtrl.GetRootItem();
	while (rootItem != NULL)
	{
		int nWidth = GetTreeMaxWidth(treeCtrl, dc, rootItem, 1);
		if (nWidth > nMaxWidth)
			nMaxWidth = nWidth;

		rootItem = treeCtrl.GetNextItem(rootItem, TVGN_NEXT);
	}

	dc->SelectObject(oldFont);
	treeCtrl.ReleaseDC(dc);

	return nMaxWidth;
}

int CTreeComboBox::GetDroppedWidth()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	if (_nDroppedWidth == 0)
	{
		int nMaxWidth = GetTreeMaxWidth(_dropTree);
		if (nMaxWidth < rcClient.Width())
			return rcClient.Width();
		else
			return nMaxWidth;
	}
	else if (_nDroppedWidth == -1)
	{
		return rcClient.Width();
	}

	return _nDroppedWidth;
}

int CTreeComboBox::GetDroppedHeight() const
{
	return _nDroppedHeight;
}

void CTreeComboBox::SetDroppedWidth(const int nWidth)
{
	_nDroppedWidth = nWidth;
}

void CTreeComboBox::SetDroppedHeight(const int nHeight)
{
	_nDroppedHeight = nHeight;
}

bool CTreeComboBox::IsCheckMode()
{
	return _bCheckMode;
}

void CTreeComboBox::SetCheckMode(bool bCheckModel)
{
	_bCheckMode = bCheckModel;
}

void CTreeComboBox::SetParentOwner(CWnd* pOwner)
{
	_pOwner = pOwner;
}

CWnd* CTreeComboBox::GetParentOwner()
{
	return (_pOwner != NULL) ? _pOwner : GetParent();
}

void CTreeComboBox::ResetItemData()
{
	_strCurSelData = L"";
	_strCurSelText = L"";
}

const CString CTreeComboBox::NoData()
{
	return CTreeComboBox::s_strNoData;
}

CString CTreeComboBox::GetDataByText(CString &text)
{
	CString Data;

	HTREEITEM rootItem = _dropTree.GetRootItem();
	while(rootItem != NULL)
	{
		if(!SearchDataByText(_dropTree , rootItem , text , &Data))
			rootItem = _dropTree.GetNextItem(rootItem , TVGN_NEXT);
		else
			break;
	}

	return Data;
}

void CTreeComboBox::SetSelectByText( bool bSelectByText )
{
	_bSelectByText = bSelectByText;
}

void CTreeComboBox::SetSelectData( CString strText, CString StrData )
{
	_strCurSelText = strText;
	_strCurSelData = StrData;
}
