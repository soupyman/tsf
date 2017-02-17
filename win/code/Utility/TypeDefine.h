#pragma once

#define DATAVIEWBUFFERLENTH		8192 /* (1024*8)*/
#define INVALID_VERSION				0xFF /* */

#define VALUE_MAX_LENGTH_IN_BYTE   8

#define UI_CLIENT_ID				0xFFFFFFFF

//#define     SYSTEM_WITH_ES_OPERATORION

typedef unsigned char                           	u8;
typedef unsigned short                          	u16;
typedef unsigned long                            u32;
typedef int	                           		 s32;
typedef unsigned __int64                        u64;
typedef  __int64                                    s64;


/*	Utility	*/
class CDVBSpace;
class CMessage;
class CMessageSpace;
class CThreadObject;
//class CShareResource;

/*	TS File	*/
class CTSFile;

/* 	ES server 	*/
class CPacket;
class CPacketPool;
class CDemux;
class CESData;
class CESDBServer;
class CESDataBase;

/*	Table server 	*/
class CBuilder;
class CSection;
class CTable;
class CTableDBServer;
class CTableDatabase;


/*	Format	*/
class CFormatServer;
class CFormatDatabase;
class CFormatParser;

class CTokenNode;
class CTableHeadNode;
class CDescriptorHeadNode;
class CDataNode;
class CKeyLoopNode;
class CKeyIfNode;
class CKeyElseNode;
class CKeyDescriptorNode;
/* 	trees	*/
class CTreeNode;
class CCommonTree;
class CContentTree;
class CFormatTree;
/*	formula	*/
class CFormula;
class CFormulaOperatorNode;
class CFormulaParameterNode;
class CFormulaNumeralNode;


/*      UI  classes         */
class CTSDoc;

