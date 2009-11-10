// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "NBCTypePrototype_ABC.h"
#include "clients_kernel/NBCTypeAgent.h"
#include "clients_kernel/Iterator.h"
#include "clients_kernel/Units.h"
#include "Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: NBCTypePrototype_ABC constructor
// Created: RFT 2006-04-20
// -----------------------------------------------------------------------------
NBCTypePrototype_ABC::NBCTypePrototype_ABC( QWidget* parent, const Resolver_ABC< NBCTypeAgent >& resolver )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "NBCTypePrototype_ABC", "NBC parameters" ) )
    , resolver_( resolver )
{
    new QLabel( tools::translate( "NBCTypePrototype_ABC", "NBC Agent:" ), this );
    nbcAgents_ = new ValuedComboBox< const NBCTypeAgent* >( this );
    FillTypes();

    new QLabel( tools::translate( "NBCTypePrototype_ABC", "Source Life Duration:" ), this );
    sourceLifeDuration_ = new QSpinBox( 0, 100, 1, this );
    sourceLifeDuration_->setSuffix( Units::hours );
}
    
// -----------------------------------------------------------------------------
// Name: NBCTypePrototype_ABC destructor
// Created: RFT 2006-04-20
// -----------------------------------------------------------------------------
NBCTypePrototype_ABC::~NBCTypePrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCTypePrototype_ABC::FillTypes
// Created: RFT 2006-08-11
// -----------------------------------------------------------------------------
void NBCTypePrototype_ABC::FillTypes()
{
    nbcAgents_->Clear();
    Iterator< const NBCTypeAgent& > it( resolver_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const NBCTypeAgent& element = it.NextElement();
        nbcAgents_->AddItem( element.GetName(), &element );
    }
}

// -----------------------------------------------------------------------------
// Name: NBCTypePrototype_ABC::showEvent
// Created: RFT 2006-08-11
// -----------------------------------------------------------------------------
void NBCTypePrototype_ABC::showEvent( QShowEvent* e )
{
    FillTypes();
    QGroupBox::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: NBCTypePrototype_ABC::CheckValidity
// Created: RFT 2006-04-20
// -----------------------------------------------------------------------------
bool NBCTypePrototype_ABC::CheckValidity() const
{
    return nbcAgents_->count() && nbcAgents_->GetValue();
}
