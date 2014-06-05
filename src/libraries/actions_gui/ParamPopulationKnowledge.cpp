// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamPopulationKnowledge.h"
#include "actions/PopulationKnowledge.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge constructor
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
ParamPopulationKnowledge::ParamPopulationKnowledge( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : EntityParameter< kernel::Population_ABC >( builder, parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamPopulationKnowledge::~ParamPopulationKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::unique_ptr< actions::parameters::Entity< kernel::Population_ABC > > param( new actions::parameters::PopulationKnowledge( parameter_, controllers_.controller_ ) );
    EntityParameter< kernel::Population_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::Visit
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::Visit( const actions::parameters::PopulationKnowledge& param )
{
    InternalVisit( param );
}

// -----------------------------------------------------------------------------
// Name: ParamPopulationKnowledge::NotifyContextMenu
// Created: LGY 2013-11-27
// -----------------------------------------------------------------------------
void ParamPopulationKnowledge::NotifyContextMenu( const kernel::PopulationKnowledge_ABC& knowledge, kernel::ContextMenu& menu )
{
    if( const kernel::Population_ABC* population = knowledge.GetEntity() )
        EntityParameter< kernel::Population_ABC >::NotifyContextMenu( *population, menu );
}
