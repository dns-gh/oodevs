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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AutomateListViewItem.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 5 $
// $Workfile: MOS_AutomateListViewItem.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_AutomateListViewItem::RegisterPion
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
inline
void MOS_AutomateListViewItem::RegisterPion( MOS_PionListViewItem& Pion, MOS_PionListViewItem* pPionAfter )
{
    if ( pPionAfter )
    {
        for ( IT_PionPtrVector itPion = PionVector_.begin() ; itPion != PionVector_.end() ; ++itPion )
        {
            if ( &(*itPion)->GetAgent() == &pPionAfter->GetAgent() )
                break;
        }

        assert( itPion != PionVector_.end() );
        if ( *itPion != *PionVector_.rbegin() )
        {
            ++itPion;
            PionVector_.insert( itPion, &Pion );
        }
        else
            PionVector_.push_back( &Pion );
    }
    else
        PionVector_.push_back( &Pion );
}
//-----------------------------------------------------------------------------
// Name: MOS_AutomateListViewItem::UnregisterPion
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
inline
void MOS_AutomateListViewItem::UnregisterPion( const MOS_PionListViewItem& Pion )
{
    IT_PionPtrVector itPion = std::find( PionVector_.begin() , PionVector_.end() , &Pion );
    assert( itPion != PionVector_.end() );
    PionVector_.erase( itPion );
}
