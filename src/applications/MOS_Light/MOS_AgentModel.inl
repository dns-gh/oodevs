// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-12-22 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentModel.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 3 $
// $Workfile: MOS_AgentModel.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_AgentModel::GetName
/** @return 
*/
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
inline
const std::string& MOS_AgentModel::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentModel::GetAvailableMissions
/** @return 
*/
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
inline
const MOS_AgentModel::T_Mission_Vector& MOS_AgentModel::GetAvailableMissions() const
{
    return vAvailableMissions_;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentModel::CanExecuteMission
/** @param  nId 
    @return 
*/
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
inline
bool MOS_AgentModel::CanExecuteMission( E_MissionID nId ) const
{
    return std::find( vAvailableMissions_.begin(), vAvailableMissions_.end(), nId ) != vAvailableMissions_.end();
}
