// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "MagicActionType.h"
#include "protocol/Protocol.h"
#include "Karma.h"
#include "OrderParameter.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MagicActionType constructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
MagicActionType::MagicActionType()
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: MagicActionType constructor
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
MagicActionType::MagicActionType( const std::string& name, unsigned long id )
    : OrderType( name, id )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: MagicActionType constructor
// Created: FDS 2010-11-23
// -----------------------------------------------------------------------------
MagicActionType::MagicActionType( const std::string& name )
    : OrderType( name, 0 )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: MagicActionType destructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
MagicActionType::~MagicActionType()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: MagicActionType::CreateOrderParameter
// Created: JSR 2010-04-23
// -----------------------------------------------------------------------------
OrderParameter* MagicActionType::CreateOrderParameter( const std::string& name, const std::string& type )
{
    OrderParameter* param = new OrderParameter( name, type, false );
    Register( Count(), *param );
    return param;
}

// -----------------------------------------------------------------------------
// Name: MagicActionType::Initialize
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
void MagicActionType::Initialize()
{
    const std::string& name = GetName();
    if( name == "teleport" )
        CreateOrderParameter( "Location", "point" );
    else if( name == "surrender" )
        CreateOrderParameter( "Camp", "army" );
    else if( name == "change_human_factors" )
    {
        OrderParameter* tiredness = CreateOrderParameter( "Tiredness", "enumeration" );
        tiredness->AddValue( eUnitTiredness_Normal, tools::ToString( eUnitTiredness_Normal ).ascii() );
        tiredness->AddValue( eUnitTiredness_Fatigue, tools::ToString( eUnitTiredness_Fatigue ).ascii() );
        tiredness->AddValue( eUnitTiredness_Epuise, tools::ToString( eUnitTiredness_Epuise ).ascii() );

        OrderParameter* morale = CreateOrderParameter( "Morale", "enumeration" );
        morale->AddValue( eUnitMorale_Fanatique, tools::ToString( eUnitMorale_Fanatique ).ascii() );
        morale->AddValue( eUnitMorale_Bon, tools::ToString( eUnitMorale_Bon ).ascii() );
        morale->AddValue( eUnitMorale_Moyen, tools::ToString( eUnitMorale_Moyen ).ascii() );
        morale->AddValue( eUnitMorale_Mauvais, tools::ToString( eUnitMorale_Mauvais ).ascii() );

        OrderParameter* experience = CreateOrderParameter( "Experience", "enumeration" );
        experience->AddValue( eUnitExperience_Veteran, tools::ToString( eUnitExperience_Veteran ).ascii() );
        experience->AddValue( eUnitExperience_Experimente, tools::ToString( eUnitExperience_Experimente ).ascii() );
        experience->AddValue( eUnitExperience_Conscrit, tools::ToString( eUnitExperience_Conscrit ).ascii() );
    }
    else if( name == "partial_recovery" )
    {
        CreateOrderParameter( "Equipments", "list" );
        CreateOrderParameter( "Humans", "list" );
        CreateOrderParameter( "Dotations", "list" );
        CreateOrderParameter( "Ammo", "list" );
        CreateOrderParameter( "Stocks", "list" );
    }
    else if( name == "formation_creation" )
    {
        CreateOrderParameter( "Level", "numeric" );
        CreateOrderParameter( "Name", "string" );
        CreateOrderParameter( "LogLevel", "string" );
    }
    else if( name == "unit_creation" )
    {
        CreateOrderParameter( "UnitType", "identifier" );
        CreateOrderParameter( "Location", "point" );
    }
    else if( name == "crowd_creation" )
    {
        CreateOrderParameter( "PopulationType", "string" );
        CreateOrderParameter( "Location", "point" );
        CreateOrderParameter( "Number", "numeric" );
        CreateOrderParameter( "Name", "string" );
    }
    else if( name == "crowd_change_attitude" )
    {
        OrderParameter* attitude = CreateOrderParameter( "Attitude", "enumeration" );
        attitude->AddValue( ePopulationAttitude_Calme, tools::ToString( ePopulationAttitude_Calme ).ascii() );
        attitude->AddValue( ePopulationAttitude_Agitee, tools::ToString( ePopulationAttitude_Agitee ).ascii() );
        attitude->AddValue( ePopulationAttitude_Excitee, tools::ToString( ePopulationAttitude_Excitee ).ascii() );
        attitude->AddValue( ePopulationAttitude_Agressive, tools::ToString( ePopulationAttitude_Agressive ).ascii() );
    }
    else if( name == "fire_order" )
    {
        CreateOrderParameter( "UnitTarget", "identifier" );
        CreateOrderParameter( "Ammo", "resourcetype" );
        CreateOrderParameter( "Iterations", "numeric" );
    }
    else if( name == "fire_order_on_location" )
    {
        CreateOrderParameter( "LocationTarget", "location" );
        CreateOrderParameter( "Ammo", "resourcetype" );
        CreateOrderParameter( "Iterations", "numeric" );
    }
    else if( name == "change_knowledge_group" )
    {
        CreateOrderParameter( "Group", "knowledgegroup" );
        CreateOrderParameter( "Camp", "army" );
    }
    else if( name == "unit_change_superior" )
        CreateOrderParameter( "Superior", "automate" );
    else if( name == "change_automat_superior" )
        CreateOrderParameter( "Automat", "automate" );
    else if( name == "change_formation_superior" )
        CreateOrderParameter( "Formation", "formation" );
    else if( name == "change_logistic_links" )
    {
        CreateOrderParameter( "TC2", "identifier" );
        CreateOrderParameter( "Maintenance", "identifier" );
        CreateOrderParameter( "Sante", "identifier" );
        CreateOrderParameter( "Ravitaillement", "identifier" );
    }
    else if( name == "knowledge_group_enable" )
        CreateOrderParameter( "Enabled", "bool" );
    else if( name == "knowledge_group_update_side" )
        CreateOrderParameter( "Camp", "army" );
    else if( name == "knowledge_group_update_side_parent" )
    {
        CreateOrderParameter( "Camp", "army" );
        CreateOrderParameter( "Parent", "knowledgegroup" );
    }
    else if( name == "knowledge_group_update_type" )
        CreateOrderParameter( "Type", "string" );
    else if( name == "automat_log_supply_push_flow" || name == "automat_log_supply_pull_flow" || name == "automat_log_supply_change_quotas" )
    {
        CreateOrderParameter( "Receiver", "automat" );
        CreateOrderParameter( "Dotations", "list" );
    }
    else if( name == "formation_log_supply_push_flow" || name == "formation_log_supply_pull_flow" || name == "formation_log_supply_change_quotas" )
    {
        CreateOrderParameter( "Receiver", "formation" );
        CreateOrderParameter( "Dotations", "list" );
    }
    else if( name == "create_object" )
    {
        CreateOrderParameter( "Type", "string" );
        CreateOrderParameter( "Location", "location" );
        CreateOrderParameter( "Name", "string" );
        CreateOrderParameter( "Camp", "army" );
        CreateOrderParameter( "Attributes", "list" );
    }
    else if( name == "update_object" )
        CreateOrderParameter( "Attributes", "list" );
    else if( name == "global_weather" || name == "local_weather" )
    {
        CreateOrderParameter( "Temperature", "numeric" );
        CreateOrderParameter( "WindSpeed", "numeric" );
        CreateOrderParameter( "WindDirection", "heading" );
        CreateOrderParameter( "CloudFloor", "numeric" );
        CreateOrderParameter( "CloudCeiling", "numeric" );
        CreateOrderParameter( "CloudDensity", "numeric" );
        OrderParameter* precipitation = CreateOrderParameter( "Precipitation", "enumeration" );
        precipitation->AddValue( 0, "PasDePrecipitation" );
        precipitation->AddValue( 1, "TempeteDeSable"     );
        precipitation->AddValue( 2, "Brouillard"         );
        precipitation->AddValue( 3, "Crachin"            );
        precipitation->AddValue( 4, "Pluie"              );
        precipitation->AddValue( 5, "Neige"              );
        precipitation->AddValue( 6, "Fumigene"           );
        if( name == "local_weather" )
        {
            CreateOrderParameter( "StartTime", "datetime" );
            CreateOrderParameter( "EndTime", "datetime" );
            CreateOrderParameter( "Location", "location" );
            CreateOrderParameter( "ID", "identifier" );
        }
    }
    else if( name == "change_diplomacy" )
    {
        CreateOrderParameter( "Camp1", "identifier" );
        CreateOrderParameter( "Camp2", "identifier" );
        OrderParameter* diplomacy = CreateOrderParameter( "Diplomacy", "enumeration" );
        diplomacy->AddValue( sword::unknown, Karma::unknown_.GetName().ascii() );
        diplomacy->AddValue( sword::friendly, Karma::friend_.GetName().ascii() );
        diplomacy->AddValue( sword::enemy, Karma::enemy_.GetName().ascii() );
        diplomacy->AddValue( sword::neutral, Karma::neutral_.GetName().ascii() );
    }
    else if( name == "create_knowledge_group" )
    {
        CreateOrderParameter( "Camp", "identifier" );
        CreateOrderParameter( "Parent", "identifier" );
        CreateOrderParameter( "Type", "string" );
    }
    else if( name == "change_resource_links" )
    {
        CreateOrderParameter( "Target", "identifier" );
        CreateOrderParameter( "Urban", "bool" );
        CreateOrderParameter( "Nodes", "list" );
    }
    else if( name == "update_urban" )
    {
        CreateOrderParameter( "Target", "identifier" );
        CreateOrderParameter( "StructuralState", "quantity" );
        //CreateOrderParameter( "Threshold", "quantity" );
    }
    else if( name == "automat_creation" )
    {
        CreateOrderParameter( "AutomatType", "identifier" );
        CreateOrderParameter( "KnowledgeGroup", "identifier" );
    }
    else if( name == "crowd_change_health_state" )
    {
        CreateOrderParameter( "Healthy", "quantity" );
        CreateOrderParameter( "Wounded", "quantity" );
        CreateOrderParameter( "Contaminated", "quantity" );
        CreateOrderParameter( "Dead", "quantity" );
    }
    else if( name == "crowd_change_affinities" )
        CreateOrderParameter( "Affinities", "list" );
    else if( name == "crowd_change_armed_individuals" )
        CreateOrderParameter( "ArmedIndividuals", "quantity" );
    else if( name == "inhabitant_change_health_state" )
    {
        CreateOrderParameter( "Healthy", "quantity" );
        CreateOrderParameter( "Wounded", "quantity" );
        CreateOrderParameter( "Dead", "quantity" );
    }
    else if( name == "inhabitant_change_affinities" )
        CreateOrderParameter( "Affinities", "list" );
    else if( name == "inhabitant_change_alerted_state" )
        CreateOrderParameter( "Alerted", "bool" );
    else if( name == "unit_change_affinities" )
        CreateOrderParameter( "Affinities", "list" );
    else if( name == "change_extension" )
        CreateOrderParameter( "Extensions", "extensionlist" );
    else if( name == "change_critical_intelligence" )
        CreateOrderParameter( "Name", "string" );
}
