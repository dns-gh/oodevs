// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Nature_GUI.h"
#include "ADN_Connector_String.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ADN_Nature_GUI constructor
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
ADN_Nature_GUI::ADN_Nature_GUI( QWidget* parent )
    : NatureEditionWidget( parent, "symbols.xml" )
{
    pConnector_ = new ADN_Connector_String< ADN_Nature_GUI >( this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Nature_GUI destructor
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
ADN_Nature_GUI::~ADN_Nature_GUI()
{
    delete pConnector_;
}
