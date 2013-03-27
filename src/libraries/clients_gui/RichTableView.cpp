// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichTableView.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichTableView constructor
// Created: NPT 2013-03-14
// -----------------------------------------------------------------------------
RichTableView::RichTableView( const QString& objectName , QWidget* parent /*= 0*/ )
    : QTableView( parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: RichTableView destructor
// Created: NPT 2013-03-14
// -----------------------------------------------------------------------------
RichTableView::~RichTableView()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
