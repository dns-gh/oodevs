// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichTimeEdit.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichTimeEdit constructor
// Created: NPT 2013-03-19
// -----------------------------------------------------------------------------
RichTimeEdit::RichTimeEdit( const QString& objectName, QWidget* parent /*= 0*/ )
    : QTimeEdit( parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}
// -----------------------------------------------------------------------------
// Name: RichTimeEdit destructor
// Created: NPT 2013-03-19
// -----------------------------------------------------------------------------
RichTimeEdit::~RichTimeEdit()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
