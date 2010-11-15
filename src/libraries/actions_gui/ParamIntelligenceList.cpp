// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamIntelligenceList.h"
#include "ParamIntelligence.h"
#include "actions/ParameterList.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamIntelligenceList constructor
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
ParamIntelligenceList::ParamIntelligenceList( QObject* parent, const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& actions, kernel::Controller& controller )
    : EntityListParameter< kernel::Intelligence_ABC >( parent, parameter, actions, controller )
    , parameter_( parameter )
    , converter_( converter )
    , count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamIntelligenceList destructor
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
ParamIntelligenceList::~ParamIntelligenceList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamIntelligenceList::CommitTo
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
void ParamIntelligenceList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::ParameterList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamIntelligenceList::AddToMenu
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
void ParamIntelligenceList::AddToMenu( kernel::ContextMenu& menu )
{
    MakeMenu( tools::translate( "ListParameter", "%1: add item" ).arg( GetName() ), menu );
}

// -----------------------------------------------------------------------------
// Name: ParamIntelligenceList::CreateElement
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
EntityParameter< kernel::Intelligence_ABC >* ParamIntelligenceList::CreateElement( const kernel::Intelligence_ABC& potential )
{
    return new ParamIntelligence( this, kernel::OrderParameter( tools::translate( "ListParameter", "%1 (item %2)" ).arg( GetName() ).arg( ++count_ ).arg( potential.GetName() ).ascii(), "intelligence", false ), converter_, potential, controller_ );
}
