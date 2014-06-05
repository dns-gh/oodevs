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
#include "ObjectNameManager.h"
#include "SubObjectName.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectTypes.h"
#include <tools/Iterator.h>
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer constructor
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
ObjectAttributePrototypeContainer::ObjectAttributePrototypeContainer( const tools::Resolver_ABC< ObjectType, std::string >& resolver,
                                                                      std::unique_ptr< ObjectAttributePrototypeFactory_ABC > factory, QWidget* parent )
    : factory_ ( std::move( factory ) )
    , resolver_( resolver )
    , parent_  ( parent )
{
    ObjectNameManager::getInstance()->SetObjectName( parent_, "ObjectAttributePrototypeContainer" );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer destructor
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
ObjectAttributePrototypeContainer::~ObjectAttributePrototypeContainer()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( parent_->objectName() );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::NotifyUpdated
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeContainer::NotifyUpdated()
{
    tools::Iterator< const ObjectType& > it = resolver_.CreateIterator();
    while( it.HasMoreElements() )
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
        SubObjectName subObject( result.first->first.c_str() );
        for( kernel::ObjectType::T_Capacities::const_iterator it = type.CapacitiesBegin(); result.second && it != type.CapacitiesEnd(); ++it )
            factory_->Create( it->first, *it->second, *result.first->second, parent_ );
        factory_->FinalizeCreate();
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
        current_ = it->second; // throw MASA_EXCEPTION( "Unable to select " + type.GetType() + " object type." );
    SetLoader( 0 );
    Show();
    return current_.get();
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::CheckValidity
// Created: SBO 2009-06-09
// -----------------------------------------------------------------------------
bool ObjectAttributePrototypeContainer::CheckValidity( const kernel::Team_ABC& team ) const
{
    if( current_.get() )
        for( T_AttributeContainer::const_iterator it = current_->begin(); it != current_->end(); ++it )
            if( ! (*it)->CheckValidity( team ) )
                return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::Commit
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeContainer::Commit( const kernel::Team_ABC& team )
{
    if( current_.get() )
        for( T_AttributeContainer::iterator it = current_->begin(); it != current_->end(); ++it )
            (*it)->Commit( team );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::Hide
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeContainer::Hide()
{
    if( current_.get() )
        for( T_AttributeContainer::const_iterator it = current_->begin(); it != current_->end(); ++it )
            (*it)->hide();
    current_.reset();
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::Show
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeContainer::Show()
{
    if( current_.get() )
        for( T_AttributeContainer::const_iterator it = current_->begin(); it != current_->end(); ++it )
            (*it)->show();
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeContainer::SetLoader
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeContainer::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    if( current_.get() )
        for( T_AttributeContainer::const_iterator it = current_->begin(); it != current_->end(); ++it )
            (*it)->SetLoader( loader );
}
