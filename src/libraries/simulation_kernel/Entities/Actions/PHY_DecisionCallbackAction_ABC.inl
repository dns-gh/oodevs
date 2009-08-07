// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: template< typename T > void PHY_DecisionCallbackAction_ABC::Callback
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
template< typename T > 
void PHY_DecisionCallbackAction_ABC::Callback( T value )
{
    decision_.Callback( this, value );
}
