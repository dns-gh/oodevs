// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichPushButton.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichPushButton constructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
RichPushButton::RichPushButton( const QString& objectName, const QString& text, QWidget* parent )
    : QPushButton( text, parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: RichPushButton constructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
RichPushButton::RichPushButton( const QString& objectName, const QIcon & icon, const QString& text, QWidget* parent /*= 0*/ )
    : QPushButton( icon, text, parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: RichPushButton destructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
RichPushButton::~RichPushButton()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
