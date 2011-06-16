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
#include <map>
#include <boost/assign.hpp>

namespace
{
    template< typename Value, typename Identifier >
    void ExtractFactory( const std::string& name, Task& result, const DispatcherFactory< Identifier, Value >& factory )
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
        ExtractFactory< Value, Identifier >( name, result, factory );
    }
    template< typename Value, typename Identifier >
    void Extract( const std::string& name, xml::xistream& xis, Task& result, const aar::StaticModel_ABC& model )
    {
        DispatcherFactory< Identifier, Value > factory( xis, model );
        ExtractFactory< Value, Identifier >( name, result, factory );
    }
    template< typename return_type, typename extractor_type, typename identifier_type >
    return_type BuildExtractor()
    {
        return boost::bind( &Extract< extractor_type, identifier_type >, _1, _2, _3 );
    }
    template< typename return_type, typename extractor_type >
    return_type BuildExtractor()
    {
        return BuildExtractor< return_type, extractor_type, IdentifierValue >();
    }
    template< typename return_type, typename extractor_type >
    return_type BuildExtractor( const aar::StaticModel_ABC& model )
    {
        return boost::bind( &Extract< extractor_type, IdentifierValue >, _1, _2, _3, boost::cref( model ) );
    }
    template< typename Key, typename Value >
    std::map< Key, Value > BuildExtractors( const aar::StaticModel_ABC& model )
    {
        // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ FIX ME :
        // Certains extracteurs sont définis avec deux noms différents pour la rétrcompatibilité des exercices au niveau des scores.
        // A l'avenir, il faudra utiliser les noms des extracteurs dans la documentation technique de la 3a, pour écrire un indicateur.
        // A terme les noms incohérents avec le reste devront être supprimés, pourquoi pas dans la prochaine version de SWORD.
        // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ FIX ME
        std::map< Key, Value > extractors = boost::assign::list_of< std::pair< Key, Value > >
            ( "operational-state"                     , BuildExtractor< Value, attributes::OperationalState >() ) // A VIRER
            ( "operational-states"                    , BuildExtractor< Value, attributes::OperationalState >() )
            ( "position"                              , BuildExtractor< Value, attributes::Positions >() )        // A VIRER
            ( "positions"                             , BuildExtractor< Value, attributes::Positions >() )
            ( "resources"                             , BuildExtractor< Value, attributes::Resources >() )
            ( "equipments"                            , BuildExtractor< Value, attributes::Equipments >() )
            ( "humans"                                , BuildExtractor< Value, attributes::Humans >() )
            ( "population-states"                     , BuildExtractor< Value, attributes::PopulationStates >() )
            ( "crowd-states"                          , BuildExtractor< Value, attributes::CrowdStates >() )
            ( "satisfactions"                         , BuildExtractor< Value, attributes::PopulationSatisfaction >() )
            ( "population-in-selected-blocks"         , BuildExtractor< Value, attributes::PopulationUrbanOccupation >() )
            ( "infrastructures-functional-states"     , BuildExtractor< Value, attributes::StructuralStates >() )
            ( "resource-networks-functional-states"   , BuildExtractor< Value, attributes::ResourcesNetworksFunctionalStates >() )
            ( "stocks"                                , BuildExtractor< Value, attributes::LogSupplyStocks >() )
            ( "maintenance-handling-units"            , BuildExtractor< Value, existences::MaintenanceHandlingUnitId >() )
            ( "maintenance-handling-unit"             , BuildExtractor< Value, existences::MaintenanceHandlingUnitId >() ) // A VIRER
            ( "direct-fire-unit"                      , BuildExtractor< Value, existences::DirectFireUnitsId >() )        // A VIRER
            ( "direct-fire-units"                     , BuildExtractor< Value, existences::DirectFireUnitsId >() )
            ( "direct-fire-targets"                   , BuildExtractor< Value, existences::DirectFireTargetsId >() )
            ( "indirect-fire-unit"                    , BuildExtractor< Value, existences::IndirectFireUnitsId >() )      // A VIRER
            ( "indirect-fire-units"                   , BuildExtractor< Value, existences::IndirectFireUnitsId >() )
            ( "indirect-fire-positions"               , BuildExtractor< Value, existences::IndirectFireTargetsPositions >() )
            ( "damage-indirect-fires"                 , BuildExtractor< Value, events::DamageIndirectFire >() )
            ( "fire-component-damages"                , BuildExtractor< Value, events::FireComponentDamages, extractors::FireComponentDamages::IdentifierValueFirer >() )
            ( "fire-component-damage"                 , BuildExtractor< Value, events::FireComponentDamages, extractors::FireComponentDamages::IdentifierValueFirer >() ) // A VIRER
            ( "fire-component-loss"                   , BuildExtractor< Value, events::FireComponentDamages, extractors::FireComponentDamages::IdentifierValueTarget >() )
            ( "fire-human-damages"                    , BuildExtractor< Value, events::FireHumanDamages, extractors::FireHumanDamages::IdentifierValueFirer >() )
            ( "fire-human-loss"                       , BuildExtractor< Value, events::FireHumanDamages, extractors::FireHumanDamages::IdentifierValueTarget >() )
            ( "fire-human-loss-by-crowd-fires"        , BuildExtractor< Value, events::CrowdHumanDamages >() )
            ( "ambulances"                            , BuildExtractor< Value, attributes::LogMedicalEquipments >() )
            ( "maintenances"                          , BuildExtractor< Value, attributes::LogMaintenanceEquipments >() )
            ( "waiting-for-medical"                   , BuildExtractor< Value, attributes::LogMedicalWaitingAttention >() )
            ( "detecting-unit"                        , BuildExtractor< Value, attributes::Detections >() )
            ( "mounted"                               , BuildExtractor< Value, attributes::Mounted >() )
            ( "direct-fire-power"                     , BuildExtractor< Value, attributes::DirectFirePower >( model ) )
            ( "indirect-fire-power"                   , BuildExtractor< Value, attributes::IndirectFirePower >( model ) )
            ( "close-combat-power"                    , BuildExtractor< Value, attributes::CloseCombatPower >( model ) )
            ( "engineering-power"                     , BuildExtractor< Value, attributes::EngineeringPower >( model ) );
        return extractors;
    }
}

// -----------------------------------------------------------------------------
// Name: ExtractorsFactory constructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ExtractorsFactory::ExtractorsFactory( const aar::StaticModel_ABC& model )
    : extractors_( BuildExtractors< std::string, T_Extractor >( model ) )
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

// -----------------------------------------------------------------------------
// Name: ExtractorsFactory::CreateElement
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ExtractorsFactory::CreateElement( const std::string& type, xml::xistream& xis, Task& result ) const
{
    if( type != "extract" )
        return;
    std::string value, name;
    xis >> xml::attribute( "function", value )
        >> xml::attribute( "id", name );
    T_Extractors::const_iterator extractor = extractors_.find( value );
    if( extractor == extractors_.end() )
        throw std::runtime_error( "Unknown value to extract '" + value + "'" );
    extractor->second( name, xis, result );
}
