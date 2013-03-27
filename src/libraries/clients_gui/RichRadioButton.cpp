// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichRadioButton.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichRadioButton constructor
// Created: NPT 2013-03-14
// -----------------------------------------------------------------------------
RichRadioButton::RichRadioButton( const QString& objectName, const QString& text, QWidget* parent /*= 0*/ )
    : QRadioButton( text, parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: RichRadioButton destructor
// Created: NPT 2013-03-14
// -----------------------------------------------------------------------------
RichRadioButton::~RichRadioButton()
{
        ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
