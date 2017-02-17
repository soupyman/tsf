
#include "formula.h"

CFormulaOperatorNode::CFormulaOperatorNode()
{
	type = OPERATOR;
}

CFormulaParameterNode::CFormulaParameterNode()
{
	type = PARAMETER;
}


CFormulaNumeralNode::CFormulaNumeralNode()
{
	type = NUMERAL;
}


CFormula::CFormula(CFormulaNode * pNode)
	:CTree(pNode , NULL ,  NULL ,  NULL )
{

}

#if 0
bool	CFormula::Calculate(CContentTree * start , u64 & Ret)
{
	CFormulaNode * ptn = (CFormulaNode *)GetMyNode();
	switch(ptn->type)
	{
		case CFormulaNode::OPERATOR:
		{
			u64 Value1= 0, Value2 = 0;
			CFormula * pFml = (CFormula *)GetChildren();
			CFormulaOperatorNode * pFON  = (CFormulaOperatorNode *)ptn;

			if((!pFml)||(false == pFml->Calculate( start, Value1)))
					return false;

			pFml =  (CFormula *)(pFml->GetRightBrother());

			if((!pFml)||(false == pFml->Calculate( start, Value2)))
					return false;
					
			/*  formula is a two son tree. */
			switch(pFON->op)
			{
				case CFormulaOperatorNode::PLUS:
					Ret = Value1 + Value2;
					break;
				case CFormulaOperatorNode::MINUS:
					ASSERT(Value1 >= Value2);
					Ret = Value1 - Value2;
					break;
				case CFormulaOperatorNode::MULTIPLY:
					Ret = Value1 * Value2;
					break;
				case CFormulaOperatorNode::DIVIDE:
					Ret = Value1 / Value2;
					break;
				case CFormulaOperatorNode::EQUAL:
					Ret = (Value1 == Value2);
					break;
				default:
					ASSERT(0);
					break;
			}
		}
		break;
		case CFormulaNode::PARAMETER:
		{
			CFormulaParameterNode * pFPN = (CFormulaParameterNode *)ptn;
			if(false == (start->SearchBackFieldValue(&(pFPN->param) ,Ret)))
				return false;
		}
		break;
		case CFormulaNode::NUMERAL:
		{
			CFormulaNumeralNode * pFNN = (CFormulaNumeralNode *)ptn;	
			Ret = pFNN->num;
		}
		break;
	}
	return true;
}

bool	CFormula::IsValid(CFormatTree * start )
{
	CFormulaNode * ptn = (CFormulaNode *)GetMyNode();
	switch(ptn->type)
	{
		case CFormulaNode::OPERATOR:
		{
			CFormula * pFml = (CFormula *)GetChildren();								
			while(pFml)
			{
				if(false == pFml->IsValid( start))
					return false;

				pFml = (CFormula *)(pFml->GetRightBrother());
			}
		}
		break;
		case CFormulaNode::PARAMETER:
		{
			CFormulaParameterNode * pFPN = (CFormulaParameterNode *)ptn;
			if(false == (start->SearchBackField(&(pFPN->param))))
				return false;
		}
		break;
		case CFormulaNode::NUMERAL:
		{
			CFormulaNumeralNode * pFNN = (CFormulaNumeralNode *)ptn;
			if(pFNN->num == 0)
			{
				return false;/* number 0 is meanless .*/
			}
		}
		break;
	}
	return true;
}
#endif
bool CFormula::DumpToString(CString & TraceStr)
{
	CFormulaNode * ptn = (CFormulaNode *)GetMyNode();
	switch(ptn->type)
	{
		case CFormulaNode::OPERATOR:
			{
				CFormula * pFml = (CFormula *)GetChildren();
				CFormulaOperatorNode * pFON ;
				pFON = (CFormulaOperatorNode *)ptn;

					
				if(pFml)
				{
					pFml->DumpToString( TraceStr);
					while(pFml->GetRightBrother())
					{
						switch(pFON->op)
						{
							case CFormulaOperatorNode::PLUS:
								TraceStr.AppendFormat(" + ");
								break;
							case CFormulaOperatorNode::MINUS:
								TraceStr.AppendFormat(" - ");
								break;
							case CFormulaOperatorNode::MULTIPLY:
								TraceStr.AppendFormat(" * ");
								break;
							case CFormulaOperatorNode::DIVIDE:
								TraceStr.AppendFormat(" / ");
								break;
							case CFormulaOperatorNode::EQUAL:
								TraceStr.AppendFormat(" = ");
								break;
							default:
								break;
						}
						pFml = (CFormula *)(pFml->GetRightBrother());
						ptn = (CFormulaNode *)pFml->GetMyNode();

						if(ptn->type == CFormulaNode::OPERATOR)
						{/*second son is operator mean () is used for change priority */
							TraceStr.AppendFormat(" ( ");
							pFml->DumpToString( TraceStr);
							TraceStr.AppendFormat(" ) ");
						}
						else
						{
							pFml->DumpToString( TraceStr);
						}
					}
				}
				else
					ASSERT(0);
			}
			break;
		case CFormulaNode::PARAMETER:
			{
				CFormulaParameterNode * pFPN = (CFormulaParameterNode *)ptn;
				TraceStr.Append(pFPN->param);
			}
			break;
		case CFormulaNode::NUMERAL:
			{
				CFormulaNumeralNode * pFNN = (CFormulaNumeralNode *)ptn;
				TraceStr.AppendFormat("%d",pFNN->num);
			}
			break;
	}
	return true;
}
