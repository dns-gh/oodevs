// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SignalAdapter.h"
#include "moc_SignalAdapter.cpp"

using namespace gui;

SignalAdapter::SignalAdapter( QObject* parent, const std::function< void() >& f )
    : QObject( parent )
    , f_( f )
{
    // NOTHING
}

void SignalAdapter::Handle()
{
    f_();
}
