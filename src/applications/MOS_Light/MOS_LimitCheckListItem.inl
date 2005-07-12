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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimitCheckListItem.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 4 $
// $Workfile: MOS_LimitCheckListItem.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem::GetLimit
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
MOS_Limit& MOS_LimitCheckListItem::GetLimit() const
{
    return Limit_;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem::GetFirstPoint
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
inline
MOS_PointListViewItem* MOS_LimitCheckListItem::GetFirstPoint() const
{
    if ( pointVector_.size() )
        return *pointVector_.begin();
    return 0;
}


//-----------------------------------------------------------------------------
// Name: std::MOS_LimitCheckListItem::GetName
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
const std::string MOS_LimitCheckListItem::GetName() const
{
    return text(0);
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem::RegisterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
inline
void MOS_LimitCheckListItem::RegisterPoint( MOS_PointListViewItem& point, MOS_PointListViewItem* pPointAfter )
{
    if ( pPointAfter )
    {
        for ( IT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
        {
            if ( &(*itPoint)->GetPoint() == &pPointAfter->GetPoint() )
                break;
        }

        assert( itPoint != pointVector_.end() );
        if ( *itPoint != *pointVector_.rbegin() )
        {
            ++itPoint;
            pointVector_.insert( itPoint, &point );
        }
        else
            pointVector_.push_back( &point );
    }
    else
        pointVector_.push_back( &point );
}
//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem::UnregisterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
inline
void MOS_LimitCheckListItem::UnregisterPoint( const MOS_PointListViewItem& point )
{
    IT_PointPtrVector itPoint = std::find( pointVector_.begin() , pointVector_.end() , &point );
    assert( itPoint != pointVector_.end() );
    pointVector_.erase( itPoint );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem::GetPoint
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
inline
MOS_PointListViewItem* MOS_LimitCheckListItem::GetPoint( const MT_Vector2D& point )
{
   
    for ( IT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        if ( &(*itPoint)->GetPoint() == &point  )
            break;
    }
    if ( itPoint != pointVector_.end() )
    {
        return (*itPoint);
    }
    return 0;
}
