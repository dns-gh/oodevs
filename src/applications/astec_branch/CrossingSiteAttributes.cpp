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
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CrossingSiteAttributes::CrossingSiteAttributes( Controller& controller )
    : controller_( controller )
    , set_( false )
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
// Name: CrossingSiteAttributes::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void CrossingSiteAttributes::UpdateData( const T& message )
{
    set_ = true;
    if( message.m.attributs_specifiquesPresent 
     && message.attributs_specifiques.t == T_AttrObjectSpecific_site_franchissement )
    {
        width_ = message.attributs_specifiques.u.site_franchissement->largeur;
        depth_ = message.attributs_specifiques.u.site_franchissement->profondeur;
        speed_ = message.attributs_specifiques.u.site_franchissement->vitesse_courant;
        needsConstruction_ = message.attributs_specifiques.u.site_franchissement->berges_a_amenager;
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::DoUpdate( const ASN1T_MsgObjectUpdate& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::DoUpdate( const ASN1T_MsgObjectCreation& message )
{
    UpdateData( message );
}
