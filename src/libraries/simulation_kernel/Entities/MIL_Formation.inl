// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Entities/Agents/Units/Categories/PHY_NatureLevel.h"

// -----------------------------------------------------------------------------
// Name: MIL_Formation::GetArmy
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
inline
MIL_Army& MIL_Formation::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::GetID
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
inline
uint MIL_Formation::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::RegisterAutomate
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
inline
void MIL_Formation::RegisterAutomate( MIL_Automate& automate )
{
    assert( automates_.find( &automate ) == automates_.end() );
    automates_.insert( &automate );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::UnregisterAutomate
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
inline
void MIL_Formation::UnregisterAutomate( MIL_Automate& automate )
{
    assert( automates_.find( &automate ) != automates_.end() );
    automates_.erase( &automate );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::RegisterFormation
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
inline    
void MIL_Formation::RegisterFormation( MIL_Formation& formation )
{
    assert( formations_.find( &formation ) == formations_.end() );
    formations_.insert( &formation );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::UnregisterFormation
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
inline
void MIL_Formation::UnregisterFormation( MIL_Formation& formation )
{
    assert( formations_.find( &formation ) != formations_.end() );
    formations_.erase( &formation );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::RegisterIntelligence
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
inline
void MIL_Formation::RegisterIntelligence( MIL_Intelligence& intelligence )
{
    assert( intelligences_.find( &intelligence ) == intelligences_.end() );
    intelligences_.insert( &intelligence );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::UnregisterIntelligence
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
inline
void MIL_Formation::UnregisterIntelligence( MIL_Intelligence& intelligence )
{
    assert( intelligences_.find( &intelligence ) != intelligences_.end() );
    intelligences_.erase( &intelligence );
}
