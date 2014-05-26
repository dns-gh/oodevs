// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamBool.h"
#include "moc_ParamBool.cpp"
#include "InterfaceBuilder_ABC.h"
#include "actions/Action_ABC.h"
#include "actions/Bool.h"
#include "clients_kernel/OrderParameter.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamBool constructor
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
ParamBool::ParamBool( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder, parameter )
    , value_   ( false )
    , comboBox_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamBool destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamBool::~ParamBool()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamBool::SetValue
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
void ParamBool::SetValue( bool value )
{
    value_ = value;
    if( comboBox_ != 0 )
        comboBox_->setCurrentIndex( value_ ? 0 : 1 );
}

// -----------------------------------------------------------------------------
// Name: ParamBool::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamBool::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QHBoxLayout* layout = new QHBoxLayout( group_ );
    layout->setAlignment( Qt::AlignCenter );
    comboBox_ = new QComboBox( parent );
    comboBox_->addItem( tr( "True" ) );
    comboBox_->addItem( tr( "False" ) );
    comboBox_->setCurrentIndex( value_ ? 0 : 1 );
    layout->addWidget( comboBox_, Qt::AlignCenter );
    connect( comboBox_, SIGNAL( currentIndexChanged( int ) ), SLOT( OnCurrentIndexChanged( int ) ) );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamBool::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ParamBool::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() )
        action.AddParameter( *new actions::parameters::Bool( parameter_, value_ ) );
    else
        action.AddParameter( *new actions::parameters::Bool( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamBool::OnCurrentIndexChanged
// Created: SBO 2007-03-16
// -----------------------------------------------------------------------------
void ParamBool::OnCurrentIndexChanged( int index )
{
    value_ = ( index == 0 );
    Update();
}

// -----------------------------------------------------------------------------
// Name: ParamBool::Visit
// Created: ABR 2013-06-12
// -----------------------------------------------------------------------------
void ParamBool::Visit( const actions::parameters::Bool& param )
{
    ActivateOptionalIfNeeded( param );
    if( param.IsSet() )
        SetValue( param.GetValue() );
}
