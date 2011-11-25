// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamDirection.h"
#include "moc_ParamDirection.cpp"
#include "actions/Direction.h"
#include "actions/Action_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "ENT/Ent_Tr.h"
#include <QtGui/qdial.h>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamDirection constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamDirection::ParamDirection( QObject* parent, const kernel::OrderParameter& parameter )
    : QObject   ( parent )
    , Param_ABC ( ENT_Tr::ConvertFromActionParameter( ENT_Tr::ConvertToActionParameter( parameter.GetName().c_str() ), ENT_Tr_ABC::eToTr ).c_str(), parameter.IsOptional() )
    , parameter_( parameter )
    , value_    ( 180 )
{
    if( name_.isEmpty() )
        name_ = parameter_.GetName().c_str();
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
    QDial* dial = new QDial( 0, 359, 1, 0, parent );
    layout->addWidget( dial );
    dial->setWrapping( true );
    dial->setMaximumSize( 50, 50 );
    connect( dial, SIGNAL( valueChanged( int ) ), SLOT( OnValueChanged( int ) ) );
    dial->setValue( value_ );
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
    value_ = value + ( value > 180 ? -180 : 180 );
}
