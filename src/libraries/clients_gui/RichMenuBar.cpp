// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichMenuBar.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichMenuBar constructor
// Created: NPT 2013-03-26
// -----------------------------------------------------------------------------
RichMenuBar::RichMenuBar( const QString& objectName, QWidget* parent )
    : QMenuBar( parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: RichMenuBar destructor
// Created: NPT 2013-03-26
// -----------------------------------------------------------------------------
RichMenuBar::~RichMenuBar()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
