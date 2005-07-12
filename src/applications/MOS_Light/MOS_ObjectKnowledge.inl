//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ObjectKnowledge.inl $
// $Author: Nld $
// $Modtime: 25/03/04 19:52 $
// $Revision: 7 $
// $Workfile: MOS_ObjectKnowledge.inl $
//
//*****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetID
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
uint MOS_ObjectKnowledge::GetID() const
{
    return nID_;    
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::IsUpdated
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
bool MOS_ObjectKnowledge::IsValid( E_AttributeUpdated nAttr ) const
{
    return nAttrUpdated_ & nAttr;
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetCenter
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_ObjectKnowledge::GetCenter() const
{
    return vCenter_;    
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetRealObject
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
MOS_DynaObject* MOS_ObjectKnowledge::GetRealObject() const
{
    return pRealObject_;    
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetListViewItem
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
MOS_ObjectKnowledge_ListView_Item* MOS_ObjectKnowledge::GetListViewItem() const
{
    return pListViewItem_;    
}


