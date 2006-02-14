// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "NBCAttributes.h"

// -----------------------------------------------------------------------------
// Name: NBCAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
NBCAttributes::NBCAttributes()
    : set_( false )
{

}

// -----------------------------------------------------------------------------
// Name: NBCAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
NBCAttributes::~NBCAttributes()
{

}

// -----------------------------------------------------------------------------
// Name: NBCAttributes::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void NBCAttributes::Update( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    if( message.m.attributs_specifiquesPresent )
    {
        if( message.attributs_specifiques.t == T_AttrObjectSpecific_nuage_nbc )
        {
            set_ = true;
            nbcId_  = message.attributs_specifiques.u.nuage_nbc->agent_nbc;
        }
        else if( message.attributs_specifiques.t == T_AttrObjectSpecific_zone_nbc )
        {   set_ = true;
            nbcId_  = message.attributs_specifiques.u.zone_nbc->agent_nbc;
        }
    }
}
