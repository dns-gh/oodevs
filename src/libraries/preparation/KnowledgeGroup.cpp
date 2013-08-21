// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "KnowledgeGroup.h"
#include "tools/IdManager.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/KnowledgeGroupType.h" // LTO
#include "clients_kernel/PropertiesDictionary.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( kernel::Controllers& controllers, tools::IdManager& idManager, tools::Resolver_ABC< KnowledgeGroupType, std::string >& types, const kernel::Entity_ABC& parent, bool isCrowd )
    : EntityImplementation< KnowledgeGroup_ABC >( controllers.controller_, idManager.GetNextId(), "" )
    , type_( ResolveType( "Standard", types ) )
    , isCrowd_( isCrowd )
    , parentId_( parent.GetId() )
    , controllers_( controllers )
{
    UpdateCommunicationDelay();
    if( isCrowd_ )
        name_ = tools::translate( "KnowledgeGroup", "Crowd" ) + " " + parent.GetName();
    else
        name_ = tools::translate( "KnowledgeGroup", "Knowledge group [%L1]" ).arg( id_ );
    RegisterSelf( *this );
    CreateDictionary( controllers_.controller_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( xml::xistream& xis, kernel::Controllers& controllers, tools::IdManager& idManager, tools::Resolver_ABC< KnowledgeGroupType, std::string >& types )
    : EntityImplementation< KnowledgeGroup_ABC >( controllers.controller_, xis.attribute< unsigned int >( "id" ), "" )
    , type_( ResolveType( xis.attribute< std::string >( "type" ), types ) )
    , isCrowd_( xis.has_attribute( "crowd" ) && xis.attribute< bool >( "crowd" ) == true )
    , controllers_( controllers )
{
    name_ = xis.attribute< std::string >( "name", "" ).c_str();
    if( name_.isEmpty() )
        name_ = tools::translate( "KnowledgeGroup", "Knowledge group [%L1]" ).arg( id_ );
    UpdateCommunicationDelay(); // LTO
    idManager.Lock( id_ );
    RegisterSelf( *this );
    CreateDictionary( controllers_.controller_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    controllers_.Unregister( *this );
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::ResolveType
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
KnowledgeGroupType* KnowledgeGroup::ResolveType( const std::string& typeName, tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types )
{
    KnowledgeGroupType* ret = types.Find( typeName );
    if( !ret )
    {
        tools::Iterator< const KnowledgeGroupType& > it = types.CreateIterator();
        if( !it.HasMoreElements() )
            throw std::exception( "No Knowledge group types defined in physical database." );
        ret = const_cast< KnowledgeGroupType* >( &it.NextElement() );
    }
    return ret;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::NotifyUpdated
// Created: NPT 2013-08-20
// -----------------------------------------------------------------------------
void KnowledgeGroup::NotifyUpdated( const kernel::Team_ABC& team )
{
    if( isCrowd_ && parentId_ && team.GetId() == *parentId_ )
        name_ = tools::translate( "KnowledgeGroup", "Crowd" ) + " " + team.GetName();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::CreateDictionary
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
void KnowledgeGroup::CreateDictionary( Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Entity_ABC& constSelf = static_cast< const Entity_ABC& >( *this );
    dictionary.Register( constSelf, tools::translate( "KnowledgeGroup", "Info/Identifier" ), static_cast< const unsigned long >( id_ ) );
    dictionary.Register( constSelf, tools::translate( "KnowledgeGroup", "Info/Name" ), name_, *static_cast< kernel::EntityImplementation< kernel::KnowledgeGroup_ABC >* >( this ), &KnowledgeGroup::Rename );
    dictionary.Register( constSelf, tools::translate( "KnowledgeGroup", "Type/Name" ), type_, *this, &KnowledgeGroup::SetType );
    dictionary.Register( constSelf, tools::translate( "KnowledgeGroup", "Type/Delay" ), static_cast< const KnowledgeGroup* >( this )->communicationDelay_ ); // LTO
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::UpdateCommunicationDelay
// Created: SYD 2009-11-23
// LTO
// -----------------------------------------------------------------------------
void KnowledgeGroup::UpdateCommunicationDelay()
{
    communicationDelay_ = type_ ? type_->ShowCommunicationDelay() : "0m0s";
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SetType
// Created: SYD 2009-11-20
// LTO
// -----------------------------------------------------------------------------
void KnowledgeGroup::SetType( KnowledgeGroupType* const& type )
{
    if( !type || type == type_ )
        return;
    type_ = type;
    UpdateCommunicationDelay();
    Touch();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SerializeAttributes
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void KnowledgeGroup::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", id_ )
        << xml::attribute( "type", type_->GetName() ) // type_ cannot be null (else, exception thrown in constructor)
        << xml::attribute( "name", name_ );
    if( isCrowd_ )
        xos << xml::attribute( "crowd", true );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup IsActivated
// Created: SLG 2009-12-18
// LTO
// -----------------------------------------------------------------------------
bool KnowledgeGroup::IsActivated() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::IsJammed
// Created: JSR 2013-06-20
// -----------------------------------------------------------------------------
bool KnowledgeGroup::IsJammed() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::IsCrowd
// Created: JSR 2013-07-03
// -----------------------------------------------------------------------------
bool KnowledgeGroup::IsCrowd() const
{
    return isCrowd_;
}
