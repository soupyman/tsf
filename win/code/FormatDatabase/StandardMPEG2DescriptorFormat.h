#pragma once

#include "FormatParser.h"
#include "typedefine.h"

class CMPEG2Descriptor : public virtual CFormatParser
{
public:
	CMPEG2Descriptor(void);
	~CMPEG2Descriptor(void);

protected:	
	u32  ParseMPEG2DescriptorContent(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	
private:
	void ParseDescriptorVideoStream(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorAudioStream(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorHierarchy(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorRegistration(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorDataStreamAlignment(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorCA(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorISO639Language(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorTargetBackgroundGrid(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorVideoWindow(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorSystemClock(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorMultiplexBufferUtilization(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorCopyright(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorMaximumBitrate(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorPricateDataIndicator(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorDefault(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);


//private:
	//CContentTree *		Content;

};

