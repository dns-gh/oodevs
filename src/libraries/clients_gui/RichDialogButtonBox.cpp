// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichDialogButtonBox.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichDialogButtonBox constructor
// Created: NPT 2013-03-22
// -----------------------------------------------------------------------------
RichDialogButtonBox::RichDialogButtonBox( const QString& objectname, StandardButtons buttonType )
    : QDialogButtonBox( buttonType )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectname );
    for( int i = 0; i < buttons().size(); ++i )
        buttons().at( i )->setObjectName( objectname + QString::number( i ) );

}

// -----------------------------------------------------------------------------
// Name: RichDialogButtonBox destructor
// Created: NPT 2013-03-22
// -----------------------------------------------------------------------------
RichDialogButtonBox::~RichDialogButtonBox()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
