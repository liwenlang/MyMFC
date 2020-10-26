// CStringBuilder.cpp: implementation of the CStringBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CDataSet_StringBuilder.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDataSet_StringBuilder_Bak::CDataSet_StringBuilder_Bak() : m_pNode(NULL) , m_iNodeSize(StringBuilder_NodeSize)
{
	//m_iNodeSize = StringBuilder_NodeSize;
	//m_pNode = NULL;
}

CDataSet_StringBuilder_Bak::CDataSet_StringBuilder_Bak(size_t iNodeSize) : m_pNode(NULL) , m_iNodeSize(iNodeSize)
{
	//m_iNodeSize = iNodeSize;
	//m_pNode = NULL;
}

CDataSet_StringBuilder_Bak::~CDataSet_StringBuilder_Bak()
{
	Empty();
	m_pNode = NULL;
}
//
//CDataSet_StringBuilder_Bak::CStringNode::CStringNode() : m_pMem(NULL),m_iNodeUse(0),m_pNextStringNode(NULL),m_pStringBuilder(NULL)
//{
//	//m_pMem = NULL;
//	//m_iNodeUse = 0;
//	//m_pNextStringNode = NULL;
//
//	//m_pStringBuilder = NULL;
//}

CDataSet_StringBuilder_Bak::CStringNode::CStringNode(const CStringNode & stringNode): m_pMem(NULL),\
																					m_iNodeUse(0),\
																					m_pNextStringNode(NULL),\
																					m_pStringBuilder(NULL)
{
	m_pStringBuilder = stringNode.m_pStringBuilder;
	size_t iBufferSize = m_pStringBuilder->GetNodeSize() * sizeof(TCHAR);
	m_pMem = new BYTE[iBufferSize];
	memset(m_pMem,0,iBufferSize - 1);
}

CDataSet_StringBuilder_Bak::CStringNode::CStringNode(CDataSet_StringBuilder_Bak * pStringBuilder) : m_pMem(NULL),m_iNodeUse(0),m_pNextStringNode(NULL),m_pStringBuilder(NULL)
{
	m_pStringBuilder = pStringBuilder;

	size_t iBufferSize = m_pStringBuilder->GetNodeSize() * sizeof(TCHAR);
	m_pMem = new BYTE[iBufferSize];
	memset(m_pMem,0,iBufferSize - 1);
}

CDataSet_StringBuilder_Bak::CStringNode::~CStringNode()
{
	/********************************************************************
	liwenlang
		时间:2010:11:8   10:20
		备注:先清除后一个 节点
	*********************************************************************/
	if(NULL != m_pNextStringNode)
	{
		delete m_pNextStringNode;
		m_pNextStringNode = NULL;
	}
	if(NULL != m_pMem)
	{
		delete [] m_pMem;
		m_pMem = NULL;
	}
	m_pStringBuilder = NULL;
	m_iNodeUse = 0;
}

BYTE * CDataSet_StringBuilder_Bak::CStringNode::GetBuffer()
{
	return m_pMem;
}

void CDataSet_StringBuilder_Bak::CStringNode::AppendBuffer(LPVOID pBuffer,size_t iBufferSize)
{
	ASSERT(m_pStringBuilder);
	ASSERT(m_pMem);
	if(NULL == m_pStringBuilder)
		return;
	if(NULL == m_pMem)
		return;

	size_t iNodeSize = m_pStringBuilder->GetNodeSize();

	if(m_iNodeUse + iBufferSize <= iNodeSize)
	{//这个节点可以放下
		size_t iWriteBufferSize = iBufferSize * sizeof(TCHAR);
		size_t iCurPosMove = m_iNodeUse * sizeof(TCHAR);
		CopyMemory(m_pMem + iCurPosMove,pBuffer,iWriteBufferSize);
		m_iNodeUse += iBufferSize;
	}
	else if(m_iNodeUse == iNodeSize && NULL != m_pNextStringNode)
	{
		m_pNextStringNode->AppendBuffer(pBuffer,iBufferSize);
	}
	else
	{//这个节点放不下
		size_t iTemSize = 0;
		if(NULL == m_pNextStringNode)
		{
			size_t iAddSize = iNodeSize - m_iNodeUse;
			iTemSize = iAddSize * sizeof(TCHAR);
			size_t iCurPosMove = m_iNodeUse * sizeof(TCHAR);
			CopyMemory(m_pMem + iCurPosMove,pBuffer,iTemSize);
			m_iNodeUse += iAddSize;
			iBufferSize -= iAddSize;
		}

		m_pNextStringNode = new CDataSet_StringBuilder_Bak::CStringNode(m_pStringBuilder);
		m_pNextStringNode->AppendBuffer((BYTE*)pBuffer + iTemSize,iBufferSize);	
	}
}


//加串
void CDataSet_StringBuilder_Bak::Append(LPCTSTR pStr)
{	
	Append((BYTE*)pStr,_tcslen(pStr) );
}


void CDataSet_StringBuilder_Bak::Append(BYTE * pBuffer,size_t iBufferSize)
{
//	UIBase::CTiming timing(_T("CDataSet_StringBuilder_Bak::Append"),TRUE);
	//增长的字节数
	if(NULL == m_pNode)
		m_pNode = new CDataSet_StringBuilder_Bak::CStringNode(this);

	CDataSet_StringBuilder_Bak::CStringNode * pTemStringNode = m_pNode;
//	m_pNode->AppendBuffer(pBuffer,iBufferSize);
	for (;;)
	{
		CDataSet_StringBuilder_Bak::CStringNode * pTemNextStringNode = pTemStringNode->GetNextStringNode();
		if(NULL == pTemNextStringNode)
		{
			pTemStringNode->AppendBuffer(pBuffer,iBufferSize);
			break;
		}
		else
			pTemStringNode = pTemNextStringNode;
	}
}

void CDataSet_StringBuilder_Bak::AppendLine(LPCTSTR pStr)
{
	Append(pStr);
	Append(_T("\r\n"));
}

void CDataSet_StringBuilder_Bak::Append(CDataSet_StringBuilder_Bak * pStringBuilder)
{
	CDataSet_StringBuilder_Bak::CStringNode * pTemNode = NULL;
	pTemNode = pStringBuilder->GetStringNode();
	if(NULL == pTemNode)
		return;
	for (;;)
	{
		this->Append(pTemNode->GetBuffer(),pTemNode->GetUseSize());

		pTemNode = pTemNode->GetNextStringNode();
		if(NULL == pTemNode)
			break;

	}
}


//返回CString
void CDataSet_StringBuilder_Bak::GetString(CString& str)
{
	size_t lStringSize = GetLength();
	if (lStringSize >0)
	{
		LPTSTR buffer = str.GetBufferSetLength((int)lStringSize);
		CDataSet_StringBuilder_Bak::CStringNode * pTemStringNode = m_pNode;
		if(NULL == pTemStringNode)
			return;

		size_t iCurPos = 0;
		for(;;)
		{
			BYTE * pNodeBuffer = pTemStringNode->GetBuffer();
			size_t iNodeSize = pTemStringNode->GetUseSize() * sizeof(TCHAR);
			memcpy( (BYTE*)buffer + iCurPos,pNodeBuffer,iNodeSize);
		
			iCurPos += iNodeSize;

			pTemStringNode = pTemStringNode->GetNextStringNode();
			if(NULL == pTemStringNode)
				break;
		}
	}
}

//返回字符个数
size_t CDataSet_StringBuilder_Bak::GetLength()
{
	size_t iStringSize = 0;
	CDataSet_StringBuilder_Bak::CStringNode * pTemStringNode = m_pNode;
	if(NULL == pTemStringNode)
		return iStringSize;

	for (;;)
	{
		iStringSize += pTemStringNode->GetUseSize();

		pTemStringNode = pTemStringNode->GetNextStringNode();
		if(NULL == pTemStringNode)
			break;	
	}
	return iStringSize;
}

//清除
void CDataSet_StringBuilder_Bak::Empty()
{
	if(NULL != m_pNode)
		delete m_pNode;
	m_pNode = NULL;
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// 


CDataSet_StringBuilder::CDataSet_StringBuilder()
{

}

CDataSet_StringBuilder::~CDataSet_StringBuilder()
{

}



void	CDataSet_StringBuilder::Append(LPCTSTR pStr)
{
	this->append( pStr ) ; 
}

void	CDataSet_StringBuilder::Append( const CDataSet_StringBuilder & str )
{
	this->append( str ) ; 
}

void	CDataSet_StringBuilder::AppendLine(LPCTSTR pStr)
{
	Append(pStr);
	Append(_T("\r\n"));
}

void	CDataSet_StringBuilder::GetString(CString& str)
{
	str = this->c_str() ; 
}
void	CDataSet_StringBuilder::Empty() 
{
	clear() ; 
}

int		CDataSet_StringBuilder::GetLength()
{
	return (int)this->size() ;
}
