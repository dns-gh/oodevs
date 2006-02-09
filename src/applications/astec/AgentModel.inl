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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentModel.inl $
// $Author: Ape $
// $Modtime: 25/10/04 10:45 $
// $Revision: 7 $
// $Workfile: AgentModel.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: AgentModel::GetName
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
inline
const std::string& AgentModel::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: AgentModel::GetDIAType
// Created: APE 2004-09-06
// -----------------------------------------------------------------------------
inline
const std::string& AgentModel::GetDIAType() const
{
    return strDIAType_;
}


// -----------------------------------------------------------------------------
// Name: AgentModel::GetAvailableMissions
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
inline
const AgentModel::T_MissionVector& AgentModel::GetAvailableMissions() const
{
    return vAvailableMissions_;
}


// -----------------------------------------------------------------------------
// Name: AgentModel::GetAvailableFragOrders
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
inline
const AgentModel::T_FragOrderSet& AgentModel::GetAvailableFragOrders() const
{
    return fragmentaryOrders_;
}
