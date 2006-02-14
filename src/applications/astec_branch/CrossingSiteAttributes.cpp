// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "CrossingSiteAttributes.h"

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CrossingSiteAttributes::CrossingSiteAttributes()
    : set_( false )
{

}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CrossingSiteAttributes::~CrossingSiteAttributes()
{

}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::Update( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    set_ = true;
    if( message.m.attributs_specifiquesPresent 
     && message.attributs_specifiques.t == T_AttrObjectSpecific_site_franchissement )
    {
        width_ = message.attributs_specifiques.u.site_franchissement->largeur;
        depth_ = message.attributs_specifiques.u.site_franchissement->profondeur;
        speed_ = message.attributs_specifiques.u.site_franchissement->vitesse_courant;
        needsConstruction_ = message.attributs_specifiques.u.site_franchissement->berges_a_amenager;
    }
}
