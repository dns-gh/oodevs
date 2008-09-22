// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamIntelligenceList.h"
#include "ParamIntelligence.h"
#include "actions/IntelligenceList.h"
#include "gaming/Tools.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "clients_kernel/OrderParameter.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamIntelligenceList constructor
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
ParamIntelligenceList::ParamIntelligenceList( QObject* parent, const OrderParameter& parameter, const CoordinateConverter_ABC& converter, kernel::ActionController& actions, kernel::Controller& controller )
    : EntityListParameter< Intelligence_ABC >( parent, parameter, actions, controller )
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
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::IntelligenceList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );    
}

// -----------------------------------------------------------------------------
// Name: ParamIntelligenceList::AddToMenu
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
void ParamIntelligenceList::AddToMenu( ContextMenu& menu )
{
    MakeMenu( tools::translate( "ParamIntelligenceList", "Add intelligence" ), menu );
}

// -----------------------------------------------------------------------------
// Name: ParamIntelligenceList::CreateElement
// Created: SBO 2007-10-24
// -----------------------------------------------------------------------------
EntityParameter< Intelligence_ABC >* ParamIntelligenceList::CreateElement( const Intelligence_ABC& potential )
{
    return new ParamIntelligence( this, OrderParameter( tools::translate( "ParamIntelligenceList", "Intelligence %1" ).arg( ++count_ ).ascii(), "intelligence", false ), converter_, potential );
}
