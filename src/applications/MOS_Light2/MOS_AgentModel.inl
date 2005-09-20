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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentModel.inl $
// $Author: Ape $
// $Modtime: 25/10/04 10:45 $
// $Revision: 7 $
// $Workfile: MOS_AgentModel.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_AgentModel::GetName
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
inline
const std::string& MOS_AgentModel::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentModel::GetDIAType
// Created: APE 2004-09-06
// -----------------------------------------------------------------------------
inline
const std::string& MOS_AgentModel::GetDIAType() const
{
    return strDIAType_;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentModel::GetAvailableMissions
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
inline
const MOS_AgentModel::T_MissionVector& MOS_AgentModel::GetAvailableMissions() const
{
    return vAvailableMissions_;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentModel::GetAvailableFragOrders
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
inline
const MOS_AgentModel::T_FragOrderSet& MOS_AgentModel::GetAvailableFragOrders() const
{
    return fragmentaryOrders_;
}
