// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "FirePrototype_ABC.h"
#include "clients_kernel/FireClass.h"
#include "clients_kernel/Iterator.h"
#include "Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: FirePrototype_ABC constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
FirePrototype_ABC::FirePrototype_ABC( QWidget* parent, const Resolver_ABC< FireClass >& resolver )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "FirePrototype_ABC", "Fire parameters" ) )
    , resolver_( resolver )
{
    new QLabel( tools::translate( "FirePrototype_ABC", "Fire Class:" ), this );
    fireClass_ = new ValuedComboBox< const FireClass* >( this );
    FillTypes();
}
    
// -----------------------------------------------------------------------------
// Name: FirePrototype_ABC destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
FirePrototype_ABC::~FirePrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePrototype_ABC::FillTypes
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void FirePrototype_ABC::FillTypes()
{
    fireClass_->Clear();
    Iterator< const FireClass& > it( resolver_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const FireClass& element = it.NextElement();
        fireClass_->AddItem( element.GetName(), &element );
    }
}

// -----------------------------------------------------------------------------
// Name: FirePrototype_ABC::showEvent
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void FirePrototype_ABC::showEvent( QShowEvent* e )
{
    FillTypes();
    QGroupBox::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: FirePrototype_ABC::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool FirePrototype_ABC::CheckValidity() const
{
    return fireClass_->count() && fireClass_->GetValue();
}
