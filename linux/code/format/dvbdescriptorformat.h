#pragma once


#include "formatparser.h"
#include "contenttree.h"



class CDVBDescriptor : public virtual CFormatParser
{
public:
	
	CDVBDescriptor(void);
	~CDVBDescriptor(void);
	/*	described on page 25 chinese SI	*/
	enum DVBDESCRIPTOR_TAG
	{
	/* dvb-si descriptor id  define */
	      DESCRIPTOR_NETWORK_NAME                       =     0x40,
	      DESCRIPTOR_SERVICE_LIST                       =     0x41,
	      DESCRIPTOR_STUFFING                           =     0x42,
	      DESCRIPTOR_SATELLITE_DELIVERY_SYSTEM          =     0x43,
	      DESCRIPTOR_CABLE_DELIVERY_SYSTEM              =     0x44,
	      DESCRIPTOR_VBI_DATA                           =     0x45,
	      DESCRIPTOR_VBI_TELETEXT                       =     0x46,
	      DESCRIPTOR_BOUQUET_NAME                       =     0x47,
	      DESCRIPTOR_SERVICE                            =     0x48,
	      DESCRIPTOR_COUNTRY_AVAILABILITY               =     0x49,
	      DESCRIPTOR_LINKAGE                            =     0x4a,
	      DESCRIPTOR_NVOD_REFERENCE                     =     0x4b,
	      DESCRIPTOR_TIME_SHIFTED_SERVICE               =     0x4c,
	      DESCRIPTOR_SHORT_EVENT                        =     0x4d,
	      DESCRIPTOR_EXTENDED_EVENT                     =     0x4e,
	      DESCRIPTOR_TIME_SHIFTED_EVENT                 =     0x4f,
	      DESCRIPTOR_COMPONENT                          =     0x50,
	      DESCRIPTOR_MOSAIC                             =     0x51,
	      DESCRIPTOR_STREAM_IDENTIFIER                  =     0x52,
	      DESCRIPTOR_CA_IDENTIFIER                      =     0x53,
	      DESCRIPTOR_CONTENT                            =     0x54,
	      DESCRIPTOR_PARENTAL_RATING                    =     0x55,
	      DESCRIPTOR_TELETEXT                           =     0x56,
	      DESCRIPTOR_TELEPHONE                          =     0x57,
	      DESCRIPTOR_LOCAL_TIME_OFFSET                  =     0x58,
	      DESCRIPTOR_SUBTITLING                         =     0x59,
	      DESCRIPTOR_TERRESTRIAL_DELIVERY_SYSTEM        =     0x5a,
	      DESCRIPTOR_MULTILINGUAL_NETWORK_NAME          =     0x5b,
	      DESCRIPTOR_MULTILINGUAL_BOUQUET_NAME          =     0x5c,
	      DESCRIPTOR_MULTILINGUAL_SERVICE_NAME          =     0x5d,
	      DESCRIPTOR_MULTILINGUAL_COMPONENT             =     0x5e,
	      DESCRIPTOR_PRIVATE_DATA_SPECIFIER             =     0x5f,
	      DESCRIPTOR_SERVICE_MOVE                       =     0x60,
	      DESCRIPTOR_SHORT_SMOOTHING_BUFFER             =     0x61,
	      DESCRIPTOR_FREQUENCY_LIST                     =     0x62,
	      DESCRIPTOR_PARTIAL_TRANSPORT_STREAM           =     0x63,
	      DESCRIPTOR_DATA_BROADCAST                     =     0x64,
	      DESCRIPTOR_CA_SYSTEM                          =     0x65,
	      DESCRIPTOR_DATA_BROADCAST_ID                  =     0x66,
	      DESCRIPTOR_TRANSPORT_STREAM                   =     0x67,
	      DESCRIPTOR_DSNG                               =     0x68,
	      DESCRIPTOR_PDC                                =     0x69,
	      DESCRIPTOR_AC_3                               =     0x6a,
	      DESCRIPTOR_ANCILLARY_DATA                     =     0x6b,
	      DESCRIPTOR_CELL_LIST                          =     0x6c,
	      DESCRIPTOR_CELL_FREQUENCY_LINK                =     0x6d,
	      DESCRIPTOR_ANNOUNCEMENT_SUPPORT               =     0x6e

	/* 0x6f 	- 0x7f are reserved 		*/
	/* 0x80 	- 0xfe are user defined 		*/
	};
	
protected:
	u32  ParseDVBDescriptorContent	(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	
private:
	void ParseDescriptorAnciliaryData(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorAnnouncementSupport(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorBouquetName(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorCAIdentifier(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorCellFrequencyLine(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorCellList(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorComponent(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorContent(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorCountryAvailability(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorDataBroadcast(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorDataBroadcastID(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorCableDeliverySystem(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorSatelliteDeliverySystem(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorTerrestrialDeliverySystem(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorDSNG(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorExtendedEvent(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorFrequencyList(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorDefault(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorLinkage(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorLocalTimeOffset(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorMosaic(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorMultilingualBouquetName(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorMultilingualComponent(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorMultilingualNetworkName(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorMultilingualServiceName(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorNVODReference(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorNetworkName(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorParentalRating(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorPDC(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorPrivateDataSpecifier(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorShortSmoothingBuffer(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorService(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorServiceList(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorServiceMove(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorShortEvent(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorStreamIdentifier(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorStuffing(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorSubtitling(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorTeletext(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorTimeShiftedEvent(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorTimeShiftedService(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorTransportStream(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorVBIData(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorVBITeletext(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	void ParseDescriptorTelephone(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);

//private:
	//CContentTree *		Content;

};

