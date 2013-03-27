// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichItemFactory.h"
#include "moc_RichItemFactory.cpp"
#include "RichLabel.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichItemFactory constructor
// Created: AGE 2006-05-10
// -----------------------------------------------------------------------------
RichItemFactory::RichItemFactory( QObject* parent )
    : QObject( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory destructor
// Created: AGE 2006-05-10
// -----------------------------------------------------------------------------
RichItemFactory::~RichItemFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::Connect
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
RichLabel* RichItemFactory::Connect( RichLabel* item )
{
    connect( item, SIGNAL( LinkClicked( const QString& ) ), this, SLOT( DealWithLink( const QString& ) ) );
    return item;
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::DealWithLink
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
void RichItemFactory::DealWithLink( const QString& anchor )
{
    emit LinkClicked( anchor );
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::CreateLabel
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
RichLabel* RichItemFactory::CreateLabel( const QString& objectName, QWidget* parent /* = 0*/ )
{
    return Connect( new RichLabel( objectName, parent ) );
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::CreateLabel
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
RichLabel* RichItemFactory::CreateLabel( const QString& objectName, const QString& text, QWidget* parent /* = 0*/ )
{
    return Connect( new RichLabel( objectName, text, parent ) );
}

// -----------------------------------------------------------------------------
// Name: RichItemFactory::CreateLabel
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
RichLabel* RichItemFactory::CreateLabel( const QString& objectName, const QString& text, bool required, QWidget* parent /* = 0*/ )
{
    return Connect( new RichLabel( objectName, text, required, parent ) );
}
