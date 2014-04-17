// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "BaseDisplayer.h"
#include "InternalLinks.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/PopulationConcentration_ABC.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_kernel/FireClass.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/BreakdownType.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/PopulationType.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/KnowledgeGroupType.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/MaterialCompositionType.h"
#include "clients_kernel/RoofShapeType.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Tools.h"

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
    static const QString yes = tools::translate( "Yes/No", "Yes" ) ;
    static const QString no = tools::translate( "Yes/No", "No" ) ;
    AddToDisplay( value ? yes : no );
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
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::Automat_ABC& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const DotationType& value )
{
    AddToDisplay( value.GetName() );
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
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Entity_ABC& value )
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
// Created: JSR 2012-06-06
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::UrbanObject_ABC& value )
{
    AddToDisplay( InternalLinks::CreateLink( value, value.GetName() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: SBO 2007-10-30
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Formation_ABC& value )
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
// Created: SLG 2010-12-06
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Inhabitant_ABC& value )
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
// Created: RFT 2006-04-05
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const FireClass& value )
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
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::AgentType& value )
{
    AddToDisplay( value.GetLocalizedName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::AutomatType& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::PopulationType& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: SLG 2010-12-06
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::InhabitantType& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::Duration& value )
{
    AddToDisplay( tools::DurationFromSeconds( value() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Mission& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::KnowledgeGroupType& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: SBO 2007-11-02
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::HierarchyLevel_ABC& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: SBO 2007-11-26
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::Karma& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::AgentKnowledge_ABC& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::PopulationKnowledge_ABC& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::ObjectKnowledge_ABC& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: JSR 2010-04-19
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::KnowledgeGroup_ABC& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Clear
// Created: LDC 2009-03-18
// -----------------------------------------------------------------------------
void BaseDisplayer::Clear()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: ABR 2011-10-20
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::Ghost_ABC& value )
{
    AddToDisplay( InternalLinks::CreateLink( value, value.GetName() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: LGY 2012-08-22
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::MaterialCompositionType& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: LGY 2012-08-22
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const kernel::RoofShapeType& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: JSR 2014-04-17
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const tools::Path& value )
{
    AddToDisplay( value.ToUTF8() );
}
