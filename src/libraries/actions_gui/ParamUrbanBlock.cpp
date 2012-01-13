// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamUrbanBlock.h"
#include "actions/UrbanBlock.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "MissionInterface_ABC.h"

using namespace actions::gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock constructor
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
ParamUrbanBlock::ParamUrbanBlock( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : EntityParameter< Object_ABC >( builder, parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock destructor
// Created: LGY 2011-02-25
// -----------------------------------------------------------------------------
ParamUrbanBlock::~ParamUrbanBlock()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock::NotifyContextMenu
// Created: LGY 2011-04-05
// -----------------------------------------------------------------------------
void ParamUrbanBlock::NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu )
{
    if( entity.GetType().IsUrban() )
        EntityParameter< Object_ABC >::NotifyContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock::CommitTo
// Created: LGY 2011-02-25
// -----------------------------------------------------------------------------
void ParamUrbanBlock::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::Entity< Object_ABC > > param( new actions::parameters::UrbanBlock( parameter_, controller_ ) );
    EntityParameter< Object_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}
