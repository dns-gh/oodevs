//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_Localisation.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 3 $
// $Workfile: MOS_MTH_Localisation.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::SetFirstWidget
// Created: FBD 03-04-28
//-----------------------------------------------------------------------------
inline
void MOS_MTH_Localisation::SetFirstWidget( bool bFirstWidget )
{
    bFirstWidget_ = bFirstWidget;
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::GetListView
// Created: NLD 2003-08-12
// -----------------------------------------------------------------------------
inline
MOS_MTH_Localisation_ListView& MOS_MTH_Localisation::GetListView() const
{
    assert( pListView_ );
    return *pListView_;
}
