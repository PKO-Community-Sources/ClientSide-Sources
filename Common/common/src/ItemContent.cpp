//=============================================================================
// FileName: ItemContent.cpp
// Creater: ZhangXuedong
// Date: 2005.10.19
// Comment: Item Content class
//=============================================================================

#include "ItemRecord.h"
#include "ItemContent.h"

bool SItemGrid::InitAttr(void)
{
	USHORT sTempID = sID;

	CItemRecord* pItem = GetItemRecordInfo(sTempID);
	if(pItem)
	{
		if(CItemRecord::IsVaildFusionID(pItem))
		{
			//LG("XXXError", "%d %d %d \r\n",sTempID, sEndure[1], (short)CItemRecord::enumItemFusionEndure);
			sEndure[1] = (short)CItemRecord::enumItemFusionEndure;
		}
	}
	// End

	//if( sTempID >= CItemRecord::enumItemFusionStart && sTempID < CItemRecord::enumItemFusionEnd && GetFusionItemID() )
	if ( sEndure[1] == CItemRecord::enumItemFusionEndure && GetFusionItemID()) // modify by ning.yan 20080821
	{
		sTempID = GetFusionItemID();
	}

	if (!CAttr.Init(sTempID))
		return false;
	SetAttr(ITEMATTR_URE, sEndure[0]);
	SetAttr(ITEMATTR_MAXURE, sEndure[1]);
	SetAttr(ITEMATTR_ENERGY, sEnergy[0]);
	SetAttr(ITEMATTR_MAXENERGY, sEnergy[1]);
	SetAttr(ITEMATTR_FORGE, chForgeLv);
	for (char i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++)
	{
		if (sInstAttr[i][0] == 0)
			break;
		SetAttr(sInstAttr[i][0], sInstAttr[i][1]);
	}

	return true;
}

void SItemGrid::CheckValid(void)
{
	if (sID <= 0)
	{
		bValid = false;
		return;
	}
	if (!CItemRecordSet::I())
	{
		bValid = false;
		return;
	}
	CItemRecord *pCItemRec = GetItemRecordInfo(sID);
	if (!pCItemRec)
	{
		bValid = false;
		return;
	}

	bValid = true;
	SetChange();
}

bool SItemGrid::FusionCheck( SItemGrid& item )
{
	CItemRecord* pRec1 = GetItemRecordInfo( this->sID );
	CItemRecord* pRec2 = GetItemRecordInfo( item.sID );
	if( !pRec1 || !pRec2 ) {
		return false;
	}

	bool bFlag = false;
	if( !pRec1->IsAllEquip() && !pRec2->IsAllEquip() )
	{
		for( int i = 1; i <= defITEM_BODY; ++i )
		{
			if( pRec1->IsAllowEquip( i ) && pRec2->IsAllowEquip( i ) )
			{
				bFlag = true;
				break;
			}
		}	
		if( !bFlag ) return false;
	}

	for( int i = 0; i < MAX_JOB_TYPE; ++i )
	{
		if( pRec1->szWork[i] == cchItemRecordKeyValue ) {
			break;
		}
		if( pRec1->szWork[i] == char(-1) ) {
			return true;
		}

		for( char j = 0; j < MAX_JOB_TYPE; ++j )
		{
			if( pRec2->szWork[j] == cchItemRecordKeyValue ) {
				break;
			}
			if( pRec2->szWork[j] == char(-1) ) {
				return true;
			}
			if( pRec2->szWork[j] == pRec1->szWork[i] ) {
				return true;
			}
		}
	}
	return false;
}




