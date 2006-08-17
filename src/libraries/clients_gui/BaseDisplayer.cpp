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
#include "gaming/PopulationConcentration.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/BreakdownType.h"
#include "clients_kernel/ObjectType.h"
#include "gaming/Experience.h"
#include "gaming/Morale.h"
#include "gaming/Tiredness.h"
#include "clients_kernel/Mission.h"
#include "gaming/InternalLinks.h"

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
    AddToDisplay( value ? qApp->tr( "Oui" ) : qApp->tr( "Non" ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Agent_ABC& value )
{
    AddToDisplay( InternalLinks::CreateLink( value, value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const DotationType& value )
{
    AddToDisplay( QString( value.GetCategory().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Team_ABC& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const EquipmentType& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Object_ABC& value )
{
    AddToDisplay( InternalLinks::CreateLink( value, value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Population_ABC& value )
{
    AddToDisplay( InternalLinks::CreateLink( value, value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const PopulationConcentration& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const NBCAgent& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const ComponentType& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const BreakdownType& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const ObjectType& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-06-27
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Experience& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-06-27
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Morale& value )
{   
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-06-27
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Tiredness& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const QTime& value )
{
    AddToDisplay( value.toString( "hh:mm:ss" ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Mission& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}
