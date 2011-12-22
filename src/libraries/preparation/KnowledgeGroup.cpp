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
#include "IdManager.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/KnowledgeGroupType.h" // LTO
#include "clients_kernel/PropertiesDictionary.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( Controller& controller, IdManager& idManager, tools::Resolver_ABC< KnowledgeGroupType, std::string >& types )
    : EntityImplementation< KnowledgeGroup_ABC >( controller, idManager.GetNextId(), "" )
    , type_( ResolveType( "Standard", types ) )
{
    UpdateCommunicationDelay();
    name_ = tools::translate( "KnowledgeGroup", "Knowledge group [%1]" ).arg( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( xml::xistream& xis, Controller& controller, IdManager& idManager, tools::Resolver_ABC< KnowledgeGroupType, std::string >& types )
    : EntityImplementation< KnowledgeGroup_ABC >( controller, xis.attribute< unsigned int >( "id" ), "" )
    , type_( ResolveType( xis.attribute< std::string >( "type" ), types ) )
{
    name_ = xis.attribute< std::string >( "name", "" ).c_str();;
    if( name_.isEmpty() )
        name_ = tools::translate( "KnowledgeGroup", "Knowledge group [%1]" ).arg( id_ );
    UpdateCommunicationDelay(); // LTO
    idManager.Lock( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
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
// Name: KnowledgeGroup::CreateDictionary
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
void KnowledgeGroup::CreateDictionary( Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Entity_ABC& constSelf = static_cast< const Entity_ABC& >( *this );
    dictionary.Register( constSelf, tools::translate( "KnowledgeGroup", "Info/Identifier" ), static_cast< const unsigned long >( id_ ) );
    dictionary.Register( constSelf, tools::translate( "KnowledgeGroup", "Info/Name" ), name_, *this, &KnowledgeGroup::Rename );
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
// Name: KnowledgeGroup::Rename
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void KnowledgeGroup::Rename( const QString& name )
{
    if( name == name_ )
        return;
    name_ = name;
    Touch();
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
// Name: KnowledgeGroup::CheckType
// Created: LDC 2011-05-25
// -----------------------------------------------------------------------------
void KnowledgeGroup::CheckType( std::string& loadingErrors ) const
{
    if( !type_ )
    {
        std::stringstream error;
        error << "Knowledge group " << id_ << ": type unknown reset to 'Standard'\n";
        loadingErrors += error.str();
    }
}
