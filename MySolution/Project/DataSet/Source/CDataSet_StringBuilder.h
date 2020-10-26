// CDataSet_StringBuilder_Bak.h: interface for the CStringBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CDataSet_StringBuilder_Bak_H__15A70F78_DABE_4A27_A956_EAB0014C94D9__INCLUDED_)
#define AFX_CDataSet_StringBuilder_Bak_H__15A70F78_DABE_4A27_A956_EAB0014C94D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define StringBuilder_NodeSize 4096

/******************************************************************** 
功能描述：超大字符串的StringBuffer。
参数：
返 回 值：
算法：将字符串不放在一个Buffer中 进行操作。以链表形式进行控制，每个节点定长有变量m_NodeSize控制。
注意事项：内部使用Size全部使用TCHAR。
作    者：liwenlang
时    间：[2010:11:8]   9:50
*********************************************************************/ 

#ifdef  UNICODE                    
#	ifndef _PM_SQLSTRING_DEFINED
/// 		
typedef	std::wstring	PmTSQLString;
#define _PM_SQLSTRING_DEFINED
#	endif
#else
#	ifndef _PM_SQLSTRING_DEFINED
typedef	std::string		PmTSQLString ;
#define _PM_SQLSTRING_DEFINED
#	endif /* !_TCHAR_DEFINED */
#endif /*UNICODE*/

class CDataSet_StringBuilder_Bak
{
public:
	CDataSet_StringBuilder_Bak();
	explicit CDataSet_StringBuilder_Bak(size_t iNodeSize);
	
	virtual ~CDataSet_StringBuilder_Bak();

	class CStringNode
	{
	public:
//		CStringNode();

		explicit CStringNode(const CStringNode & stringNode);

		explicit CStringNode(CDataSet_StringBuilder_Bak * pStringBuilder);
		
		virtual ~CStringNode();

		BYTE * GetBuffer();
		size_t GetUseSize()const
		{
			return m_iNodeUse;
		}

		void SetUseSize(size_t iUseSize){m_iNodeUse = iUseSize;}

		void AppendBuffer(LPVOID pBuffer,size_t iBufferSize);

		CStringNode * GetNextStringNode(){return m_pNextStringNode;}

	protected:
		BYTE * m_pMem;
		size_t m_iNodeUse;
		CStringNode * m_pNextStringNode;

		CDataSet_StringBuilder_Bak * m_pStringBuilder;
	};

	size_t GetNodeSize() const
	{
		return m_iNodeSize;
	}

	CStringNode * GetStringNode(){return m_pNode;}

	void	Append(LPCTSTR pStr);
	void	Append(BYTE * pBuffer,size_t iBufferSize);
	void	Append(CDataSet_StringBuilder_Bak * pStringBuilder);
	void	AppendLine(LPCTSTR pStr);
	void	GetString(CString& str);
	void	Empty();
	size_t	GetLength();

private:

	CStringNode * m_pNode;//字符串节点
	size_t		m_iNodeSize;//节点长度，在构造时指定
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// 
/// 经测试，std::string 的拼接效率很高，所以使用
/// 

class CDataSet_StringBuilder : public PmTSQLString
{
public:
	CDataSet_StringBuilder();
	virtual ~CDataSet_StringBuilder();

public:
	void	Append(LPCTSTR pStr);

	void	Append( const CDataSet_StringBuilder & str );

	void	AppendLine(LPCTSTR pStr);
	void	GetString(CString& str);
	void	Empty();
	int		GetLength();
};


#endif // !defined(AFX_CDataSet_StringBuilder_Bak_H__15A70F78_DABE_4A27_A956_EAB0014C94D9__INCLUDED_)
