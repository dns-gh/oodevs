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
#include "actions/Action_ABC.h"
#include "actions/String.h"
#include "clients_gui/RichLabel.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamStringField constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamStringField::ParamStringField( const kernel::OrderParameter& parameter )
    : Param_ABC( parameter.GetName().c_str() )
    , parameter_( parameter )
    , pLabel_( 0 )
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
QWidget* ParamStringField::BuildInterface( QWidget* parent )
{
    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    pLabel_ = new ::gui::RichLabel( GetName(), box );
    pLabel_->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    pEdit_ = new QLineEdit( "", box );
    box->setStretchFactor( pEdit_, 1 );
    return box;
}

// -----------------------------------------------------------------------------
// Name: ParamStringField::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ParamStringField::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    action.AddParameter( *new actions::parameters::String( parameter_, pEdit_->text().ascii() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamStringField::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
bool ParamStringField::IsOptional() const
{
    return parameter_.IsOptional();
}
