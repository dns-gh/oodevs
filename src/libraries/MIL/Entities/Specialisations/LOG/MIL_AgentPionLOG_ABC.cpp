// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_AgentPionLOG_ABC.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 11:32 $
// $Revision: 6 $
// $Workfile: MIL_AgentPionLOG_ABC.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_AgentPionLOG_ABC.h"

#include "Entities/Automates/MIL_AutomateType.h"

BOOST_CLASS_EXPORT_GUID( MIL_AgentPionLOG_ABC, "MIL_AgentPionLOG_ABC" )

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOG_ABC::MIL_AgentPionLOG_ABC( const MIL_AgentTypePion& type, uint nID, MIL_InputArchive& archive )
    : MIL_AgentPion( type, nID, archive )
{
    if( !GetAutomate().GetType().IsLogistic() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "The automata of this pion is not a logistic one", archive.GetContext() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOG_ABC::MIL_AgentPionLOG_ABC( MIL_Automate& automate, MIL_InputArchive& archive )
    : MIL_AgentPion( automate, archive )
{
    if ( !automate.GetType().IsLogistic() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "The automata of this pion is not a logistic one", archive.GetContext() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC constructor
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPionLOG_ABC::MIL_AgentPionLOG_ABC( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition )
    : MIL_AgentPion( type, nID, automate, vPosition )
{
    assert( automate.GetType().IsLogistic() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC constructor
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
MIL_AgentPionLOG_ABC::MIL_AgentPionLOG_ABC()
    : MIL_AgentPion()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOG_ABC::~MIL_AgentPionLOG_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_AgentPionLOG_ABC::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_AgentPion >( *this );
}
