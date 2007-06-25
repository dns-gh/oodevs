// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

namespace dispatcher 
{

// -----------------------------------------------------------------------------
// Name: Dispatcher::GetModel
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
inline
Model& Dispatcher::GetModel() const
{
    return *pModel_;
}

// -----------------------------------------------------------------------------
// Name: Dispatcher::GetClientsNetworker
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
inline
ClientsNetworker& Dispatcher::GetClientsNetworker() const
{
    return *pClientsNetworker_;
}

// -----------------------------------------------------------------------------
// Name: Dispatcher::GetSimulationNetworker
// Created: NLD 2007-01-30
// -----------------------------------------------------------------------------
inline
SimulationNetworker& Dispatcher::GetSimulationNetworker() const
{
    return *pSimulationNetworker_;
}

// -----------------------------------------------------------------------------
// Name: Dispatcher::GetProfileManager
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
inline
ProfileManager& Dispatcher::GetProfileManager() const
{
    return *pProfileManager_;
}

}