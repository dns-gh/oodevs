// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichTabWidget.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichTabWidget constructor
// Created: NPT 2013-03-14
// -----------------------------------------------------------------------------
RichTabWidget::RichTabWidget( const QString& objectName , QWidget* parent /* = 0 */ )
    : QTabWidget( parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}
// -----------------------------------------------------------------------------
// Name: RichTabWidget destructor
// Created: NPT 2013-03-14
// -----------------------------------------------------------------------------
RichTabWidget::~RichTabWidget()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
