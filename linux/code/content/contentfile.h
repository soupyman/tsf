#ifndef CONTENTFILE_H
#define CONTENTFILE_H
/*	portable include  */
#include "typedefine.h"
#include "xmlfile.h"
#include "contenttree.h"
#include "formatparser.h"



class CContentFile : public CXmlFile , public CFormatParser
{
		/*          node type string define        */
#define STRING_TABLE 	"TABLE"
#define STRING_SECTION 	"SECTION"
#define STRING_PACKET 	"PACKET"
#define STRING_FIELD 	"FIELD"
#define STRING_FILE		"FILE"
#define STRING_SUBBODY "SUBBODY"
#define STRING_RESERVE  "RESERVE"
		/*          attr type string define        */
#define STRING_PID       	"pid"
#define STRING_NAME 	"name"
#define STRING_LENTH 	"lenth"
#define STRING_FORMAT 	"format"

		/*          format short name   */
#define STRING_BIN 	    "bin"
#define STRING_DEC 	    "dec"
#define STRING_HEX      "hex"
#define STRING_BCD 	    "bcd"
#define STRING_MJD 	    "mjd"
#define STRING_STR      "str"

	public:
		CContentTree   *pCTree; /* total tree */
	private:
		CContentTree   *pCurTree;/* a temp tree point for parse */
		bool 		bFileNode;

	public:
		CContentFile(const s8 *pFileName);
		~CContentFile(void);

		void UserStartValue(const char *el, const char **attr);
		void UserEndValue(const char *el);
		void UserInitBeforeLoad(void) ;

	private:
		void InitCntTree();
		void ClearCntTree();
		void AddCntTreeBranch(const char *el, const char **attr);
		void SetCurrentValue(s8 *s, u32 len);

};

#endif
