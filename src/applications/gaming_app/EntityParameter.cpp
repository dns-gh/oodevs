// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EntityParameter.h"
#include "moc_EntityParameter.cpp"
#include "clients_gui/RichLabel.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntityParameterBase constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
EntityParameterBase::EntityParameterBase( QWidget* pParent, ASN1T_OID& id, const QString& label, const QString& menu )
    : QHBox( pParent )
    , id_  ( id )
    , menu_( menu )
{
    pLabel_ = new RichLabel( label, false, this, "" );

    entityLabel_ = new QLabel( "---", this );
    entityLabel_->setMinimumWidth( 100 );
    entityLabel_->setAlignment( Qt::AlignCenter );
    entityLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
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
// Name: EntityParameterBase::Invalid
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
bool EntityParameterBase::Invalid()
{
    pLabel_->Warn( 3000 );
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntityParameterBase::SetId
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityParameterBase::SetId( ASN1T_OID id )
{
    id_ = id;
}

// -----------------------------------------------------------------------------
// Name: EntityParameterBase::AddToMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityParameterBase::AddToMenu( ContextMenu& menu )
{
    menu.InsertItem( "Parameter", menu_.c_str(), this, SLOT( MenuItemValidated() ) );
}

// -----------------------------------------------------------------------------
// Name: EntityParameterBase::Display
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityParameterBase::Display( const QString& what )
{
    entityLabel_->setText( what );
}
