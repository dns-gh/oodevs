//*****************************************************************************
//
// $Created: AGN 03-04-14 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Mission_OC.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 4 $
// $Workfile: MOS_Mission_OC.inl $
//
//*****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::GetName
/** @return 
*/
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
inline
std::string MOS_Mission_OC::GetName() const
{
    return MOS_Tools::GetOrderDisplay( nTypeOC_ );
}


// -----------------------------------------------------------------------------
// Name: MOS_Mission_OC::IsAutomateMission
/** @return 
*/
// Created: APE 2004-03-11
// -----------------------------------------------------------------------------
inline
bool MOS_Mission_OC::IsAutomateMission() const
{
    return false;
}
