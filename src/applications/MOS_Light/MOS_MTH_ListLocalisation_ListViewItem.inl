//*****************************************************************************
//
// $Created: AGN 03-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_ListLocalisation_ListViewItem.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 2 $
// $Workfile: MOS_MTH_ListLocalisation_ListViewItem.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListViewItem::GetLocalisation
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
inline
const T_PointVector& MOS_MTH_ListLocalisation_ListViewItem::GetLocalisation()
{
    return vLocalisationPoint_;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListViewItem::GetType
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
inline
ASN1T_EnumTypeLocalisation MOS_MTH_ListLocalisation_ListViewItem::GetType() const
{
    return nTypeLocalisation_;
}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListViewItem::SetType
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
inline
void MOS_MTH_ListLocalisation_ListViewItem::SetType( ASN1T_EnumTypeLocalisation nType )
{
    nTypeLocalisation_ = nType;
}

    

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListViewItem::ClearList
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
inline
void MOS_MTH_ListLocalisation_ListViewItem::ClearList()
{
    vLocalisationPoint_.clear();
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListViewItem::AddPoint
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
inline
void MOS_MTH_ListLocalisation_ListViewItem::AddPoint( const MT_Vector2D& point )
{
    vLocalisationPoint_.push_back( point );
}

