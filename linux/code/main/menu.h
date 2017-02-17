#pragma once

bool ExtractSiPsiToXml(void *pParam);
bool RemuxSiPsiFromXml(void *pParam);
bool FormatDbOpr(void *pParam);
bool ToolsOpr(void *pParam);

bool FormatDbDumpOpr(void *pParam);
bool FormatDbLoadOpr(void *pParam);

bool ToolsCalBitrateOpr(void *pParam);
bool ToolsParsePktOpr(void *pParam);
bool ToolsExtractEsOpr(void *pParam);
bool ToolsChangePidOpr(void *pParam);
bool ToolsDefragTsOpr(void *pParam);
bool ToolsSplitFileOpr(void *pParam);

bool ExtractPat(void *pParam);
bool ExtractCat(void *pParam);
bool ExtractPmt(void *pParam);
bool ExtractTsdt(void *pParam);
bool ExtractNit_a(void *pParam);
bool ExtractNit_o(void *pParam);
bool ExtractSdt_a(void *pParam);
bool ExtractSdt_o(void *pParam);
bool ExtractBat(void *pParam);
bool ExtractEIT_pf_a(void *pParam);
bool ExtractEIT_pf_o(void *pParam);
bool ExtractTdt(void *pParam);
bool ExtractRsit(void *pParam);
bool ExtractTot(void *pParam);
bool ExtractDit(void *pParam);
bool ExtractSit(void *pParam);
bool ExtractEcm_e(void *pParam);
bool ExtractEcm_o(void *pParam);
bool ExtractEmm(void *pParam);
bool ExtractOtherTable(void *pParam);

typedef bool (* pMenuOper)(void *pParam);

typedef struct
{
	pMenuOper oper;
	s8 name[100];
} MenuItem_s;

typedef struct _Menu
{
	_Menu *father;
	s8 Title[100];
	MenuItem_s Item[30];
} Menu_s;

Menu_s MainMenu =
{
	NULL,
	"what u want for the stream?",
	{
		{
			ExtractSiPsiToXml,
			"Extract SI PSI",
		},
		{
			RemuxSiPsiFromXml,
			"Insert SI PSI",
		},
		{
			FormatDbOpr,
			"Format",
		},
		{
			ToolsOpr,
			"Tools",
		},
            {
			NULL,
			"END",
		}
	}
};

Menu_s ToolsMenu =
{
	&MainMenu,
	"what u want?",
	{
		{
			ToolsDefragTsOpr,
			"Defrag TS",
		},
		{
			ToolsChangePidOpr,
			"Change PID",
		},
		{
			ToolsExtractEsOpr,
			"Extract ES",
		},
		{
			ToolsParsePktOpr,
			"Parse packet",
		},
		{
			ToolsCalBitrateOpr,
			"Calcuate bitrate",
		},
		{
			ToolsSplitFileOpr,
			"Split file",
		},
            {
			NULL,
			"END",
		}
	}
};
Menu_s FormatMenu =
{
	&MainMenu,
	"what u want?",
	{
		{
			FormatDbDumpOpr,
			"Dump Format DB",
		},
		{
			FormatDbLoadOpr,
			"Load Format File",
		},
            {
			NULL,
			"END",
		}
	}
};

Menu_s ExtractSipsiMenu =
{
	&MainMenu,
	"what table u want to extract?",
	{
		{
			ExtractPat,
			"PAT",
		},
		{
			ExtractCat,
			"CAT",
		},
		{
			ExtractPmt,
			"PMT",
		},
		{
			ExtractTsdt,
			"TSDT",
		},
		{
			ExtractNit_a,
			"NIT_A",
		},
		{
			ExtractNit_o,
			"NIT_O",
		},
		{
			ExtractSdt_a,
			"SDT_A",
		},
		{
			ExtractSdt_o,
			"SDT_O",
		},
		{
			ExtractBat,
			"BAT",
		},
		{
			ExtractEIT_pf_a,
			"EIT_PF_A",
		},
        	{
			ExtractEIT_pf_o,
			"EIT_PF_O",
		},
		{
			ExtractTdt,
			"TDT",
		},
		{
			ExtractRsit,
			"RSIT",
		},
		{
			ExtractTot,
			"TOT",
		},
		{
			ExtractDit,
			"DIT",
		},
		{
			ExtractSit,
			"SIT",
		},
		{
			ExtractEcm_e,
			"ECM_EVEN",
		},
		{
			ExtractEcm_o,
			"ECM_ODD",
		},
		{
			ExtractEmm,
			"EMM",
		},
		{
			ExtractOtherTable,
			"Other",
		},
            {
			NULL,
			"END",
		}
	}
};

