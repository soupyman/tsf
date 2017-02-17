#pragma once

// CFormatFIle

/*

 
 		  format file define
 		
1.
"#" means comment, like \\ in C 
"@" means comment, same as #. but if followed by "include", it means include a other file.

2.
"@include file_name " means include another format file file_name.

3.
"<format type: format name: format ID> "
 format type can be table or descriptor 
 format name is a string like 'CAT' 
 format ID is tableID or descriptor tag 

4.
"{" means sub body start 
"}" means sub body end

5.
"[keyword] "
 keyword always in [] ,only keywork can be in []
 keyword can be 'loop' 'if' 'else' or 'descriptor' 

6.
"(formula) "
 currently formula support '+'-'*'/'=', and they all has same priority 
 formula can be nest,like (A+B-(C*D)-10) 
 Note: all the parameter A B C D must can be found in privious content 

7.
"[loop : name](formula) 
{ 
}" 
means it's a loop subbody, and the total loop lenth is calculate from formula 
loop key word is special, can have a name string,means what this loop is.
but name is optional,if there is no name, will consider subbody as brothers.
if is there is name string, will consider subbody as children of brothers with 'name string'.

8.
"[if](formula)
{ 
} 
[else] 
{ 
}" 
means if formula != 0 goto subbody first else goto subbody second; else key and subbody is optional.

9.
"[descriptor] "
means it's descriptor here. 

10.
"FieldName : lenth formula : Format "
FieldName is a string like 'section lenth', and can be empty if you want it not display on tree;
lenth is formula for calcuate the field's lenth in bit, now we only support <= 255;
Format is optional, can be 'b' 'd' 'x' 'c' 'm' or 's';
'b' means binnary, 'd' means decimal, 'x' means hex, 'c' means BCD code, 'm' MJD code, 's' means string;
the default is 'x'.

10.
 example 

@include PAT.pff
<table:CAT:1>

{
		table_id			:8

		section_syntax_indicator	:1		
		 :3 #		:1			
		 #				:2		
		section_length		:12
		
		 				:18
		
	 	version_number			:5		
	 	current_next_indicator	:1
	
	 	section_number			:8
	 	last_section_number		:8

	[loop](section_length - 9)
	 {
		[descriptor]					
	}

	CRC_32								:32

};
 
<descriptor:linkage descriptor:74>

{
     	descriptor_tag                       :8
     	descriptor_length                    :8
     	ts_id : 16
	onid : 16
	sid:16
	linkage_type:8
	[if](linkage_type = 154)#154
	{
		service_key	:16
		ls_replacement: 1
		use_sevice_name:1
		:6

		service_name : 8*(descriptor_length - 10) :s

	}

	[else]

	{
		private_data: (descriptor_length - 7)*8
	}
}

*/
#include "FormatDatabase.h"
#include "typedefine.h"

enum SUB_PASE_PHASE
{
	SEARCH_START,
	SEARCH_END
};


class CFormatDefineFile : public CStdioFile
{
public:
	CFormatDefineFile();
	virtual ~CFormatDefineFile();
	bool UpdateFormatToDB(CFormatDatabase & pFDB/* update format to format db */);

	CString ErrorInfo;
    
private:
      CString LastLine;
	bool ReadNextLine(CString & pNl);
      bool ReadIncludeFile(CString & pFileName);

	bool ParseKeyNode(CString & CurLine, CFormatTree * Father);
	bool ParseKeyIfElseNode(CString & CurLine, CFormatTree * Father);
	bool ParseKeyLoopNode(CString & LengthStr, CFormatTree * Father , CString name);
	bool ParseKeyDescriptorNode( CFormatTree * Father);
	bool ParseDataNode(CString & CurLine, CFormatTree * Father);
	CFormula * ParseFormula(CString  FmlStr );
	CFormatTree *  ParseHead(CString & pHdl);
   //   int   LastFmlStrInsideBracket(CString & FatherFmlStr ,CString & SonFmlStr );
      bool CheckAndTrimFmlStr(CString & FmlStr );
      s32 FindLastOpInTopLevel(CString  FmlStr );

	bool ParseSubBody(CFormatTree * pFather );
      bool ParseInclude(CFormatDatabase & pFDB);

};


