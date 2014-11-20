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
#include <boost/algorithm/string/erase.hpp>

using namespace gui;

SignalAdapter::SignalAdapter( QObject* sender, const char* signal, const std::function< void() >& f )
    : f_( f )
{
    connect( sender, signal, this, SLOT( Handle() ) );
    // If sender is a QWidget, its children will be deleted in its destructor
    // before its QObject parent has a chance to emit 'destroyed' signals to
    // them, therefore we cannot have sender as parent for the adapter.
    connect( sender, SIGNAL( destroyed( QObject* ) ), this, SLOT( Destroy() ) );
}

void SignalAdapter::Destroy()
{
    delete this;
}

void SignalAdapter::Handle()
{
    f_();
}

bool gui::connect( QObject* sender, const char* signal, const std::function< void() >& f )
{
    new gui::SignalAdapter( sender, signal, f );
    return true;
}
