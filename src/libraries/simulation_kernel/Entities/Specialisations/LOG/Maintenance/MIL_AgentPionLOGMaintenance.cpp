// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Maintenance/MIL_AgentPionLOGMaintenance.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 4 $
// $Workfile: MIL_AgentPionLOGMaintenance.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentPionLOGMaintenance.h"

#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePionLOG_Maintenance.h"

BOOST_CLASS_EXPORT_GUID( MIL_AgentPionLOGMaintenance, "MIL_AgentPionLOGMaintenance" )

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMaintenance constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMaintenance::MIL_AgentPionLOGMaintenance( const MIL_AgentTypePion& type, uint nID, MIL_InputArchive& archive )
    : MIL_AgentPionLOG_ABC( type, nID, archive )
{
    RegisterRole< PHY_RolePionLOG_Maintenance >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMaintenance constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMaintenance::MIL_AgentPionLOGMaintenance( MIL_Automate& automate, MIL_InputArchive& archive )
    : MIL_AgentPionLOG_ABC( automate, archive )
{
    RegisterRole< PHY_RolePionLOG_Maintenance >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMaintenance constructor
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMaintenance::MIL_AgentPionLOGMaintenance( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition )
    : MIL_AgentPionLOG_ABC( type, nID, automate, vPosition )
{
    RegisterRole< PHY_RolePionLOG_Maintenance >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMaintenance constructor
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMaintenance::MIL_AgentPionLOGMaintenance()
    : MIL_AgentPionLOG_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMaintenance destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMaintenance::~MIL_AgentPionLOGMaintenance()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMaintenance::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGMaintenance::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    
    PHY_RolePionLOG_Maintenance* pRole;
    file >> pRole;
    RegisterRole( pRole );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMaintenance::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGMaintenance::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    file << &GetRole< PHY_RolePionLOG_Maintenance >();
}
