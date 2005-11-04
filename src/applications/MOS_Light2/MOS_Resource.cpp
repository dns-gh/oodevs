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

#include "MOS_Light2_pch.h"
#include "MOS_Resource.h"

// -----------------------------------------------------------------------------
// Name: MOS_Resource constructor
// Created: HME 2005-11-04
// -----------------------------------------------------------------------------
MOS_Resource::MOS_Resource( uint nID, std::string strName, std::string strDot )
: strName_ ( strName )
, strDot_  ( strDot  )
, nId_     ( nID     )
{
}

// -----------------------------------------------------------------------------
// Name: MOS_Resource destructor
// Created: HME 2005-11-04
// -----------------------------------------------------------------------------
MOS_Resource::~MOS_Resource()
{
}