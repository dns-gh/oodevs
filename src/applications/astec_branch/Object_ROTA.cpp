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
#include "Object_ROTA.h"

#include "App.h"

// -----------------------------------------------------------------------------
// Name: Object_ROTA constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ROTA::Object_ROTA( ASN1T_EnumObjectType eType /* = EnumObjectType::rota */)
    : Object_ABC ( eType )
    , nDanger_           ( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: Object_ROTA constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ROTA::Object_ROTA( const ASN1T_MsgObjectCreation& asnMsg )
    : Object_ABC ( asnMsg )
    , nDanger_           ( 0 )
{
    nDanger_ = asnMsg.attributs_specifiques.u.rota->niveau_danger;
    nbcAgents_.clear();
    for( uint i = 0; i < asnMsg.attributs_specifiques.u.rota->agents_nbc.n; ++i )
        nbcAgents_.push_back( asnMsg.attributs_specifiques.u.rota->agents_nbc.elem[i] );
}
// -----------------------------------------------------------------------------
// Name: Object_ROTA destructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ROTA::~Object_ROTA()
{
    nbcAgents_.clear();
}

