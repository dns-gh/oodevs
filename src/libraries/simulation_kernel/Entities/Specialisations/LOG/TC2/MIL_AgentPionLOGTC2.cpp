// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Tc2/MIL_AgentPionLOGTC2.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 5 $
// $Workfile: MIL_AgentPionLOGTC2.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentPionLOGTC2.h"

#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePionLOG_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/Medical/PHY_RolePionLOG_Medical.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePionLOG_Supply.h"

BOOST_CLASS_EXPORT_GUID( MIL_AgentPionLOGTC2, "MIL_AgentPionLOGTC2" )

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2 constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGTC2::MIL_AgentPionLOGTC2( const MIL_AgentTypePion& type, uint nID, MIL_InputArchive& archive )
    : MIL_AgentPionLOG_ABC( type, nID, archive )
{
    RegisterRole< PHY_RolePionLOG_Maintenance >( *this );
    RegisterRole< PHY_RolePionLOG_Medical     >( *this );  
    RegisterRole< PHY_RolePionLOG_Supply      >( *this );  
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2 constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGTC2::MIL_AgentPionLOGTC2( MIL_Automate& automate, MIL_InputArchive& archive )
    : MIL_AgentPionLOG_ABC( automate, archive )
{
    RegisterRole< PHY_RolePionLOG_Maintenance >( *this );  
    RegisterRole< PHY_RolePionLOG_Medical     >( *this );  
    RegisterRole< PHY_RolePionLOG_Supply      >( *this );  
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2 constructor
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPionLOGTC2::MIL_AgentPionLOGTC2( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition )
    : MIL_AgentPionLOG_ABC( type, nID, automate, vPosition )
{
    RegisterRole< PHY_RolePionLOG_Maintenance >( *this );  
    RegisterRole< PHY_RolePionLOG_Medical     >( *this );  
    RegisterRole< PHY_RolePionLOG_Supply      >( *this );  
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2 constructor
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
MIL_AgentPionLOGTC2::MIL_AgentPionLOGTC2()
    : MIL_AgentPionLOG_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2 destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGTC2::~MIL_AgentPionLOGTC2()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGTC2::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    
    { PHY_RolePionLOG_Maintenance* pRole; file >> pRole; RegisterRole( pRole ); }
    { PHY_RolePionLOG_Medical    * pRole; file >> pRole; RegisterRole( pRole ); }
    { PHY_RolePionLOG_Supply     * pRole; file >> pRole; RegisterRole( pRole ); }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGTC2::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    file << &GetRole< PHY_RolePionLOG_Maintenance >()
         << &GetRole< PHY_RolePionLOG_Medical     >()
         << &GetRole< PHY_RolePionLOG_Supply      >();
}

