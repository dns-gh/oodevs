// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "CrossingSitePrototype.h"
#include "DisplayBuilder.h"

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype constructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype::CrossingSitePrototype( DisplayBuilder& builder )
    : display_( builder )
{
    display_.AddGroup( "Site de franchissement" )
                .AddSpinBox( "Largeur:", 1, 10000, 10 )
                .AddSpinBox( "Profondeur:", 0, 1000, 10 )
                .AddSpinBox( "Vitesse courant:", 0, 100, 1 )
                .AddCheckBox( "Berges à aménager:" );
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype destructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype::~CrossingSitePrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::CheckValidity
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
bool CrossingSitePrototype::CheckValidity() const
{
    return true;
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::Serialize
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CrossingSitePrototype::Serialize( ASN1T_MagicActionCreateObject& msg ) const
{
    
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::Show
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CrossingSitePrototype::Show() const
{
    display_.Group( "Site de franchissement" ).show();
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::Hide
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CrossingSitePrototype::Hide()
{
    display_.Group( "Site de franchissement" ).Hide();
}
