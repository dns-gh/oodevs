// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "CampPrototype.h"
#include "Agent.h"
#include "Controllers.h"
#include "ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: CampPrototype::CampPrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CampPrototype::CampPrototype( QWidget* parent, Controllers& controllers, const Resolver< Agent >& agents )
    : ObjectPrototypeAttributes_ABC( parent, tr( "Camp" ) )
    , attrPrisonners_( 0 )
    , attrRefugees_( 0 )
{
    new QLabel( qApp->tr( "TC2:" ), this );
    tc2s_ = new ValuedComboBox< const Agent* >( this );

    Iterator< const Agent& > it( agents.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const Agent& element = it.NextElement();
//        if( element.IsTC2() ) // $$$$ SBO 2006-04-19: 
            tc2s_->AddItem( element.GetName().c_str(), &element );
    }

    controllers.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: CampPrototype::~CampPrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CampPrototype::~CampPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CampPrototype::CheckValidity
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
bool CampPrototype::CheckValidity() const
{
    return tc2s_->GetValue();
}

// -----------------------------------------------------------------------------
// Name: CampPrototype::Serialize
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CampPrototype::Serialize( ASN1T_MagicActionCreateObject& msg )
{
    if( msg.type == EnumObjectType::camp_prisonniers )
    {
        attrPrisonners_ = new ASN1T_AttrObjectCampPrisonniers();
        attrPrisonners_->tc2 = tc2s_->GetValue()->GetId();
        msg.m.attributs_specifiquesPresent    = 1;
        msg.attributs_specifiques.t           = T_AttrObjectSpecific_camp_prisonniers;
        msg.attributs_specifiques.u.camp_prisonniers= attrPrisonners_;
    }
    else if( msg.type == EnumObjectType::camp_refugies )
    {
        attrRefugees_ = new ASN1T_AttrObjectCampRefugies();
        attrRefugees_->tc2 = tc2s_->GetValue()->GetId();
        msg.m.attributs_specifiquesPresent    = 1;
        msg.attributs_specifiques.t           = T_AttrObjectSpecific_camp_refugies;
        msg.attributs_specifiques.u.camp_refugies = attrRefugees_;
    }
}

// -----------------------------------------------------------------------------
// Name: CampPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void CampPrototype::Clean()
{
    delete attrPrisonners_;
    delete attrRefugees_;
}

// -----------------------------------------------------------------------------
// Name: CampPrototype::NotifyCreated
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CampPrototype::NotifyCreated( const Agent& agent )
{
    if( tc2s_->GetItemIndex( &agent ) != -1 )
        return;
    // $$$$ SBO 2006-04-19: condition: must be a tc2?
    tc2s_->AddItem( agent.GetName().c_str(), &agent );
}

// -----------------------------------------------------------------------------
// Name: CampPrototype::NotifyUpdated
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CampPrototype::NotifyUpdated( const Agent& agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CampPrototype::NotifyDeleted
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CampPrototype::NotifyDeleted( const Agent& agent )
{
    tc2s_->RemoveItem( &agent );
}
