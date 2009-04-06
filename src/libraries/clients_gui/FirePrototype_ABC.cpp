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

using namespace gui;

// -----------------------------------------------------------------------------
// Name: FirePrototype_ABC constructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
FirePrototype_ABC::FirePrototype_ABC( QWidget* parent, const kernel::Resolver_ABC< kernel::FireClass >& resolver )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "FirePrototype_ABC", "Fire parameters" ) )
    , resolver_( resolver )
{
    new QLabel( tools::translate( "FirePrototype_ABC", "Fire Class:" ), this );
    fireClass_ = new ValuedComboBox< const kernel::FireClass* >( this );
    FillTypes();
}
    
// -----------------------------------------------------------------------------
// Name: FirePrototype_ABC destructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
FirePrototype_ABC::~FirePrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePrototype_ABC::FillTypes
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
void FirePrototype_ABC::FillTypes()
{
    fireClass_->Clear();
    kernel::Iterator< const kernel::FireClass& > it( resolver_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const kernel::FireClass& element = it.NextElement();
        fireClass_->AddItem( element.GetName(), &element );
    }
}

// -----------------------------------------------------------------------------
// Name: FirePrototype_ABC::showEvent
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
void FirePrototype_ABC::showEvent( QShowEvent* e )
{
    FillTypes();
    ObjectAttributePrototype_ABC::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: FirePrototype_ABC::CheckValidity
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
bool FirePrototype_ABC::CheckValidity() const
{
    return fireClass_->count() && fireClass_->GetValue();
}
