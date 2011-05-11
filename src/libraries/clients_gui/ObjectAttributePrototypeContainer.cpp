// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ObjectAttributePrototypeContainer.h"
#include "ObjectAttributePrototype_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectTypes.h"
#include "tools/Iterator.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer constructor
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
ObjectAttributePrototypeContainer::ObjectAttributePrototypeContainer( const tools::Resolver_ABC< ObjectType, std::string >& resolver, const ObjectAttributePrototypeFactory_ABC& factory, QWidget* parent )
    : factory_ ( factory )
    , resolver_( resolver )
    , current_ ()
    , parent_  ( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer destructor
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
ObjectAttributePrototypeContainer::~ObjectAttributePrototypeContainer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::NotifyUpdated
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeContainer::NotifyUpdated()
{
    tools::Iterator< const ObjectType& > it = resolver_.CreateIterator();
    while ( it.HasMoreElements() )
        Load( it.NextElement() );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::Load
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeContainer::Load( const kernel::ObjectType& type )
{
    // load qwidget container if necessary
    if( attributes_.find( type.GetType() ) == attributes_.end() )
    {
        std::pair< IT_AttributesPrototypes, bool > result = attributes_.insert( std::make_pair( type.GetType(), new T_AttributeContainer() ) );
        for( kernel::ObjectType::T_Capacities::const_iterator it = type.CapacitiesBegin(); result.second && it != type.CapacitiesEnd(); ++it )
            factory_.Create( it->first, *it->second, *result.first->second, parent_ );
        factory_.FinalizeCreate();
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::Select
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
const ObjectAttributePrototypeContainer::T_AttributeContainer* ObjectAttributePrototypeContainer::Select( const kernel::ObjectType& type )
{
    CIT_AttributesPrototypes it = attributes_.find( type.GetType() );
    Hide();
    if( it != attributes_.end() )
        current_ = it->second; // throw std::runtime_error( "Unable to select " + type.GetType() + " object type." );
    SetLoader( 0 );
    Show();
    return current_.get();
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::CheckValidity
// Created: SBO 2009-06-09
// -----------------------------------------------------------------------------
bool ObjectAttributePrototypeContainer::CheckValidity() const
{
    if( current_.get() )
        for( T_AttributeContainer::const_iterator it = current_->begin(); it != current_->end(); ++it )
            if( ! (*it)->CheckValidity() )
                return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::Commit
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeContainer::Commit()
{
    if( current_.get() )
        for( T_AttributeContainer::iterator it = current_->begin(); it != current_->end(); ++it )
            (*it)->Commit();
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::Hide
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeContainer::Hide()
{
    if( current_.get() )
        std::for_each( current_->begin(), current_->end(), boost::bind( &ObjectAttributePrototype_ABC::hide, _1 ) );
    current_.reset();
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::Show
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeContainer::Show()
{
    if( current_.get() )
        std::for_each( current_->begin(), current_->end(), boost::bind( &ObjectAttributePrototype_ABC::show, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::SetLoader
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeContainer::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    if( current_.get() )
        std::for_each( current_->begin(), current_->end(), boost::bind( &ObjectAttributePrototype_ABC::SetLoader, _1, loader ) );
}
