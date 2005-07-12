//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneForbiddenFire.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 4 $
// $Workfile: MIL_ZoneForbiddenFire.cpp $
//
//*****************************************************************************

#include "MIL_Pch.h"

#include "MIL_ZoneForbiddenFire.h"
#include "MIL_RealObjectType.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/MIL_Army.h"

BOOST_CLASS_EXPORT_GUID( MIL_ZoneForbiddenFire, "MIL_ZoneForbiddenFire" )

//-----------------------------------------------------------------------------
// Name: MIL_ZoneForbiddenFire constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneForbiddenFire::MIL_ZoneForbiddenFire()
    : MIL_RealObject_ABC( MIL_RealObjectType::zoneForbiddenFire_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneForbiddenFire destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneForbiddenFire::~MIL_ZoneForbiddenFire()
{
	
}


// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneForbiddenFire::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_ZoneForbiddenFire::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}

// =============================================================================
// COLLISION
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneForbiddenFire::CanCollideWithAgent
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
bool MIL_ZoneForbiddenFire::CanCollideWithAgent( const MIL_Agent_ABC& agent ) const
{
    return agent.GetArmy() == GetArmy();
}
