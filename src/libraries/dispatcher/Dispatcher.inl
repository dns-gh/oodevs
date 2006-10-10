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
    assert( pModel_ );
    return *pModel_;
}

// -----------------------------------------------------------------------------
// Name: Dispatcher::GetClientsNetworker
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
inline
ClientsNetworker& Dispatcher::GetClientsNetworker() const
{
    assert( pClientsNetworker_ );
    return *pClientsNetworker_;
}

// -----------------------------------------------------------------------------
// Name: Dispatcher::GetProfileManager
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
inline
ProfileManager& Dispatcher::GetProfileManager() const
{
    assert( pProfileManager_ );
    return *pProfileManager_;
}

}