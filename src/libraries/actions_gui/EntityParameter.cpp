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
EntityParameterBase::EntityParameterBase( QObject* parent, const QString& name )
    : QObject( parent )
    , Param_ABC( name )
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
    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    pLabel_ = new ::gui::RichLabel( GetName(), false, box );
    entityLabel_ = new QLabel( "---", box );
    entityLabel_->setMinimumWidth( 100 );
    entityLabel_->setAlignment( Qt::AlignCenter );
    entityLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    box->setStretchFactor( entityLabel_, 1 );
    return box;
}

// -----------------------------------------------------------------------------
// Name: EntityParameterBase::Show
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void EntityParameterBase::Show()
{
    pLabel_->show();
    entityLabel_->show();
}

// -----------------------------------------------------------------------------
// Name: EntityParameterBase::Hide
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void EntityParameterBase::Hide()
{
    pLabel_->hide();
    entityLabel_->hide();
}

// -----------------------------------------------------------------------------
// Name: EntityParameterBase::Invalid
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
bool EntityParameterBase::Invalid()
{
    pLabel_->Warn( 3000 );
    return false;
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
    if( pLabel_ )
        pLabel_->setText( label );
}
