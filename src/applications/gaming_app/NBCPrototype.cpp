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
        nbcCloud_ = new ASN1T_AttrObjectNuageNBC();
        nbcCloud_->agent_nbc  = nbcAgents_->GetValue()->GetId();
        msg_.m.attributs_specifiquesPresent    = 1;
        msg_.attributs_specifiques.t           = T_AttrObjectSpecific_nuage_nbc;
        msg_.attributs_specifiques.u.nuage_nbc = nbcCloud_;
    }
    else if( msg_.type == EnumObjectType::zone_nbc )
    {
        nbcArea_ = new ASN1T_AttrObjectZoneNBC();
        nbcArea_->agent_nbc  = nbcAgents_->GetValue()->GetId();
        msg_.m.attributs_specifiquesPresent    = 1;
        msg_.attributs_specifiques.t           = T_AttrObjectSpecific_zone_nbc;
        msg_.attributs_specifiques.u.zone_nbc  = nbcArea_;
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
