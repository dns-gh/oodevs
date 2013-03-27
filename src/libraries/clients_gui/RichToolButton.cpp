// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichToolButton.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichToolButton constructor
// Created: NPT 2013-03-14
// -----------------------------------------------------------------------------
RichToolButton::RichToolButton( const QString& objectName, QWidget* parent /* = 0*/ )
    : QToolButton( parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: RichToolButton destructor
// Created: NPT 2013-03-14
// -----------------------------------------------------------------------------
RichToolButton::~RichToolButton()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
