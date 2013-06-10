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
// Created: ABR 2013-06-10
// -----------------------------------------------------------------------------
RichDialogButtonBox::RichDialogButtonBox( const QString& objectname, QWidget* parent /*= 0*/ )
    : RichWidget< QDialogButtonBox >( objectname, parent )
{
    for( int i = 0; i < buttons().size(); ++i )
        buttons().at( i )->setObjectName( objectname + QString::number( i ) );
}

// -----------------------------------------------------------------------------
// Name: RichDialogButtonBox constructor
// Created: NPT 2013-03-22
// -----------------------------------------------------------------------------
RichDialogButtonBox::RichDialogButtonBox( const QString& objectname, StandardButtons buttonType, QWidget* parent /* = 0 */ )
    : RichWidget< QDialogButtonBox >( objectname, parent )
{
    setStandardButtons( buttonType );
    for( int i = 0; i < buttons().size(); ++i )
        buttons().at( i )->setObjectName( objectname + QString::number( i ) );

}

// -----------------------------------------------------------------------------
// Name: RichDialogButtonBox destructor
// Created: NPT 2013-03-22
// -----------------------------------------------------------------------------
RichDialogButtonBox::~RichDialogButtonBox()
{
    // NOTHING
}
