// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-28 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetID
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
uint MIL_Population::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetType
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
const MIL_PopulationType& MIL_Population::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetName
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
const std::string& MIL_Population::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDecision
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
DEC_PopulationDecision& MIL_Population::GetDecision() const
{
    assert( pDecision_ );
    return *pDecision_;
}
