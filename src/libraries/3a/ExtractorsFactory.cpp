// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
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
    template< typename Value >
    void Extract( const std::string& name, Task& result, const DispatcherFactory< IdentifierValue, Value >& factory )
    {
        typedef FunctionConnector< IdentifierValue::Type, typename Value::Type > Connector;
        boost::shared_ptr< Connector > connector( new Connector() );
        boost::shared_ptr< ModelFunction_ABC > function( factory(
            connector->handlers_.KeyParameter(),
            connector->handlers_.Parameter() ) );
        result.AddExtractor( function );
        result.AddConnector( name, connector );
    }
    template< typename Value >
    void Extract( const std::string& name, xml::xistream& xis, Task& result )
    {
        DispatcherFactory< IdentifierValue, Value > factory( xis );
        Extract( name, result, factory );
    }
    template< typename Value >
    void Extract( const std::string& name, xml::xistream& xis, Task& result, const aar::StaticModel_ABC& model )
    {
        DispatcherFactory< IdentifierValue, Value > factory( xis, model );
        Extract( name, result, factory );
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
        Extract< attributes::OperationalState >( name, xis, result );
    else if( value == "position" )
        Extract< attributes::Position >( name, xis, result );
    else if( value == "resources" )
        Extract< attributes::Resources >( name, xis, result );
    else if( value == "equipments" )
        Extract< attributes::Equipments >( name, xis, result );
    else if( value == "humans" )
        Extract< attributes::Humans >( name, xis, result );
    else if( value == "maintenance-handling-unit" )
        Extract< existences::MaintenanceHandlingUnitId >( name, xis, result );
    else if( value == "direct-fire-unit" )
        Extract< existences::DirectFireUnitId >( name, xis, result );
    else if( value == "fire-component-damage" )
        Extract< events::FireComponentDamages >( name, xis, result );
    else if( value == "detecting-unit" )
        Extract< attributes::Detections >( name, xis, result );
    else if( value == "mounted" )
        Extract< attributes::Mounted >( name, xis, result );
    else if( value == "direct-fire-power" )
        Extract< attributes::DirectFirePower >( name, xis, result, model_ );
    else if( value == "indirect-fire-power" )
        Extract< attributes::IndirectFirePower >( name, xis, result, model_ );
    else if( value == "close-combat-power" )
        Extract< attributes::CloseCombatPower >( name, xis, result, model_ );
    else if( value == "engineering-power" )
        Extract< attributes::EngineeringPower >( name, xis, result, model_ );
    else
        throw std::runtime_error( "Unknown value to extract '" + value + "'" );
}
