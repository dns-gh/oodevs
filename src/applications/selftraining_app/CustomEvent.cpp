// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "CustomEvent.h"

// -----------------------------------------------------------------------------
// Name: CustomEvent constructor
// Created: NPT 2012-12-12
// -----------------------------------------------------------------------------
CustomEvent::CustomEvent( int type, void *data /*= 0*/ )
    : QEvent( static_cast< QEvent::Type >( QEvent::User + type ) )
    , data_( data )
{
        // NOTHING
}
// -----------------------------------------------------------------------------
// Name: CustomEvent destructor
// Created: NPT 2012-12-12
// -----------------------------------------------------------------------------
CustomEvent::~CustomEvent()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CustomEvent::*data
// Created: NPT 2012-12-12
// -----------------------------------------------------------------------------
void* CustomEvent::GetData() const
{
    return data_;
}
// -----------------------------------------------------------------------------
// Name: CustomEvent::setData
// Created: NPT 2012-12-12
// -----------------------------------------------------------------------------
void CustomEvent::SetData( void* aData )
{
    data_ = reinterpret_cast<QEventPrivate *>( aData );
}