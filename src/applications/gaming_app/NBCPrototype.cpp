// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "NBCPrototype.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_kernel/Iterator.h"
#include "gaming/ASN_Messages.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: NBCPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
NBCPrototype::NBCPrototype( QWidget* parent, const Resolver_ABC< NBCAgent >& resolver, ASN1T_MagicActionCreateObject& msg )
    : NBCPrototype_ABC( parent, resolver )
    , msg_( msg ) 
    , nbcArea_( 0 )
    , nbcCloud_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: NBCPrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
NBCPrototype::~NBCPrototype()
{
    Clean();
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void NBCPrototype::Commit()
{
    if( msg_.type == EnumObjectType::nuage_nbc )
    {
        nbcCloud_ = new ASN1T_ObjectAttributesNbcCloud();
        nbcCloud_->nbc_agent  = nbcAgents_->GetValue()->GetId();
        msg_.m.specific_attributesPresent    = 1;
        msg_.specific_attributes.t           = T_ObjectAttributesSpecific_nbc_cloud;
        msg_.specific_attributes.u.nbc_cloud = nbcCloud_;
    }
    else if( msg_.type == EnumObjectType::zone_nbc )
    {
        nbcArea_ = new ASN1T_ObjectAttributesNbcZone();
        nbcArea_->nbc_agent  = nbcAgents_->GetValue()->GetId();
        msg_.m.specific_attributesPresent    = 1;
        msg_.specific_attributes.t           = T_ObjectAttributesSpecific_nbc_zone;
        msg_.specific_attributes.u.nbc_zone  = nbcArea_;
    }
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void NBCPrototype::Clean()
{
    delete nbcArea_; nbcArea_ = 0;
    delete nbcCloud_; nbcCloud_ = 0;
}
