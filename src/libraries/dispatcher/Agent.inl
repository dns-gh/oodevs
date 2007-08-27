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
// Name: Agent::GetID
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
inline
unsigned long Agent::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetName
// Created: SBO 2007-08-27
// -----------------------------------------------------------------------------
inline
std::string Agent::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetAutomat
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
inline
const Automat& Agent::GetAutomat() const
{
    if( !pAutomat_ )
        throw std::runtime_error( "Agent automat not set" );
    return *pAutomat_;
}

}
