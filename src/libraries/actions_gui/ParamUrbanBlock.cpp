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
#include "ParamInterface_ABC.h"
#include "actions/UrbanBlock.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/ObjectType.h"

using namespace actions::gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock constructor
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
ParamUrbanBlock::ParamUrbanBlock( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : EntityParameter< UrbanObject_ABC >( builder, parameter )
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
void ParamUrbanBlock::NotifyContextMenu( const kernel::UrbanObject_ABC& entity, kernel::ContextMenu& menu )
{
    EntityParameter< UrbanObject_ABC >::NotifyContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock::CommitTo
// Created: LGY 2011-02-25
// -----------------------------------------------------------------------------
void ParamUrbanBlock::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::unique_ptr< actions::parameters::Entity< kernel::UrbanObject_ABC > > param( new actions::parameters::UrbanBlock( parameter_, controllers_.controller_ ) );
    EntityParameter< UrbanObject_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamUrbanBlock::Visit
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
void ParamUrbanBlock::Visit( const actions::parameters::UrbanBlock& param )
{
    InternalVisit( param );
}
