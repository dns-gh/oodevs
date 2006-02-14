// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-02 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "Object_NBC.h"

#include "App.h"

// -----------------------------------------------------------------------------
// Name: Object_NBC constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_NBC::Object_NBC( ASN1T_EnumObjectType eType )
    : Object_ABC ( eType )
    , nAgentNbcId_       ( (uint)-1 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: Object_NBC constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_NBC::Object_NBC( const ASN1T_MsgObjectCreation& asnMsg )
    : Object_ABC ( asnMsg )
    , nAgentNbcId_       ( 0 )
{
    if( GetType() == EnumObjectType::nuage_nbc )
        nAgentNbcId_ = asnMsg.attributs_specifiques.u.nuage_nbc->agent_nbc;
    else if( GetType() == EnumObjectType::zone_nbc )
        nAgentNbcId_ = asnMsg.attributs_specifiques.u.zone_nbc->agent_nbc;
}
// -----------------------------------------------------------------------------
// Name: Object_NBC destructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_NBC::~Object_NBC()
{
    // NOTHING
}
//
