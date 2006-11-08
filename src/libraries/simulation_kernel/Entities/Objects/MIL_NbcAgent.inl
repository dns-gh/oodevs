// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgent::GetType
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
inline
const MIL_NbcAgentType& MIL_NbcAgent::GetType() const
{
    assert( pType_ );
    return *pType_;
}
