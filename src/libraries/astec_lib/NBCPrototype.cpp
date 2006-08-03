// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "NBCPrototype.h"
#include "NBCAgent.h"
#include "Iterator.h"
#include "ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: NBCPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
NBCPrototype::NBCPrototype( QWidget* parent, const Resolver< NBCAgent >& resolver )
    : ObjectPrototypeAttributes_ABC( parent, tr( "Paramètres NBC" ) )
    , nbcArea_( 0 )
    , nbcCloud_( 0 )
{
    new QLabel( tr( "Agent_ABC NBC:" ), this );
    nbcAgents_ = new ValuedComboBox< const NBCAgent* >( this );

    Iterator< const NBCAgent& > it( resolver.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const NBCAgent& element = it.NextElement();
        nbcAgents_->AddItem( element.GetName().c_str(), &element );
    }
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
// Name: NBCPrototype::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool NBCPrototype::CheckValidity() const
{
    return nbcAgents_->count() && nbcAgents_->GetValue();
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype::Serialize
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void NBCPrototype::Serialize( ASN1T_MagicActionCreateObject& msg )
{
    if( msg.type == EnumObjectType::nuage_nbc )
    {
        nbcCloud_ = new ASN1T_AttrObjectNuageNBC();
        nbcCloud_->agent_nbc  = nbcAgents_->GetValue()->GetId();
        msg.m.attributs_specifiquesPresent    = 1;
        msg.attributs_specifiques.t           = T_AttrObjectSpecific_nuage_nbc;
        msg.attributs_specifiques.u.nuage_nbc = nbcCloud_;
    }
    else if( msg.type == EnumObjectType::zone_nbc )
    {
        nbcArea_ = new ASN1T_AttrObjectZoneNBC();
        nbcArea_->agent_nbc  = nbcAgents_->GetValue()->GetId();
        msg.m.attributs_specifiquesPresent    = 1;
        msg.attributs_specifiques.t           = T_AttrObjectSpecific_zone_nbc;
        msg.attributs_specifiques.u.zone_nbc  = nbcArea_;
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
