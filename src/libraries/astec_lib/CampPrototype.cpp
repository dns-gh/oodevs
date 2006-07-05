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
#include "moc_CampPrototype.cpp"

#include "Agent.h"
#include "Controllers.h"
#include "ASN_Messages.h"
#include "AutomatType.h"

// -----------------------------------------------------------------------------
// Name: CampPrototype::CampPrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CampPrototype::CampPrototype( QWidget* parent, Controllers& controllers )
    : ObjectPrototypeAttributes_ABC( parent, tr( "Camp" ) )
    , controllers_( controllers )
    , attrPrisonners_( 0 )
    , attrRefugees_( 0 )
    , selected_( controllers )
{
    new QLabel( qApp->tr( "TC2:" ), this );
    tc2s_ = new ValuedComboBox< const Agent* >( this );
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: CampPrototype::~CampPrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CampPrototype::~CampPrototype()
{
    Clean();
    controllers_.Remove( *this );
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
    delete attrPrisonners_; attrPrisonners_ = 0;
    delete attrRefugees_; attrRefugees_ = 0;
}

// -----------------------------------------------------------------------------
// Name: CampPrototype::NotifyCreated
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CampPrototype::NotifyCreated( const Agent& agent )
{
    if( tc2s_->GetItemIndex( &agent ) != -1 )
        return;
    if( agent.GetAutomatType() && agent.GetAutomatType()->IsTC2() )
        tc2s_->AddItem( agent.GetName().c_str(), &agent );
}

// -----------------------------------------------------------------------------
// Name: CampPrototype::NotifyDeleted
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CampPrototype::NotifyDeleted( const Agent& agent )
{
    tc2s_->RemoveItem( &agent );
}

// -----------------------------------------------------------------------------
// Name: CampPrototype::NotifyContextMenu
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void CampPrototype::NotifyContextMenu( const Agent& agent, QPopupMenu& menu )
{
    if( agent.GetAutomatType() && agent.GetAutomatType()->IsTC2() )
    {
        selected_ = &agent;
        if( menu.count() )
            menu.insertSeparator();

        menu.insertItem( tr( "TC2 du camp" ), this, SLOT( SetSelected() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CampPrototype::SetSelected
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void CampPrototype::SetSelected()
{
    if( selected_ )
        tc2s_->SetCurrentItem( selected_ );
}
