//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneForbiddenMove.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 11:26 $
// $Revision: 4 $
// $Workfile: MIL_ZoneForbiddenMove.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_ZoneForbiddenMove.h"
#include "MIL_RealObjectType.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/MIL_Army.h"

BOOST_CLASS_EXPORT_GUID( MIL_ZoneForbiddenMove, "MIL_ZoneForbiddenMove" )

//-----------------------------------------------------------------------------
// Name: MIL_ZoneForbiddenMove constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneForbiddenMove::MIL_ZoneForbiddenMove()
    : MIL_RealObject_ABC( MIL_RealObjectType::zoneForbiddenMove_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneForbiddenMove destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneForbiddenMove::~MIL_ZoneForbiddenMove()
{	
}


// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneForbiddenMove::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_ZoneForbiddenMove::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}


// =============================================================================
// COLLISION
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneForbiddenMove::CanCollideWithAgent
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
bool MIL_ZoneForbiddenMove::CanCollideWithAgent( const MIL_Agent_ABC& agent ) const
{
    return agent.GetArmy() == GetArmy();
}
