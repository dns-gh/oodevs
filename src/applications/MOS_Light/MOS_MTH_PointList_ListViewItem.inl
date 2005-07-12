//*****************************************************************************
//
// $Created: AGN 03-08-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_PointList_ListViewItem.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 2 $
// $Workfile: MOS_MTH_PointList_ListViewItem.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListViewItem::rtti
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
int MOS_MTH_PointList_ListViewItem::rtti() const
{
    return MOS_MTH_PointList_ListViewItem::e_rtti;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListViewItem::GetListPoint
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
const T_PointVector& MOS_MTH_PointList_ListViewItem::GetListPoint()
{
    return vLocalisationPoint_;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListViewItem::ClearList
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
void MOS_MTH_PointList_ListViewItem::ClearList()
{
    vLocalisationPoint_.clear();
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListViewItem::AddPoint
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
void MOS_MTH_PointList_ListViewItem::AddPoint( const MT_Vector2D& point )
{
    vLocalisationPoint_.push_back( point );
}

