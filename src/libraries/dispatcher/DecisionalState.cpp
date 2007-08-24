// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "DecisionalState.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: DecisionalState constructor
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
DecisionalState::DecisionalState()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DecisionalState destructor
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
DecisionalState::~DecisionalState()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DecisionalState::Clear
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
void DecisionalState::Clear()
{
    for( std::map< std::string, std::string >::iterator it = decisionalInfos_.begin(); it != decisionalInfos_.end(); ++it )
        it->second.clear();
}

// -----------------------------------------------------------------------------
// Name: DecisionalState::Update
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
void DecisionalState::Update( const ASN1T_MsgDecisionalState& asnMsg )
{
    decisionalInfos_[ asnMsg.key ] = decisionalInfos_[ asnMsg.value ];
}

// -----------------------------------------------------------------------------
// Name: DecisionalState::Send
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
void DecisionalState::Send( unsigned id, ClientPublisher_ABC& publisher ) const
{
    for( std::map< std::string, std::string >::const_iterator it = decisionalInfos_.begin(); it != decisionalInfos_.end(); ++it )
    {
        AsnMsgSimToClientDecisionalState asn;
        asn().oid   = id;
        asn().key   = it->first.c_str();
        asn().value = it->second.c_str();
        asn.Send( publisher );
    }
}
