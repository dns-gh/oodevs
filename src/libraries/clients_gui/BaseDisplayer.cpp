// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "BaseDisplayer.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/PopulationConcentration_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/BreakdownType.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Mission.h"
#include "gaming/InternalLinks.h"
#include "Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: BaseDisplayer constructor
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
BaseDisplayer::BaseDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer destructor
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
BaseDisplayer::~BaseDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const bool& value )
{
    static const QString oui = tools::translate( "Oui/Non", "Oui" ) ;
    static const QString non = tools::translate( "Oui/Non", "Non" ) ;
    AddToDisplay( value ? oui : non );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Agent_ABC& value )
{
    AddToDisplay( InternalLinks::CreateLink( value, value.GetName() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const DotationType& value )
{
    AddToDisplay( value.GetCategory() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Team_ABC& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const EquipmentType& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Object_ABC& value )
{
    AddToDisplay( InternalLinks::CreateLink( value, value.GetName() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Population_ABC& value )
{
    AddToDisplay( InternalLinks::CreateLink( value, value.GetName() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const PopulationConcentration_ABC& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const NBCAgent& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const ComponentType& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const BreakdownType& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const ObjectType& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const QTime& value )
{
    static QString format = tools::translate( "Time Format", "hh:mm:ss" );
    AddToDisplay( value.toString( format ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Mission& value )
{
    AddToDisplay( value.GetName() );
}
