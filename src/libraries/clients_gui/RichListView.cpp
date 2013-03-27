// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichListView.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichListView constructor
// Created: NPT 2013-03-14
// -----------------------------------------------------------------------------
RichListView::RichListView( const QString& objectName , QWidget* parent  )
    : QListView( parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: RichListView destructor
// Created: NPT 2013-03-14
// -----------------------------------------------------------------------------
RichListView::~RichListView()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
