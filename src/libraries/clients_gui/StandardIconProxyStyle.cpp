// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "StandardIconProxyStyle.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: StandardIconProxyStyle constructor
// Created: ABR 2013-02-01
// -----------------------------------------------------------------------------
StandardIconProxyStyle::StandardIconProxyStyle( int iconSize /* = 32 */ )
    : iconSize_( iconSize )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StandardIconProxyStyle destructor
// Created: ABR 2013-02-01
// -----------------------------------------------------------------------------
StandardIconProxyStyle::~StandardIconProxyStyle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StandardIconProxyStyle::pixelMetric
// Created: ABR 2013-02-01
// -----------------------------------------------------------------------------
int StandardIconProxyStyle::pixelMetric( PixelMetric metric, const QStyleOption* option /*= 0*/, const QWidget * widget /*= 0*/ ) const
{
    return ( metric == QStyle::PM_SmallIconSize ) ? iconSize_ : QCleanlooksStyle::pixelMetric( metric, option, widget );
}
