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
        OrderParameter* tiredness = new OrderParameter( "Tiredness", "enumeration", true );
        tiredness->AddValue( eUnitTiredness_Normal, tools::ToString( eUnitTiredness_Normal ).ascii() );
        tiredness->AddValue( eUnitTiredness_Fatigue, tools::ToString( eUnitTiredness_Fatigue ).ascii() );
        tiredness->AddValue( eUnitTiredness_Epuise, tools::ToString( eUnitTiredness_Epuise ).ascii() );
        Register( Count(), *tiredness );
        OrderParameter* morale = new OrderParameter( "Morale", "enumeration", true );
        morale->AddValue( eUnitMorale_Fanatique, tools::ToString( eUnitMorale_Fanatique ).ascii() );
        morale->AddValue( eUnitMorale_Bon, tools::ToString( eUnitMorale_Bon ).ascii() );
        morale->AddValue( eUnitMorale_Moyen, tools::ToString( eUnitMorale_Moyen ).ascii() );
        morale->AddValue( eUnitMorale_Mauvais, tools::ToString( eUnitMorale_Mauvais ).ascii() );
        Register( Count(), *morale );
        OrderParameter* experience = new OrderParameter( "Experience", "enumeration", true );
        experience->AddValue( eUnitExperience_Veteran, tools::ToString( eUnitExperience_Veteran ).ascii() );
        experience->AddValue( eUnitExperience_Experimente, tools::ToString( eUnitExperience_Experimente ).ascii() );
        experience->AddValue( eUnitExperience_Conscrit, tools::ToString( eUnitExperience_Conscrit ).ascii() );
        Register( Count(), *experience );
    }
    else if( name_ == "partial_recovery" )
    {
        OrderParameter* equipments = new OrderParameter( "Equipments", "list", true );
        Register( Count(), *equipments );
        OrderParameter* humans = new OrderParameter( "Humans", "list", true );
        Register( Count(), *humans );
        OrderParameter* dotations = new OrderParameter( "Dotations", "list", true );
        Register( Count(), *dotations );
        OrderParameter* ammo = new OrderParameter( "Ammo", "list", true );
        Register( Count(), *ammo );
        OrderParameter* stocks = new OrderParameter( "Stocks", "list", true );
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
}
