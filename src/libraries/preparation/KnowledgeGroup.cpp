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
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h" // LTO
#include "clients_kernel/Controller.h"
#include "clients_kernel/KnowledgeGroupType.h" // LTO
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Team_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( Controller& controller, IdManager& idManager, tools::Resolver_ABC< KnowledgeGroupType, std::string >& types )
    : EntityImplementation< KnowledgeGroup_ABC >( controller, idManager.GetNextId(), "" )
    , type_( types.Find( "Standard" ) ) // $$$$ SBO 2006-11-17: Hard coded default
    , isActivated_( true ) // LTO
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
KnowledgeGroup::KnowledgeGroup( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager, tools::Resolver_ABC< KnowledgeGroupType, std::string >& types )
    : EntityImplementation< KnowledgeGroup_ABC >( controller, xis.attribute< unsigned int >( "id" ), "" )
    , type_( types.Find( xis.attribute< std::string >( "type" ) ) )
{
    std::string name;
    xis >> xml::optional >> xml::attribute( "name", name );
    UpdateCommunicationDelay(); // LTO
    name_ = name.empty() ? tools::translate( "KnowledgeGroup", "Knowledge group [%1]" ).arg( id_ ) : name.c_str();
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
// Name: KnowledgeGroup::CreateDictionary
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
void KnowledgeGroup::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "KnowledgeGroup", "Info/Identifier" ), (const unsigned long)id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "KnowledgeGroup", "Info/Name" ), name_, *this, &KnowledgeGroup::Rename );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "KnowledgeGroup", "Type/Name" ), type_, *this, &KnowledgeGroup::SetType );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "KnowledgeGroup", "Type/Delay" ), ((const KnowledgeGroup*)this)->communicationDelay_ ); // LTO
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
void KnowledgeGroup::SetType( kernel::KnowledgeGroupType* const& type )
{
    if( type == type_ )
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
        << xml::attribute( "type", type_ ? type_->GetName() : "Standard" ) // $$$$ LDC: Same Hard coded default as in constructor
        << xml::attribute( "name", name_ );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup IsActivated
// Created: SLG 2009-12-18
// LTO
// -----------------------------------------------------------------------------
bool KnowledgeGroup::IsActivated() const
{
    return isActivated_;
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
