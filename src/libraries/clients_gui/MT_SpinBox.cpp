// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "MT_SpinBox.h"
#include "moc_MT_SpinBox.cpp"

// -----------------------------------------------------------------------------
// Name: MT_SpinBox constructor
// Created: SBO 2005-11-07
// -----------------------------------------------------------------------------
MT_SpinBox::MT_SpinBox( QWidget * parent /*= 0*/, const char* name /*= 0*/ )
: QSpinBox( parent, name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MT_SpinBox constructor
// Created: SBO 2005-11-07
// -----------------------------------------------------------------------------
MT_SpinBox::MT_SpinBox( int minValue, int maxValue, int step /*= 1*/, QWidget * parent /*= 0*/, const char * name /*= 0*/ )
: QSpinBox( minValue, maxValue, step, parent, name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MT_SpinBox destructor
// Created: SBO 2005-11-07
// -----------------------------------------------------------------------------
MT_SpinBox::~MT_SpinBox()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MT_SpinBox::eventFilter
// Created: SBO 2005-11-07
// -----------------------------------------------------------------------------
bool MT_SpinBox::eventFilter( QObject* /*pSender*/, QEvent* pEvent )
{
    switch( pEvent->type() )
    {
        case QEvent::KeyPress:
            if(    ( ( QKeyEvent* )pEvent )->key() == Qt::Key_Enter
                || ( ( QKeyEvent* )pEvent )->key() == Qt::Key_Return )
                emit( enterPressed() );
            return false;
    }
    return false;
}
