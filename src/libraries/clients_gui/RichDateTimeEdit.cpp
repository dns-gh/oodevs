// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichDateTimeEdit.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit constructor
// Created: NPT 2013-03-20
// -----------------------------------------------------------------------------
RichDateTimeEdit::RichDateTimeEdit( const QString& objectName, QWidget* parent )
    : QDateTimeEdit( parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}
// -----------------------------------------------------------------------------
// Name: RichDateTimeEdit destructor
// Created: NPT 2013-03-20
// -----------------------------------------------------------------------------
RichDateTimeEdit::~RichDateTimeEdit()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
