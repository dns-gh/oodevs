// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "EntityParameter.h"
#include "moc_EntityParameter.cpp"
#include "RichLabel.h"

// -----------------------------------------------------------------------------
// Name: EntityParameterBase constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
EntityParameterBase::EntityParameterBase( QWidget* pParent, ASN1T_OID& id, const std::string& label, const std::string& menu )
    : QHBox     ( pParent )
    , id_       ( id )
    , menu_     ( menu )
{
    pLabel_ = new RichLabel( label.c_str(), false, this, "" );

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
void EntityParameterBase::AddToMenu( QPopupMenu& menu )
{
    menu.insertItem( menu_.c_str(), this, SLOT( MenuItemValidated() ) );
}

// -----------------------------------------------------------------------------
// Name: EntityParameterBase::Display
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityParameterBase::Display( const QString& what )
{
    entityLabel_->setText( what );
}
