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
#include "moc_EntityParameter.cpp"
#include "clients_gui/RichLabel.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: EntityParameterBase constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
EntityParameterBase::EntityParameterBase( QObject* parent, const QString& name, bool optional )
    : QObject( parent )
    , Param_ABC( name, optional )
{
    // NOTHING
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
QWidget* EntityParameterBase::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    entityLabel_ = new QLabel( "---", parent );
    layout->addWidget( entityLabel_ );
    entityLabel_->setMinimumWidth( 100 );
    entityLabel_->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
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
// Name: EntityParameterBase::AddToMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityParameterBase::AddToMenu( kernel::ContextMenu& menu )
{
    if( entityLabel_->isShown() )
        menu.InsertItem( "Parameter", GetName(), this, SLOT( MenuItemValidated() ) );
}

// -----------------------------------------------------------------------------
// Name: EntityParameterBase::Display
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityParameterBase::Display( const QString& what )
{
    entityLabel_->setText( what );
}

// -----------------------------------------------------------------------------
// Name: EntityParameterBase::SetLabel
// Created: ABR 2011-01-21
// -----------------------------------------------------------------------------
void EntityParameterBase::SetLabel( const QString& label )
{
    if( group_ )
        group_->setTitle( label );
}
