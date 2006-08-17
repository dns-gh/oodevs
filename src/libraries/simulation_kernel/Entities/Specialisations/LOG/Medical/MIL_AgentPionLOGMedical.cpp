// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Medical/MIL_AgentPionLOGMedical.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 4 $
// $Workfile: MIL_AgentPionLOGMedical.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentPionLOGMedical.h"

#include "Entities/Agents/Roles/Logistic/Medical/PHY_RolePionLOG_Medical.h"

BOOST_CLASS_EXPORT_GUID( MIL_AgentPionLOGMedical, "MIL_AgentPionLOGMedical" )

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMedical constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMedical::MIL_AgentPionLOGMedical( const MIL_AgentTypePion& type, uint nID, MIL_InputArchive& archive )
    : MIL_AgentPionLOG_ABC( type, nID, archive )
{
    RegisterRole< PHY_RolePionLOG_Medical >( *this );  
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMedical constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMedical::MIL_AgentPionLOGMedical( MIL_Automate& automate, MIL_InputArchive& archive )
    : MIL_AgentPionLOG_ABC( automate, archive )
{
    RegisterRole< PHY_RolePionLOG_Medical >( *this );  
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMedical constructor
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMedical::MIL_AgentPionLOGMedical( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition )
    : MIL_AgentPionLOG_ABC( type, nID, automate, vPosition )
{
    RegisterRole< PHY_RolePionLOG_Medical >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMedical constructor
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMedical::MIL_AgentPionLOGMedical()
    : MIL_AgentPionLOG_ABC()
{

}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMedical destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMedical::~MIL_AgentPionLOGMedical()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMedical::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGMedical::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    
    PHY_RolePionLOG_Medical* pRole;
    file >> pRole;
    RegisterRole( pRole );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMedical::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGMedical::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    file << &GetRole< PHY_RolePionLOG_Medical >();
}
