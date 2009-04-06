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
#include "clients_kernel/Iterator.h"

#include <xeumeuleu/xml.h>
#include <boost/bind.hpp>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer constructor
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
ObjectAttributePrototypeContainer::ObjectAttributePrototypeContainer( const Resolver_ABC< ObjectType, std::string >& resolver, const ObjectAttributePrototypeFactory_ABC& factory, QWidget* parent )
    : factory_ ( factory )
    , resolver_ ( resolver )
    , select_ ( 0 )
    , parent_ ( parent )
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
    Iterator< const ObjectType& > it = resolver_.CreateIterator();
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
    if ( attributes_.find( type.GetType() ) == attributes_.end() )
    {
        std::pair< IT_AttributesPrototypes, bool > result = attributes_.insert( std::make_pair( type.GetType(), new T_AttributeContainer() ) );
        for( kernel::ObjectType::T_Capacities::const_iterator it = type.CapacitiesBegin(); result.second && it != type.CapacitiesEnd(); ++it )
            factory_.Create( it->first, *it->second, *result.first->second, parent_ );    
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
    if ( it != attributes_.end() )
        select_ = it->second.get(); // throw std::runtime_error( "Unable to select " + type.GetType() + " object type." );        
    Show();
    return select_;
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::Commit
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeContainer::Commit()
{
    if ( select_ )
        std::for_each( select_->begin(), select_->end(), boost::bind( &ObjectAttributePrototype_ABC::Commit, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::Commit
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeContainer::Clean()
{
    if ( select_ )
        std::for_each( select_->begin(), select_->end(), boost::bind( &ObjectAttributePrototype_ABC::Clean, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::Hide
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeContainer::Hide()
{
    if ( select_ )
        std::for_each( select_->begin(), select_->end(), boost::bind( &ObjectAttributePrototype_ABC::hide, _1 ) );
    select_ = 0;
}
    
// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::Show
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeContainer::Show()
{
    if ( select_ )
        std::for_each( select_->begin(), select_->end(), boost::bind( &ObjectAttributePrototype_ABC::show, _1 ) );
}
