// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichComboBox.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichComboBox constructor
// Created: NPT 2013-03-14
// -----------------------------------------------------------------------------
RichComboBox::RichComboBox( const QString& objectName, QWidget* parent /*= 0*/ )
    : QComboBox( parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}
// -----------------------------------------------------------------------------
// Name: RichComboBox destructor
// Created: NPT 2013-03-14
// -----------------------------------------------------------------------------
RichComboBox::~RichComboBox()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
