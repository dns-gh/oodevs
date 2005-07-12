//*****************************************************************************
// 
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_PointListView.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 7 $
// $Workfile: MOS_PointListView.inl $
// 
//*****************************************************************************
//-----------------------------------------------------------------------------
// Name: MOS_PointListView::GetType
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
inline
E_ListViewType MOS_PointListView::GetType() const
{
    return nType_;
}

//-----------------------------------------------------------------------------
// Name: MOS_PointListView::RegisterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
inline
void MOS_PointListView::RegisterPoint( MOS_PointListViewItem& point, MOS_PointListViewItem* pPointAfter )
{
    if( pPointAfter )
    {
        for( IT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
        {
            if( (MOS_PointListViewItem*)(*itPoint) == pPointAfter )
                break;
        }

        assert( itPoint != pointVector_.end() );
        if( *itPoint != *pointVector_.rbegin() )
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
// Name: MOS_PointListView::UnregisterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
inline
void MOS_PointListView::UnregisterPoint( const MOS_PointListViewItem& point )
{
    IT_PointPtrVector itPoint = std::find( pointVector_.begin() , pointVector_.end() , &point );
    assert( itPoint != pointVector_.end() );
    pointVector_.erase( itPoint );
}


//-----------------------------------------------------------------------------
// Name: MOS_PointListView::DeletePointFromInterface
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
inline
void MOS_PointListView::DeletePointFromInterface( MOS_PointListViewItem& point )
{
    DeletePoint( point );
}
