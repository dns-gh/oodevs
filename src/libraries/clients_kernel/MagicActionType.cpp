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
#include "OrderParameter.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MagicActionType constructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
MagicActionType::MagicActionType()
    : OrderType()
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: MagicActionType constructor
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
MagicActionType::MagicActionType( const std::string& name, unsigned long id )
    : OrderType()
{
    name_ = name;
    id_ = id;
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: MagicActionType destructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
MagicActionType::~MagicActionType()
{
    Clean();
}

// -----------------------------------------------------------------------------
// Name: MagicActionType::Clean
// Created: JSR 2010-04-06
// -----------------------------------------------------------------------------
void MagicActionType::Clean()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: MagicActionType::Initialize
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
void MagicActionType::Initialize()
{
    if( name_ == "teleport" )
    {
        OrderParameter* param = new OrderParameter( "Location", "point", false );
        Register( Count(), *param );
    }
    else if( name_ == "surrender" )
    {
        OrderParameter* param = new OrderParameter( "Camp", "army", false );
        Register( Count(), *param );
    }
    else if( name_ == "change_human_factors" )
    {
        OrderParameter* tiredness = new OrderParameter( "Tiredness", "enumeration", false );
        tiredness->AddValue( eUnitTiredness_Normal, tools::ToString( eUnitTiredness_Normal ).ascii() );
        tiredness->AddValue( eUnitTiredness_Fatigue, tools::ToString( eUnitTiredness_Fatigue ).ascii() );
        tiredness->AddValue( eUnitTiredness_Epuise, tools::ToString( eUnitTiredness_Epuise ).ascii() );
        Register( Count(), *tiredness );
        OrderParameter* morale = new OrderParameter( "Morale", "enumeration", false );
        morale->AddValue( eUnitMorale_Fanatique, tools::ToString( eUnitMorale_Fanatique ).ascii() );
        morale->AddValue( eUnitMorale_Bon, tools::ToString( eUnitMorale_Bon ).ascii() );
        morale->AddValue( eUnitMorale_Moyen, tools::ToString( eUnitMorale_Moyen ).ascii() );
        morale->AddValue( eUnitMorale_Mauvais, tools::ToString( eUnitMorale_Mauvais ).ascii() );
        Register( Count(), *morale );
        OrderParameter* experience = new OrderParameter( "Experience", "enumeration", false );
        experience->AddValue( eUnitExperience_Veteran, tools::ToString( eUnitExperience_Veteran ).ascii() );
        experience->AddValue( eUnitExperience_Experimente, tools::ToString( eUnitExperience_Experimente ).ascii() );
        experience->AddValue( eUnitExperience_Conscrit, tools::ToString( eUnitExperience_Conscrit ).ascii() );
        Register( Count(), *experience );
    }
    else if( name_ == "partial_recovery" )
    {
        OrderParameter* equipments = new OrderParameter( "Equipments", "list", false );
        Register( Count(), *equipments );
        OrderParameter* humans = new OrderParameter( "Humans", "list", false );
        Register( Count(), *humans );
        OrderParameter* dotations = new OrderParameter( "Dotations", "list", false );
        Register( Count(), *dotations );
        OrderParameter* ammo = new OrderParameter( "Ammo", "list", false );
        Register( Count(), *ammo );
        OrderParameter* stocks = new OrderParameter( "Stocks", "list", false );
        Register( Count(), *stocks );
    }
    else if( name_ == "unit_creation" )
    {
        OrderParameter* superior = new OrderParameter( "UnitType", "identifier", false );
        Register( Count(), *superior );
        OrderParameter* location = new OrderParameter( "Location", "point", false );
        Register( Count(), *location );
    }
    else if( name_ == "population_kill" || name_ == "population_resurrect" )
    {
        OrderParameter* number = new OrderParameter( "Number", "quantity", false );
        Register( Count(), *number );
    }
    else if( name_ == "population_change_attitude" )
    {
        OrderParameter* attitude = new OrderParameter( "Attitude", "enumeration", false );
        attitude->AddValue( ePopulationAttitude_Calme, tools::ToString( ePopulationAttitude_Calme ).ascii() );
        attitude->AddValue( ePopulationAttitude_Agitee, tools::ToString( ePopulationAttitude_Agitee ).ascii() );
        attitude->AddValue( ePopulationAttitude_Excitee, tools::ToString( ePopulationAttitude_Excitee ).ascii() );
        attitude->AddValue( ePopulationAttitude_Agressive, tools::ToString( ePopulationAttitude_Agressive ).ascii() );
        Register( Count(), *attitude );
        // $$$$ JSR 2010-04-16: TODO? not used by now
        // optional int32 flux
        // optional int32 concentration
        // optional bool global
    }
    else if( name_ == "fire_order" )
    {
        OrderParameter* target = new OrderParameter( "Target", "identifier", false );
        Register( Count(), *target );
        OrderParameter* ammo = new OrderParameter( "Ammo", "dotationtype", false );
        Register( Count(), *ammo );
        OrderParameter* iterations = new OrderParameter( "Iterations", "numeric", false );
        Register( Count(), *iterations );
    }
    else if( name_ == "change_knowledge_group" )
    {
        OrderParameter* group = new OrderParameter( "Group", "knowledgegroup", false );
        Register( Count(), *group );
        OrderParameter* camp = new OrderParameter( "Camp", "army", false );
        Register( Count(), *camp );
    }
    else if( name_ == "unit_change_superior" )
    {
        OrderParameter* superior = new OrderParameter( "Superior", "automate", false );
        Register( Count(), *superior );
    }
    else if( name_ == "change_automat_superior" )
    {
        OrderParameter* automat = new OrderParameter( "Automat", "automate", false );
        Register( Count(), *automat );
    }
    else if( name_ == "change_formation_superior" )
    {
        OrderParameter* formation = new OrderParameter( "Formation", "formation", false );
        Register( Count(), *formation );
    }
    else if( name_ == "change_logistic_links" )
    {
        OrderParameter* tc2 = new OrderParameter( "TC2", "identifier", false );
        Register( Count(), *tc2 );
        OrderParameter* maintenance = new OrderParameter( "Maintenance", "identifier", false );
        Register( Count(), *maintenance );
        OrderParameter* sante = new OrderParameter( "Sante", "identifier", false );
        Register( Count(), *sante );
        OrderParameter* ravitaillement = new OrderParameter( "Ravitaillement", "identifier", false );
        Register( Count(), *ravitaillement );
    }
    else if( name_ == "knowledge_group_enable" )
    {
        OrderParameter* enabled = new OrderParameter( "Enabled", "bool", false );
        Register( Count(), *enabled );
    }
    else if( name_ == "knowledge_group_update_side" )
    {
        OrderParameter* camp = new OrderParameter( "Camp", "army", false );
        Register( Count(), *camp );
    }
    else if( name_ == "knowledge_group_update_side_parent" )
    {
        OrderParameter* camp = new OrderParameter( "Camp", "army", false );
        Register( Count(), *camp );
        OrderParameter* parent = new OrderParameter( "Parent", "knowledgegroup", false );
        Register( Count(), *parent );
    }
    else if( name_ == "knowledge_group_update_type" )
    {
        OrderParameter* type = new OrderParameter( "Type", "string", false );
        Register( Count(), *type );
    }
    else if( name_ == "log_supply_push_flow" || name_ == "log_supply_change_quotas" )
    {
        OrderParameter* automat = new OrderParameter( "Receiver", "automate", false );
        Register( Count(), *automat );
        OrderParameter* dotations = new OrderParameter( "Dotations", "list", false );
        Register( Count(), *dotations );
    }
    else if( name_ == "create_object" )
    {
        OrderParameter* type = new OrderParameter( "Type", "string", false );
        Register( Count(), *type );
        OrderParameter* location = new OrderParameter( "Location", "location", false );
        Register( Count(), *location );
        OrderParameter* name = new OrderParameter( "Name", "string", false );
        Register( Count(), *name);
        OrderParameter* camp = new OrderParameter( "Camp", "army", false );
        Register( Count(), *camp );
        OrderParameter* attributes = new OrderParameter( "Attributes", "list", false );
        Register( Count(), *attributes );
    }
    else if( name_ == "update_object" )
    {
        OrderParameter* attributes = new OrderParameter( "Attributes", "list", false );
        Register( Count(), *attributes );
    }
}
