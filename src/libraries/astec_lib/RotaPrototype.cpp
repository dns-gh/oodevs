// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "RotaPrototype.h"
#include "NBCAgent.h"
#include "ASN_Messages.h"
#include "ValuedListItem.h"
#include "Iterator.h"
#include "RichLabel.h"

// -----------------------------------------------------------------------------
// Name: RotaPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
RotaPrototype::RotaPrototype( QWidget* parent, const Resolver< NBCAgent >& resolver )
    : ObjectPrototypeAttributes_ABC( parent, tr( "ROTA" ) )
    , attr_( 0 )
{
    new QLabel( tr( "Danger:" ), this );
    danger_ = new QSpinBox( 0, 100, 1, this );

    nbcAgentsLabel_ = new RichLabel( tr( "Agent_ABC(s) NBC:" ), this );
    nbcAgents_ = new QListView( this );
    nbcAgents_->setSelectionMode( QListView::Multi );
    nbcAgents_->setMinimumHeight( 3 * nbcAgents_->height() ); // 3 lines visible
    nbcAgents_->addColumn( tr( "Type" ) );

    Iterator< const NBCAgent& > it( resolver.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const NBCAgent& element = it.NextElement();
        ValuedListItem* item = new ValuedListItem( nbcAgents_ );
        item->Set( &element, element.GetName().c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: RotaPrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
RotaPrototype::~RotaPrototype()
{
    Clean();
}

// -----------------------------------------------------------------------------
// Name: RotaPrototype::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool RotaPrototype::CheckValidity() const
{
    if( !GetAgentCount() )
    {
        nbcAgentsLabel_->Warn( 3000 );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: RotaPrototype::Serialize
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void RotaPrototype::Serialize( ASN1T_MagicActionCreateObject& msg )
{
    if( msg.type != EnumObjectType::rota )
        return;

    attr_ = new ASN1T_AttrObjectROTA();
    
    attr_->agents_nbc.n = GetAgentCount();
    attr_->agents_nbc.elem  = new uint[ attr_->agents_nbc.n ];
    unsigned i = 0;
    for( QListViewItem* item = nbcAgents_->firstChild(); item != 0; item = item->nextSibling() )
        if( item->isSelected() )
            attr_->agents_nbc.elem[i++] = static_cast< ValuedListItem* >( item )->GetValue< const NBCAgent* >()->GetId();

    attr_->niveau_danger = danger_->text().toUInt();
    msg.m.attributs_specifiquesPresent = 1;
    msg.attributs_specifiques.t        = T_AttrObjectSpecific_rota;
    msg.attributs_specifiques.u.rota   = attr_;
}

// -----------------------------------------------------------------------------
// Name: RotaPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void RotaPrototype::Clean()
{
    delete attr_; attr_ = 0;
}

// -----------------------------------------------------------------------------
// Name: RotaPrototype::GetAgentCount
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
unsigned RotaPrototype::GetAgentCount() const
{
    unsigned selected = 0;
    for( QListViewItem* item = nbcAgents_->firstChild(); item != 0; item = item->nextSibling() )
        if( item->isSelected() )
            ++selected;
    return selected;
}
