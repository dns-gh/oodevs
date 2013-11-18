// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamStringField.h"
#include "InterfaceBuilder_ABC.h"
#include "actions/Action_ABC.h"
#include "actions/String.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamStringField constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamStringField::ParamStringField( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder, parameter )
    , pEdit_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamStringField destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamStringField::~ParamStringField()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamStringField::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamStringField::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    pEdit_ = new QLineEdit( parent );
    pEdit_->setPlaceholderText( "Enter your text here" );
    layout->addWidget( pEdit_ );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamStringField::InternalCheckValidity
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
bool ParamStringField::InternalCheckValidity() const
{
    return !pEdit_->text().isEmpty();
}

// -----------------------------------------------------------------------------
// Name: ParamStringField::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ParamStringField::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() && !pEdit_->text().isEmpty() )
        action.AddParameter( *new actions::parameters::String( parameter_, pEdit_->text().toStdString() ) );
    else
        action.AddParameter( *new actions::parameters::String( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamStringField::Visit
// Created: ABR 2013-06-12
// -----------------------------------------------------------------------------
void ParamStringField::Visit( const actions::parameters::String& param )
{
    ActivateOptionalIfNeeded( param );
    if( param.IsSet() )
        pEdit_->setText( param.GetValue() );
}
