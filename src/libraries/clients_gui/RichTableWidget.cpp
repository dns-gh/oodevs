// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichTableWidget.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichTableWidget constructor
// Created: NPT 2013-03-18
// -----------------------------------------------------------------------------
RichTableWidget::RichTableWidget( const QString& objectName, QWidget* parent /*= 0*/ )
    : QTableWidget( parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}
// -----------------------------------------------------------------------------
// Name: RichTableWidget destructor
// Created: NPT 2013-03-18
// -----------------------------------------------------------------------------
RichTableWidget::~RichTableWidget()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
