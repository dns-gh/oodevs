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
#include "clients_kernel/KnowledgeGroupType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( Controller& controller, IdManager& idManager, Resolver_ABC< KnowledgeGroupType, std::string >& types )
    : EntityImplementation< KnowledgeGroup_ABC >( controller, idManager.GetNextId(), "" )
    , type_( types.Find( "Standard" ) ) // $$$$ SBO 2006-11-17: Hard coded default
{
    name_ = tools::translate( "KnowledgeGroup", "Knowledge group [%1]" ).arg( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager, Resolver_ABC< KnowledgeGroupType, std::string >& types )
    : EntityImplementation< KnowledgeGroup_ABC >( controller, xml::attribute< unsigned int >( xis, "id" ), "" )
{
    std::string type, name;
    xis >> attribute( "type", type )
        >> optional() >> attribute( "name", name );
    type_ = &types.Get( type );
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
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "KnowledgeGroup", "Info/Name" ), name_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "KnowledgeGroup", "Info/Type" ), type_ );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Rename
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void KnowledgeGroup::Rename( const QString& name )
{
    name_ = name;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::DoSerialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void KnowledgeGroup::SerializeAttributes( xml::xostream& xos ) const
{
    xos << attribute( "id", long( id_ ) )
        << attribute( "type", type_->GetName() )
        << attribute( "name", name_ );
}
