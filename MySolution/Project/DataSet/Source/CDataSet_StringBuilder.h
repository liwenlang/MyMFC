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
���������������ַ�����StringBuffer��
������
�� �� ֵ��
�㷨�����ַ���������һ��Buffer�� ���в�������������ʽ���п��ƣ�ÿ���ڵ㶨���б���m_NodeSize���ơ�
ע������ڲ�ʹ��Sizeȫ��ʹ��TCHAR��
��    �ߣ�liwenlang
ʱ    �䣺[2010:11:8]   9:50
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

	CStringNode * m_pNode;//�ַ����ڵ�
	size_t		m_iNodeSize;//�ڵ㳤�ȣ��ڹ���ʱָ��
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// 
/// �����ԣ�std::string ��ƴ��Ч�ʺܸߣ�����ʹ��
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
