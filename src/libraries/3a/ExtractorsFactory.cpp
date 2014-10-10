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
#include "Damages.h"
#include "Existences.h"
#include "Fires.h"
#include "FirerIdentifierValue.h"
#include "IdentifierValue.h"
#include "DispatchedFunctionHelper.h"
#include "Connectors.h"
#include "Task.h"
#include "protocol/Simulation.h"
#include <tools/Exception.h>
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
            ( "ambulances"                            , BuildExtractor< Value, attributes::LogMedicalEquipments >() )
            ( "close-combat-power"                    , BuildExtractor< Value, attributes::CloseCombatPower >( model ) )
            ( "carriers"                              , BuildExtractor< Value, existences::Carriers >() )
            ( "crowd-states"                          , BuildExtractor< Value, attributes::CrowdStates >() )
            ( "damage-indirect-fires"                 , BuildExtractor< Value, damages::DamageIndirectFire >() )
            ( "detecting-unit"                        , BuildExtractor< Value, attributes::Detections >() )
            ( "direct-fire-power"                     , BuildExtractor< Value, attributes::DirectFirePower >( model ) )
            ( "direct-fire-targets"                   , BuildExtractor< Value, fires::DirectFireTargetsId >() )
            ( "direct-fire-unit"                      , BuildExtractor< Value, fires::DirectFireUnitsId >() )        // A VIRER
            ( "direct-fire-units"                     , BuildExtractor< Value, fires::DirectFireUnitsId >() )
            ( "engineering-power"                     , BuildExtractor< Value, attributes::EngineeringPower >( model ) )
            ( "equipments"                            , BuildExtractor< Value, attributes::Equipments >() )
            ( "fire-component-damage"                 , BuildExtractor< Value, damages::FireComponentDamages, FirerIdentifierValue >() ) // A VIRER
            ( "fire-component-damages"                , BuildExtractor< Value, damages::FireComponentDamages, FirerIdentifierValue >() )
            ( "fire-component-loss"                   , BuildExtractor< Value, damages::FireComponentDamages, TargetIdentifierValue >() )
            ( "fire-human-damages"                    , BuildExtractor< Value, damages::FireHumanDamages, FirerIdentifierValue >() )
            ( "fire-human-loss"                       , BuildExtractor< Value, damages::FireHumanDamages, TargetIdentifierValue >() )
            ( "fire-human-loss-by-crowd-fires"        , BuildExtractor< Value, damages::CrowdHumanDamages >() )
            ( "force-ratio"                           , BuildExtractor< Value, attributes::ForceRatioValue >() )
            ( "humans"                                , BuildExtractor< Value, attributes::Humans >() )
            ( "indirect-fire-positions"               , BuildExtractor< Value, fires::IndirectFireTargetsPositions >() )
            ( "indirect-fire-power"                   , BuildExtractor< Value, attributes::IndirectFirePower >( model ) )
            ( "indirect-fire-unit"                    , BuildExtractor< Value, fires::IndirectFireUnitsId >() )      // A VIRER
            ( "indirect-fire-units"                   , BuildExtractor< Value, fires::IndirectFireUnitsId >() )
            ( "infrastructures-functional-states"     , BuildExtractor< Value, attributes::StructuralStates >() )
            ( "maintenances"                          , BuildExtractor< Value, attributes::LogMaintenanceEquipments >() )
            ( "maintenance-handling-unit"             , BuildExtractor< Value, existences::MaintenanceHandlingUnitId >() ) // A VIRER
            ( "maintenance-handling-units"            , BuildExtractor< Value, existences::MaintenanceHandlingUnitId >() )
            ( "mounted"                               , BuildExtractor< Value, attributes::Mounted >() )
            ( "operational-state"                     , BuildExtractor< Value, attributes::OperationalState >() ) // A VIRER
            ( "operational-states"                    , BuildExtractor< Value, attributes::OperationalState >() )
            ( "population-in-selected-blocks"         , BuildExtractor< Value, attributes::PopulationUrbanOccupation >() )
            ( "populations"                           , BuildExtractor< Value, attributes::PopulationStates >() )
            ( "position"                              , BuildExtractor< Value, attributes::Positions >() )        // A VIRER
            ( "positions"                             , BuildExtractor< Value, attributes::Positions >() )
            ( "resource-networks-functional-states"   , BuildExtractor< Value, attributes::ResourcesNetworksFunctionalStates >() )
            ( "resources"                             , BuildExtractor< Value, attributes::Resources >() )
            ( "satisfactions"                         , BuildExtractor< Value, attributes::PopulationSatisfaction >() )
            ( "satisfactions-dpre"                    , BuildExtractor< Value, attributes::DPRESatisfaction >() )
            ( "resources-satisfactions"               , BuildExtractor< Value, attributes::PopulationResourcesSatisfaction >() )
            ( "stocks"                                , BuildExtractor< Value, attributes::LogSupplyStocks >() )
            ( "waiting-for-medical"                   , BuildExtractor< Value, attributes::LogMedicalWaitingAttention >() )
            ( "contaminated"                          , BuildExtractor< Value, attributes::Contaminated >() )
            ( "nbc-dose"                              , BuildExtractor< Value, attributes::NbcDose >() );
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
        throw MASA_EXCEPTION( "Unknown value to extract '" + value + "'" );
    extractor->second( name, xis, result );
}
