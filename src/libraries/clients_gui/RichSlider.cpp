// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichSlider.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichSlider constructor
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
RichSlider::RichSlider( const QString& objectName, QWidget* parent /* = 0 */ )
    : RichWidget< QSlider >( objectName, parent )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichSlider destructor
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
RichSlider::~RichSlider()
{
        // NOTHING
}
