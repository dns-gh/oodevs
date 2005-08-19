// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Convoy/MIL_AgentPionLOGConvoy.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 4 $
// $Workfile: MIL_AgentPionLOGConvoy.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_AgentPionLOGConvoy.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePionLOGConvoy_Supply.h"

BOOST_CLASS_EXPORT_GUID( MIL_AgentPionLOGConvoy, "MIL_AgentPionLOGConvoy" )

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGConvoy::MIL_AgentPionLOGConvoy( const MIL_AgentTypePion& type, uint nID, MIL_InputArchive& archive )
    : MIL_AgentPionLOG_ABC( type, nID, archive )
{
    RegisterRole< PHY_RolePionLOGConvoy_Supply >( *this );
    throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Creation of pion of type 'Pion LOG Convoi' not allowed in ODB", archive.GetContext() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGConvoy::MIL_AgentPionLOGConvoy( MIL_Automate& automate, MIL_InputArchive& archive )
    : MIL_AgentPionLOG_ABC( automate, archive )
{
    RegisterRole< PHY_RolePionLOGConvoy_Supply >( *this );
    throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Creation of pion of type 'Pion LOG Convoi' not allowed in ODB", archive.GetContext() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy constructor
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPionLOGConvoy::MIL_AgentPionLOGConvoy( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition )
    : MIL_AgentPionLOG_ABC( type, nID, automate, vPosition )
{
    RegisterRole< PHY_RolePionLOGConvoy_Supply >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy constructor
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
MIL_AgentPionLOGConvoy::MIL_AgentPionLOGConvoy()
    : MIL_AgentPionLOG_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGConvoy::~MIL_AgentPionLOGConvoy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGConvoy::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    
    PHY_RolePionLOGConvoy_Supply *pRole;
    file >> pRole;
    RegisterRole( pRole );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGConvoy::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    file << &GetRole< PHY_RolePionLOGConvoy_Supply >();
}
