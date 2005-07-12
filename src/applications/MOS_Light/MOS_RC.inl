//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RC.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 8 $
// $Workfile: MOS_RC.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_RC::GetPosition
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_RC::GetPos() const
{
    return vPos_;
}


//-----------------------------------------------------------------------------
// Name: MOS_RC::GetTime
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
inline
int MOS_RC::GetTime() const
{
    return nTime_;
}


//-----------------------------------------------------------------------------
// Name: MOS_RC::GetAgent
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
inline
MOS_Agent& MOS_RC::GetAgent()
{
    return agent_;
}


//-----------------------------------------------------------------------------
// Name: MOS_RC::SetRCItem
// Created: AGN 03-06-03
//-----------------------------------------------------------------------------
inline
void MOS_RC::SetRCItem( MOS_RCListViewItem* pItem )
{
    pItem_ = pItem;
}


//-----------------------------------------------------------------------------
// Name: MOS_RC::GetTextFields
// Created: NLD 2003-06-05
//-----------------------------------------------------------------------------
inline
const T_StringVector& MOS_RC::GetTextFields() const
{
    return textVector_;
}


//-----------------------------------------------------------------------------
// Name: MOS_RC::GetTitle
// Created: NLD 2003-06-05
//-----------------------------------------------------------------------------
inline
const std::string& MOS_RC::GetTitle() const
{
    return strTitle_;
}


//-----------------------------------------------------------------------------
// Name: MOS_RC::GetOrderConduites
// Created: NLD 2003-06-05
//-----------------------------------------------------------------------------
inline
const MOS_RC::T_OrderConduiteVector& MOS_RC::GetOrderConduites()const
{
    return conduiteOrderVector_;
}
