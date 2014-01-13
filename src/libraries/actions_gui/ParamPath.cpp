// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamPath.h"
#include "InterfaceBuilder_ABC.h"
#include "actions/Action_ABC.h"
#include "actions/Path.h"
#include "actions/PathPoint.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Path.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Positions.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamPath constructor
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
ParamPath::ParamPath( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : ParamLocation_ABC< actions::parameters::Path >( builder, parameter )
    , entity_( builder.GetControllers() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPath destructor
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
ParamPath::~ParamPath()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPath::OnMenuClick
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
void ParamPath::OnMenuClick()
{
    if( !entity_ )
        return;
    layer_.StartPath( *this, entity_->Get< kernel::Positions >() );
    ParamLocation_ABC< actions::parameters::Path >::OnMenuClick();
}

// -----------------------------------------------------------------------------
// Name: ParamPath::BuildInterface
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
QWidget* ParamPath::BuildInterface( const QString& objectName, QWidget* parent )
{
    QWidget* result = ParamLocation_ABC< actions::parameters::Path >::BuildInterface( objectName, parent );
    group_->setEnabled( false );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ParamPath::SetEntity
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void ParamPath::SetEntity( const kernel::Entity_ABC* entity )
{
    if( location_ && entity != entity_ )
    {
        kernel::Path& path = static_cast< kernel::Path& >( *location_ );
        path.SetEntityPosition( entity ? entity->Retrieve< kernel::Positions >() : 0 );
        path.FixOrigin( false );
    }
    entity_ = entity;
    if( group_ )
        group_->setEnabled( IsInParam() || entity != 0 );
}

// -----------------------------------------------------------------------------
// Name: ParamPath::Visit
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void ParamPath::Visit( const actions::parameters::Path& param )
{
    if( !entity_ )
        return;

    ActivateOptionalIfNeeded( param );
    location_.reset( new kernel::Path( entity_->Get< kernel::Positions >() ) );
    pPosLabel_->setText( location_->GetName() );

    auto it = param.CreateIterator();
    while( it.HasMoreElements() )
    {
        const actions::parameters::PathPoint& param = static_cast< const actions::parameters::PathPoint& >( it.NextElement() );
        assert( param.GetPoints().size() == 1 );
        location_->AddPoint( param.GetPoints().front() );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamPath::FixOrigin
// Created: ABR 2014-01-07
// -----------------------------------------------------------------------------
void ParamPath::FixOrigin( bool fix ) const
{
    if( location_ )
        static_cast< kernel::Path& >( *location_ ).FixOrigin( fix );
}
