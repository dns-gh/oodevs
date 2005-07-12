//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ArmeListViewItem.inl $
// $Author: Nld $
// $Modtime: 29/12/04 15:46 $
// $Revision: 4 $
// $Workfile: MOS_ArmeListViewItem.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_ArmeListViewItem::GetFirstAutomate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
inline
QListViewItem* MOS_ArmeListViewItem::GetFirstAutomate() const
{
    if ( automateVector_.size() )
        return *automateVector_.begin();
    return 0;
}





//-----------------------------------------------------------------------------
// Name: MOS_ArmeListViewItem::UnregisterAutomate
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
inline
void MOS_ArmeListViewItem::UnregisterAutomate( const QListViewItem& automate )
{
    IT_AutomatePtrVector itAutomate = std::find( automateVector_.begin() , automateVector_.end() , &automate );
    assert( itAutomate != automateVector_.end() );
    automateVector_.erase( itAutomate );
}


