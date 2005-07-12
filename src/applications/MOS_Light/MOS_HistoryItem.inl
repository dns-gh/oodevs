//*****************************************************************************
//
// $Created: FBD 03-08-25 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_HistoryItem.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 2 $
// $Workfile: MOS_HistoryItem.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: MOS_HistoryItem::GetAgent
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
inline
const MOS_Agent& MOS_HistoryItem::GetAgent() const
{
    assert( pAgent_ );
    return *pAgent_;
}


//-----------------------------------------------------------------------------
// Name: MOS_HistoryItem::GetWidth
// Created: FBD 03-08-26
//-----------------------------------------------------------------------------
inline
uint MOS_HistoryItem::GetWidth() const
{
    return pixmap().width();
}

