// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "EntityParameter.h"
#include "ParamInterface_ABC.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: EntityParameterBase constructor
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
EntityParameterBase::EntityParameterBase( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder, parameter )
{
    entityLabel_ = new QLabel();
    entityLabel_->setMinimumWidth( 100 );
    entityLabel_->setAlignment( Qt::AlignCenter );
    Display( "---" );
}

// -----------------------------------------------------------------------------
// Name: EntityParameterBase destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
EntityParameterBase::~EntityParameterBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityParameterBase::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* EntityParameterBase::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QHBoxLayout* layout = new QHBoxLayout( group_ );
    layout->addWidget( entityLabel_ );
    if( needSwitchEditor_ )
        layout->addWidget( CreateSwitchEditor() );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: EntityParameterBase::Show
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void EntityParameterBase::Show()
{
    group_->show();
}

// -----------------------------------------------------------------------------
// Name: EntityParameterBase::Hide
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void EntityParameterBase::Hide()
{
    group_->hide();
}

// -----------------------------------------------------------------------------
// Name: EntityParameterBase::Display
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityParameterBase::Display( const QString& what )
{
    entityLabel_->setText( what );
}
