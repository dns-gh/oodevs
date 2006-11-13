// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "DotationsItem.h"
#include "Dotation.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DictionaryUpdated.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_gui/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DotationsItem constructor
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
DotationsItem::DotationsItem( kernel::Controller& controller, kernel::Entity_ABC& owner, kernel::PropertiesDictionary& dico, kernel::Resolver< Dotation >& dotations )
    : controller_( controller )
    , owner_( owner )
    , dico_( dico )
    , dotations_( dotations )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: DotationsItem destructor
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
DotationsItem::~DotationsItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationsItem::AddDotation
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
void DotationsItem::AddDotation( const Dotation& dotation )
{
    dico_.Register( owner_, tools::translate( "DotationsItem", "Dotations/" ) + dotation.type_->GetCategory(), (int&)dotation.quantity_ );
    controller_.Update( kernel::DictionaryUpdated( owner_, "Dotations" ) );
}

// -----------------------------------------------------------------------------
// Name: DotationsItem::AddDotation
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void DotationsItem::AddDotation( const DotationType& type, unsigned long quantity )
{
    // $$$$ SBO 2006-11-10: change to SetDotation, update quantity instead of rebuilding whole dotation list
    Dotation* dotation = new Dotation( type, quantity );
    dotations_.Register( type.GetId(), *dotation );
    AddDotation( *dotation );
}

// -----------------------------------------------------------------------------
// Name: DotationsItem::Clear
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void DotationsItem::Clear()
{
    Iterator< const Dotation& > it = dotations_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const Dotation& dotation = it.NextElement();
        dico_.Remove( tools::translate( "DotationsItem", "Dotations/" ) + dotation.type_->GetCategory() );
    }
    dotations_.DeleteAll();
    controller_.Delete( kernel::DictionaryUpdated( owner_, "Dotations" ) );
}

// -----------------------------------------------------------------------------
// Name: kernel::Iterator< const Dotation& > DotationsItem::CreateIterator
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
kernel::Iterator< const Dotation& > DotationsItem::CreateIterator() const
{
    return dotations_.CreateIterator();
}
