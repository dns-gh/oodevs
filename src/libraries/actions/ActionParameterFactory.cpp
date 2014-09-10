// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "ActionParameterFactory.h"
#include "Agent.h"
#include "Army.h"
#include "AtlasNature.h"
#include "Automat.h"
#include "Bool.h"
#include "DateTime.h"
#include "Direction.h"
#include "DotationType.h"
#include "EquipmentType.h"
#include "EngineerConstruction.h"
#include "Enumeration.h"
#include "ExtensionList.h"
#include "Formation.h"
#include "Identifier.h"
#include "Itinerary.h"
#include "KnowledgeGroup.h"
#include "Lima.h"
#include "LimaList.h"
#include "Limit.h"
#include "Location.h"
#include "MaintenancePriorities.h"
#include "MedicalPriorities.h"
#include "Numeric.h"
#include "ObjectKnowledge.h"
#include "ParameterList.h"
#include "Path.h"
#include "Point.h"
#include "Polygon.h"
#include "PopulationKnowledge.h"
#include "PullFlowParameters.h"
#include "PushFlowParameters.h"
#include "Quantity.h"
#include "ResourceNetworkNode.h"
#include "ResourceNetworkType.h"
#include "String.h"
#include "UrbanBlock.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/StaticModel.h"
#include "protocol/Protocol.h"
#pragma warning( push, 1 )
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterFactory::ActionParameterFactory( const kernel::CoordinateConverter_ABC& converter,
                                                const kernel::EntityResolver_ABC& entities,
                                                const kernel::StaticModel& staticModel,
                                                kernel::ObjectKnowledgeConverter_ABC& objectKnowledgeConverter,
                                                kernel::Controller& controller )
    : converter_               ( converter )
    , entities_                ( entities )
    , staticModel_             ( staticModel )
    , objectKnowledgeConverter_( objectKnowledgeConverter )
    , controller_              ( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory destructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterFactory::~ActionParameterFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
Parameter_ABC* ActionParameterFactory::CreateParameter( const kernel::OrderParameter& parameter,
                                                        const sword::MissionParameter& message,
                                                        const kernel::Entity_ABC* entity ) const
{
    if( !parameter.IsRepeated() && message.value_size() == 1 && parameter.GetType() != "list" )
        return CreateParameter( parameter, message.value( 0 ), entity, message.null_value() );
    return new parameters::ParameterList( parameter, message.value(), *this, entity );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: MGD 2010-11-09
// -----------------------------------------------------------------------------
Parameter_ABC* ActionParameterFactory::CreateParameter( const kernel::OrderParameter& parameter,
                                                        const sword::MissionParameter_Value& message,
                                                        const kernel::Entity_ABC* entity,
                                                        bool nullValue ) const
{
    if( message.has_booleanvalue() )
        return ( nullValue ) ? new parameters::Bool( parameter )                            : new parameters::Bool( parameter, message.booleanvalue() != 0 );
    if( message.has_acharstr() )
        return ( nullValue ) ? new parameters::String( parameter )                          : new parameters::String( parameter, message.acharstr().c_str() );
    if( message.has_agent() )
        return ( nullValue ) ? new parameters::Agent( parameter, controller_, false )       : new parameters::Agent( parameter, message.agent().id(), entities_, controller_, false );
    if( message.has_areal() )
        return ( nullValue ) ? new parameters::Numeric( parameter )                         : new parameters::Numeric( parameter, message.areal() );
    if( message.has_intvalue() )
        return ( nullValue ) ? new parameters::Numeric( parameter )                         : new parameters::Numeric( parameter, static_cast< float >( message.intvalue() ) );
    if( message.has_automat() )
        return ( nullValue ) ? new parameters::Automat( parameter, controller_ )            : new parameters::Automat( parameter, message.automat().id(), entities_, controller_ );
    if( message.has_heading() )
        return ( nullValue ) ? new parameters::Direction( parameter )                       : new parameters::Direction( parameter, message.heading().heading() );
    if( message.has_enumeration() )
        return ( nullValue ) ? new parameters::Enumeration( parameter )                     : new parameters::Enumeration( parameter, message.enumeration() );
    if( message.has_path() )
        return ( nullValue ) ? new parameters::Path( parameter, converter_ )                : new parameters::Path( parameter, converter_, message.path().location() );
    if( message.has_agentknowledge() )
        return ( nullValue ) ? new parameters::Agent( parameter, controller_, true )        : new parameters::Agent( parameter, message.agentknowledge().id(), entities_, controller_, true );
    if( message.has_objectknowledge() )
        return ( nullValue ) ? new parameters::ObjectKnowledge( parameter, controller_ )    : new parameters::ObjectKnowledge( parameter, message.objectknowledge().id(), objectKnowledgeConverter_, entity ? &*entity : 0, controller_, entities_ );
    if( message.has_crowdknowledge() )
        return ( nullValue ) ? new parameters::PopulationKnowledge( parameter, controller_ ): new parameters::PopulationKnowledge( parameter, message.crowdknowledge().id(), entities_, controller_ );
    if( message.has_location() )
        return ( nullValue ) ? new parameters::Location( parameter, converter_ )            : new parameters::Location( parameter, converter_, message.location() );
    if( message.has_plannedwork() )
        return ( nullValue ) ? new parameters::EngineerConstruction( parameter )            : new parameters::EngineerConstruction( parameter, converter_, staticModel_.objectTypes_, entities_, message.plannedwork(), controller_, staticModel_.objectTypes_ );
    if( message.has_nature() )
        return ( nullValue ) ? new parameters::AtlasNature( parameter )                     : new parameters::AtlasNature( parameter, message.nature(), staticModel_.atlasNatures_ );
    if( message.has_missionobjective() )
        return 0; // $$$$ ABR 2011-11-22: Unused and unsupported now, need to modify .proto
    if( message.has_point() )
        return ( nullValue ) ? new parameters::Point( parameter, converter_ )               : new parameters::Point( parameter, converter_, message.point() );
    if( message.has_area() )
        return ( nullValue ) ? new parameters::Polygon( parameter, converter_ )             : new parameters::Polygon( parameter, converter_, message.area().location() );
    if( message.has_resourcetype() )
        return ( nullValue ) ? new parameters::DotationType( parameter )                    : new parameters::DotationType( parameter, message.resourcetype().id(), staticModel_.objectTypes_ );
    if( message.has_equipmenttype() )
        return nullValue ? new parameters::EquipmentType( parameter )                       : new parameters::EquipmentType( parameter, message.equipmenttype().id(), staticModel_.objectTypes_ );
    if( message.has_logmaintenancepriorities() )
        return ( nullValue ) ? new parameters::MaintenancePriorities( parameter )           : new parameters::MaintenancePriorities( parameter, staticModel_.objectTypes_, message.logmaintenancepriorities() );
    if( message.has_logmedicalpriorities() )
        return ( nullValue ) ? new parameters::MedicalPriorities( parameter )               : new parameters::MedicalPriorities( parameter, message.logmedicalpriorities() );
    if( message.has_indirectfire() ) // $$$$ SBO 2007-05-21: reports only, not to be used!
        return 0;
    if( message.has_limit() )
        return ( nullValue ) ? new parameters::Limit( parameter, converter_ )               : new parameters::Limit( parameter, converter_, message.limit() );
    if( message.has_phaseline() )
    {
        if( message.phaseline().elem_size() > 1 )
            return ( nullValue ) ? new parameters::LimaList( parameter )                    : new parameters::LimaList( parameter, converter_, message.phaseline(), entity, objectKnowledgeConverter_, controller_, entities_ );
        else if( message.phaseline().elem_size() == 1 )
            return ( nullValue ) ? new parameters::Lima( parameter )                        : new parameters::Lima( parameter, converter_, message.phaseline().elem(0), entity, objectKnowledgeConverter_, controller_, entities_ );
    }
    if( message.has_datetime() )
        return ( nullValue ) ? new parameters::DateTime( parameter )                        : new parameters::DateTime( parameter, message.datetime() );
    if( message.has_urbanknowledge() )  // $$$$ _RC_ LGY 2011-02-24: urban block id
        return ( nullValue ) ? new parameters::UrbanBlock( parameter, controller_ )         : new parameters::UrbanBlock( parameter, message.urbanknowledge().id(), entities_, controller_ );
    if( message.has_party() )
        return ( nullValue ) ? new parameters::Army( parameter, controller_ )               : new parameters::Army( parameter, message.party().id(), entities_, controller_ );
    if( message.has_formation() )
        return ( nullValue ) ? new parameters::Formation( parameter, controller_ )          : new parameters::Formation( parameter, message.formation().id(), entities_, controller_ );
    if( message.has_quantity() )
        return ( nullValue ) ? new parameters::Quantity( parameter )                        : new parameters::Quantity( parameter, message.quantity() );
    if( message.has_identifier() )
        return ( nullValue ) ? new parameters::Identifier( parameter )                      : new parameters::Identifier( parameter, message.identifier() );
    if( message.has_knowledgegroup() )
        return ( nullValue ) ? new parameters::KnowledgeGroup( parameter, controller_ )     : new parameters::KnowledgeGroup( parameter, message.knowledgegroup().id(), entities_, controller_ );
    if( message.has_resourcenetworknode() )
        return ( nullValue ) ? new parameters::ResourceNetworkNode( parameter, controller_ ): new parameters::ResourceNetworkNode( parameter, message.resourcenetworknode(), entities_, controller_ );
    if( message.has_resourcenetworktype() )
        return ( nullValue ) ? new parameters::ResourceNetworkType( parameter )             : new parameters::ResourceNetworkType( parameter, message.resourcenetworktype().name(), staticModel_.objectTypes_ );
    if( message.has_extensionlist() )
        return ( nullValue ) ? new parameters::ExtensionList( parameter )                   : new parameters::ExtensionList( parameter, message.extensionlist() );
    if( message.has_push_flow_parameters() )
        return ( nullValue ) ? new parameters::PushFlowParameters( parameter, converter_, false ) : new parameters::PushFlowParameters( parameter, converter_, entities_, staticModel_.objectTypes_, staticModel_.objectTypes_, message.push_flow_parameters() );
    if( message.has_pull_flow_parameters() )
        return ( nullValue ) ? new parameters::PullFlowParameters( parameter, converter_ ) : new parameters::PullFlowParameters( parameter, converter_, entities_, staticModel_.objectTypes_, staticModel_.objectTypes_, message.pull_flow_parameters() );
    if( message.has_pathfind() )
        return ( nullValue ) ? new parameters::Itinerary( parameter, converter_ ) : new parameters::Itinerary( parameter, converter_, message.pathfind() );
    if( message.list_size() || parameter.IsList() )
        return new parameters::ParameterList( parameter, message.list(), *this, entity );
    return 0;
}
