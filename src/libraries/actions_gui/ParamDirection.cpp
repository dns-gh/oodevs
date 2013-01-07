// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamDirection.h"
#include "moc_ParamDirection.cpp"
#include "InterfaceBuilder_ABC.h"
#include "actions/Direction.h"
#include "actions/Action_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "ENT/Ent_Tr.h"
#include <QtGui/qdial.h>

using namespace actions::gui;

namespace
{
    const int turnDegrees = 360;
    int AddHalfTurn( int nDegrees )
    {
        if( nDegrees < 0 )
            nDegrees = nDegrees % turnDegrees + turnDegrees;
        return ( nDegrees + turnDegrees / 2 ) % turnDegrees;
    }
}

// -----------------------------------------------------------------------------
// Name: ParamDirection constructor
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
ParamDirection::ParamDirection( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder.GetParentObject(), builder.GetParamInterface(), parameter )
    , value_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDirection destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamDirection::~ParamDirection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDirection::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamDirection::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QHBoxLayout* layout = new QHBoxLayout( group_ );
    QDial* dial = new QDial( 0, turnDegrees - 1, 1, 0, parent );
    layout->addWidget( dial );
    dial->setWrapping( true );
    dial->setMaximumSize( 50, 50 );
    connect( dial, SIGNAL( valueChanged( int ) ), SLOT( OnValueChanged( int ) ) );
    dial->setValue( AddHalfTurn( value_ ) );
    layout->addWidget( dial, Qt::AlignCenter );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamDirection::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ParamDirection::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() )
        action.AddParameter( *new actions::parameters::Direction( parameter_, value_ ) );
    else
        action.AddParameter( *new actions::parameters::Direction( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamDirection::OnValueChanged
// Created: SBO 2007-03-16
// -----------------------------------------------------------------------------
void ParamDirection::OnValueChanged( int value )
{
    value_ = AddHalfTurn( value );
}
