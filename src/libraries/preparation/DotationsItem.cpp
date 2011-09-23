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

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DotationsItem constructor
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
DotationsItem::DotationsItem( kernel::Controller& controller, kernel::Entity_ABC& owner, kernel::PropertiesDictionary& dico, const QString& propertyName, tools::Resolver< Dotation >& dotations )
    : controller_( controller )
    , owner_( owner )
    , dico_( dico )
    , propertyName_( propertyName )
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
    dico_.Register( owner_, propertyName_ + "/" + dotation.type_->GetName().c_str(), ( int& ) dotation.quantity_ );
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
// Name: DotationsItem::CountDotations
// Created: ABR 2011-09-22
// -----------------------------------------------------------------------------
unsigned long DotationsItem::CountDotations() const
{
    return dotations_.Count();
}

// -----------------------------------------------------------------------------
// Name: DotationsItem::Clear
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void DotationsItem::Clear()
{
    tools::Iterator< const Dotation& > it = dotations_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const Dotation& dotation = it.NextElement();
        dico_.Remove( propertyName_ + "/" + dotation.type_->GetName().c_str() );
    }
    dotations_.DeleteAll();
    controller_.Delete( kernel::DictionaryUpdated( owner_, propertyName_ ) );
}

// -----------------------------------------------------------------------------
// Name: DotationsItem::Update
// Created: SBO 2006-11-15
// -----------------------------------------------------------------------------
void DotationsItem::Update()
{
    controller_.Update( kernel::DictionaryUpdated( owner_, propertyName_ ) );
}

// -----------------------------------------------------------------------------
// Name: DotationsItem::CreateIterator
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
tools::Iterator< const Dotation& > DotationsItem::CreateIterator() const
{
    return dotations_.CreateIterator();
}
