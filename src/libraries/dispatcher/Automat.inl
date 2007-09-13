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
// Name: Automat::GetID
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
inline
unsigned long Automat::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: ModelRefsContainer< Agent >& Automat::GetAgents
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
inline
ModelRefsContainer< Agent >& Automat::GetAgents()
{
    return agents_;
}

// -----------------------------------------------------------------------------
// Name: ModelRefsContainer< Automat >& Automat::GetAutomats
// Created: NLD 2007-04-02
// -----------------------------------------------------------------------------
inline
ModelRefsContainer< Automat >& Automat::GetAutomats()
{
    return automats_;
}

// -----------------------------------------------------------------------------
// Name: Automat::IsEngaged
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
inline
bool Automat::IsEngaged() const
{
    return nAutomatState_ == EnumAutomatMode::embraye;
}

}
