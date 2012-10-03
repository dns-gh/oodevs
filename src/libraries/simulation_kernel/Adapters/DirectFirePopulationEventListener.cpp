// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DirectFirePopulationEventListener.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_Pion.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include <core/Facade.h>
#include <core/Model.h>

using namespace sword;

namespace
{
    const std::string event = "direct fire population";
}

//-----------------------------------------------------------------------------
// Name: DirectFirePopulationEventListener constructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
DirectFirePopulationEventListener::DirectFirePopulationEventListener( const core::Model& model, core::Facade& facade, tools::Resolver< MIL_AgentPion >& resolver )
    : model_   ( model )
    , facade_  ( facade )
    , resolver_( resolver )
{
    facade.Register( event, *this );
}

//-----------------------------------------------------------------------------
// Name: DirectFirePopulationEventListener destructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
DirectFirePopulationEventListener::~DirectFirePopulationEventListener()
{
    facade_.Unregister( event, *this );
    for( IT_Results it = results_.begin(); it != results_.end(); ++it )
        it->second->DecRef();
}

// -----------------------------------------------------------------------------
// Name: DirectFirePopulationEventListener::Notify
// Created: MCO 2012-04-26
// -----------------------------------------------------------------------------
void DirectFirePopulationEventListener::Notify( const core::Model& event )
{
    try
    {
        if( event[ "running" ] )
            Update( event );
        else
            Remove( event );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception in " << __FUNCTION__ << " : " << e.what() );
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Unknown exception in " << __FUNCTION__ );
    }
}

// -----------------------------------------------------------------------------
// Name: DirectFirePopulationEventListener::Update
// Created: MCO 2012-05-04
// -----------------------------------------------------------------------------
void DirectFirePopulationEventListener::Update( const core::Model& event )
{
    MIL_AgentPion& pion = resolver_.Get( static_cast< unsigned int >( event[ "entity" ] ) );
    MIL_PopulationElement_ABC& element = event[ "element/data" ].GetUserData< MIL_PopulationElement_ABC >();
    const std::string& dotation = event[ "dotation" ];
    const PHY_DotationCategory* category = PHY_DotationType::FindDotationCategory( dotation );
    if( ! category )
    {
        MT_LOG_ERROR_MSG( "Unknown dotation category in DirectFirePopulationEventListener : " << dotation );
        return;
    }
    PHY_FireResults_Pion*& results = results_[ event[ "entity" ] ]; // $$$$ MCO 2012-04-26: use command id instead ? can a unit have several direct fire running in parallel ?
    if( ! results )
    {
        results = new PHY_FireResults_Pion( pion, element );
        results->IncRef();
    }
    const bool lethal = &PHY_AmmoDotationClass::alr_ != category->GetAmmoDotationClass();
    element.ApplyFire( event[ "hits" ], *results, lethal );
}

// -----------------------------------------------------------------------------
// Name: DirectFirePopulationEventListener::Remove
// Created: MCO 2012-05-04
// -----------------------------------------------------------------------------
void DirectFirePopulationEventListener::Remove( const core::Model& event )
{
    IT_Results it = results_.find( event[ "entity" ] );
    if( it != results_.end() )
    {
        it->second->DecRef();
        results_.erase( it );
    }
}
