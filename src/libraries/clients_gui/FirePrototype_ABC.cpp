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
#include "LoadableSpinBox.h"
#include "SubObjectName.h"

#include "clients_kernel/FireClass.h"
#include "clients_kernel/Tools.h"

#include <tools/Iterator.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: FirePrototype_ABC constructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
FirePrototype_ABC::FirePrototype_ABC( QWidget* parent, const tools::Resolver_ABC< kernel::FireClass, std::string >& resolver )
    : ObjectAttributePrototype_ABC( parent, "FirePrototypeABC", tools::translate( "gui::FirePrototype_ABC", "Fire parameters" ) )
    , resolver_( resolver )
    , hasFirePropagation_( false )
{
    gui::SubObjectName subObject( "FirePrototypeABC" );
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    layout->setMargin( 5 );
    layout->addWidget( new QLabel( tools::translate( "gui::FirePrototype_ABC", "Fire Class:" ) ) );
    fireClass_ = new ValuedComboBox< const kernel::FireClass* >( "fireClass", 0 );
    layout->addWidget( fireClass_ );

    layout->addWidget( new QLabel( tools::translate( "gui::FirePrototype_ABC", "Max combustion energy:" ) ) );
    maxCombustionEnergy_ = new LoadableSpinBox( "maxCombustionEnergy", 0, std::numeric_limits< int >::max(), 1, 0 );
    layout->addWidget( maxCombustionEnergy_ );

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
// Name: FirePrototype_ABC::SetHasFirePropagation
// Created: BCI 2011-04-27
// -----------------------------------------------------------------------------
void FirePrototype_ABC::SetHasFirePropagation( bool b)
{
    if( hasFirePropagation_ != b )
    {
        FillTypes();
        hasFirePropagation_ = b;
    }
}

// -----------------------------------------------------------------------------
// Name: FirePrototype_ABC::FillTypes
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
void FirePrototype_ABC::FillTypes()
{
    fireClass_->Clear();
    tools::Iterator< const kernel::FireClass& > it( resolver_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const kernel::FireClass& element = it.NextElement();
        if( hasFirePropagation_ == element.CanPropagate() )
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
bool FirePrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    return fireClass_->count() && fireClass_->GetValue();
}

// -----------------------------------------------------------------------------
// Name: FirePrototype_ABC::SetLoader
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
void FirePrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    maxCombustionEnergy_->SetLoader( loader );
}
