// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ExtractorsFactory.h"
#include "Attributes.h"
#include "Existences.h"
#include "Events.h"
#include "IdentifierValue.h"
#include "DispatchedFunctionHelper.h"
#include "Connectors.h"
#include "Task.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ExtractorsFactory constructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ExtractorsFactory::ExtractorsFactory( const aar::StaticModel_ABC& model )
    : model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtractorsFactory destructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ExtractorsFactory::~ExtractorsFactory()
{
    // NOTHING
}

namespace
{
    template< typename Value, typename Identifier >
    void Extract( const std::string& name, Task& result, const DispatcherFactory< Identifier, Value >& factory )
    {
        typedef FunctionConnector< Identifier::Type, typename Value::Type > Connector;
        boost::shared_ptr< Connector > connector( new Connector() );
        boost::shared_ptr< ModelFunction_ABC > function( factory(
            connector->handlers_.KeyParameter(),
            connector->handlers_.Parameter() ) );
        result.AddExtractor( function );
        result.AddConnector( name, connector );
    }
    template< typename Value, typename Identifier >
    void Extract( const std::string& name, xml::xistream& xis, Task& result )
    {
        DispatcherFactory< Identifier, Value > factory( xis );
        Extract< Value, Identifier >( name, result, factory );
    }
    template< typename Value, typename Identifier >
    void Extract( const std::string& name, xml::xistream& xis, Task& result, const aar::StaticModel_ABC& model )
    {
        DispatcherFactory< Identifier, Value > factory( xis, model );
        Extract< Value, Identifier >( name, result, factory );
    }
}

// -----------------------------------------------------------------------------
// Name: ExtractorsFactory::CreateElement
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ExtractorsFactory::CreateElement( const std::string& type, xml::xistream& xis, Task& result ) const
{
    if( type != "extract" )
        return;
    std::string value, name;
    xis >> xml::attribute( "function", value ) >> xml::attribute( "id", name );
    if( value == "operational-state" )
        Extract< attributes::OperationalState, IdentifierValue >( name, xis, result );
    else if( value == "position" )
        Extract< attributes::Position, IdentifierValue >( name, xis, result );
    else if( value == "resources" )
        Extract< attributes::Resources, IdentifierValue >( name, xis, result );
    else if( value == "equipments" )
        Extract< attributes::Equipments, IdentifierValue >( name, xis, result );
    else if( value == "humans" )
        Extract< attributes::Humans, IdentifierValue >( name, xis, result );
    else if( value == "population-states" )
        Extract< attributes::PopulationStates, IdentifierValue >( name, xis, result );
    else if( value == "crowd-states" )
        Extract< attributes::CrowdConcentration, IdentifierValue >( name, xis, result );
    else if( value == "satisfaction" )
        Extract< attributes::PopulationSatisfaction, IdentifierValue >( name, xis, result );
    else if( value == "population-in-selected-blocks" )
        Extract< attributes::PopulationUrbanOccupation, IdentifierValue >( name, xis, result );
    else if( value == "infrastructure-functional-state" )
        Extract< attributes::PopulationUrbanOccupation, IdentifierValue >( name, xis, result );
    else if( value == "stocks" )
        Extract< attributes::LogSupplyStocks, IdentifierValue >( name, xis, result );
    else if( value == "maintenance-handling-unit" )
        Extract< existences::MaintenanceHandlingUnitId, IdentifierValue >( name, xis, result );
    else if( value == "direct-fire-unit" )
        Extract< existences::DirectFireUnitId, IdentifierValue >( name, xis, result );
    else if( value == "indirect-fire-unit" )
        Extract< existences::IndirectFireUnitId, IdentifierValue >( name, xis, result );
    else if( value == "fire-component-damage" )
        Extract< events::FireComponentDamages, IdentifierValue >( name, xis, result );
    else if( value == "fire-human-damage" )
        Extract< events::FireHumanDamages, IdentifierValue >( name, xis, result );
    else if( value == "woundhumans" )
        Extract< events::WoundHumans, extractors::WoundHumans::IdentifierValue >( name, xis, result );
    else if( value == "ambulances" )
        Extract< attributes::LogMedicalEquipments, IdentifierValue >( name, xis, result );
    else if( value == "maintenance" )
        Extract< attributes::LogMaintenanceEquipments, IdentifierValue >( name, xis, result );
    else if( value == "waiting-for-medical" )
        Extract< attributes::LogMedicalWaitingAttention, IdentifierValue >( name, xis, result );
    else if( value == "detecting-unit" )
        Extract< attributes::Detections, IdentifierValue >( name, xis, result );
    else if( value == "mounted" )
        Extract< attributes::Mounted, IdentifierValue >( name, xis, result );
    else if( value == "direct-fire-power" )
        Extract< attributes::DirectFirePower, IdentifierValue >( name, xis, result, model_ );
    else if( value == "indirect-fire-power" )
        Extract< attributes::IndirectFirePower, IdentifierValue >( name, xis, result, model_ );
    else if( value == "close-combat-power" )
        Extract< attributes::CloseCombatPower, IdentifierValue >( name, xis, result, model_ );
    else if( value == "engineering-power" )
        Extract< attributes::EngineeringPower, IdentifierValue >( name, xis, result, model_ );
    else
        throw std::runtime_error( "Unknown value to extract '" + value + "'" );
}
