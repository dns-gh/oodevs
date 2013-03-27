// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichListWidget.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichListWidget constructor
// Created: NPT 2013-03-18
// -----------------------------------------------------------------------------
RichListWidget::RichListWidget( const QString& objectName, QWidget* parent /*= 0*/ )
    : QListWidget( parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: RichListWidget destructor
// Created: NPT 2013-03-18
// -----------------------------------------------------------------------------
RichListWidget::~RichListWidget()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
