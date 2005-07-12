// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_Agent_ABC.cpp $
// $Author: Jvt $
// $Modtime: 5/04/05 11:12 $
// $Revision: 2 $
// $Workfile: MIL_Agent_ABC.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_Agent_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "MT_Tools/MT_Role_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Agent_ABC::MIL_Agent_ABC( uint nID )
    : nID_( nID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC constructor
// Created: JVT 2005-03-15
// -----------------------------------------------------------------------------
MIL_Agent_ABC::MIL_Agent_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Agent_ABC::~MIL_Agent_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_Agent_ABC::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MT_RoleContainer >( *this );
    file >> nID_;
    
    if ( !MIL_EntityManager::unitsIDManager_.IsMosIDValid( nID_ ) )
        MIL_EntityManager::unitsIDManager_.LockSimID( nID_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Agent_ABC::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_Agent_ABC::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MT_RoleContainer >( *this );
    file << nID_;
}
