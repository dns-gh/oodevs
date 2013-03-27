// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichTreeWidget.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichTreeWidget constructor
// Created: NPT 2013-03-18
// -----------------------------------------------------------------------------
RichTreeWidget::RichTreeWidget( const QString& objectName, QWidget* parent /*= 0*/ )
    : QTreeWidget( parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}
// -----------------------------------------------------------------------------
// Name: RichTreeWidget destructor
// Created: NPT 2013-03-18
// -----------------------------------------------------------------------------
RichTreeWidget::~RichTreeWidget()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
