// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "Resource.h"

// -----------------------------------------------------------------------------
// Name: Resource constructor
// Created: HME 2005-11-04
// -----------------------------------------------------------------------------
Resource::Resource( uint nID, std::string strName, std::string strDot )
: strName_ ( strName )
, strDot_  ( strDot  )
, nId_     ( nID     )
{
}

// -----------------------------------------------------------------------------
// Name: Resource destructor
// Created: HME 2005-11-04
// -----------------------------------------------------------------------------
Resource::~Resource()
{
}