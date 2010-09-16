// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "AgentFactory.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "tools/MIL_IDManager.h"
#include "tools/MIL_Tools.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/Decision/DEC_DataBase.h"
#include "MT_Tools/MT_FormatString.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( AgentFactory )

// -----------------------------------------------------------------------------
// Name: AgentFactory constructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
AgentFactory::AgentFactory( MIL_IDManager& idManager, DEC_DataBase& database, unsigned int gcPause, unsigned int gcMult )
    : idManager_( idManager )
    , algorithmsFactories_( new AlgorithmsFactories() )
    , database_( database )
    , gcPause_ (gcPause )
    , gcMult_  ( gcMult )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentFactory destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
AgentFactory::~AgentFactory()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
MIL_AgentPion* AgentFactory::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis )
{
    MIL_AgentPion* pPion = tools::Resolver< MIL_AgentPion >::Find( xis.attribute< unsigned long >( "id" ) );
    if( pPion )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "A unit with ID '%d' already exists.", pPion->GetID() ) );

    pPion = type.InstanciatePion( automate, *algorithmsFactories_, xis );

    type.RegisterRoles( *pPion , database_, gcPause_, gcMult_ );

    std::string strPosition;
    xis >> xml::attribute( "position", strPosition );
    MT_Vector2D vPosTmp;
    MIL_Tools::ConvertCoordMosToSim( strPosition, vPosTmp );

    Initialize( *pPion, vPosTmp );
    tools::Resolver< MIL_AgentPion >::Register( pPion->GetID(), *pPion );
    pPion->ReadOverloading( xis );
    return pPion;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
MIL_AgentPion* AgentFactory::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition )
{
    MIL_AgentPion* pPion = type.InstanciatePion( automate, *algorithmsFactories_ );
    type.RegisterRoles( *pPion, database_, gcPause_, gcMult_ );

    Initialize( *pPion, vPosition );
    tools::Resolver< MIL_AgentPion >::Register( pPion->GetID(), *pPion );
    return pPion;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Initialize
// Created: SLG 2010-01-21
// -----------------------------------------------------------------------------
void AgentFactory::Initialize( MIL_AgentPion& pion, const MT_Vector2D& vPosition )
{
    pion.GetRole< PHY_RoleInterface_Location >().Move( vPosition, MT_Vector2D( 0., 1. ), 0. );
    pion.GetRole< PHY_RoleInterface_UrbanLocation >().MagicMove( vPosition );
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::load
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void AgentFactory::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object < AgentFactory_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::save
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void AgentFactory::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object < AgentFactory_ABC >( *this );
}
