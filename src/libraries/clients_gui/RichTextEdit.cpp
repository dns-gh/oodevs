// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichTextEdit.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichTextEdit constructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
RichTextEdit::RichTextEdit( const QString& objectName )
    : QTextEdit()
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: RichTextEdit destructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
RichTextEdit::~RichTextEdit()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
