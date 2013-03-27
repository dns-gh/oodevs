// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichCheckBox.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichCheckBox constructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
RichCheckBox::RichCheckBox( const QString& objectName )
    : QCheckBox()
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: RichCheckBox constructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
RichCheckBox::RichCheckBox( const QString& objectName, const QString& text, QWidget* parent )
    : QCheckBox( text, parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: RichCheckBox destructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
RichCheckBox::~RichCheckBox()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
