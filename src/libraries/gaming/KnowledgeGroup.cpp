// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "KnowledgeGroup.h"
#include "Tools.h"
#include "clients_kernel/KnowledgeGroupType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/ModelVisitor_ABC.h"

namespace
{
    QString ComputeName( const std::string& name, unsigned long nId )
    {
        if( !name.empty() )
            return QString::fromStdString( name );
        return QString( tools::translate( "KnowledgeGroup", "Group %L1" ) ).arg( nId );
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( unsigned long nId, const std::string& name, kernel::Controller& controller, const std::string& type, const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types, bool isJammed )
    : kernel::EntityImplementation< kernel::KnowledgeGroup_ABC >( controller, nId, ComputeName( name, nId ) )
    , type_( type )
    , types_( types )
    , activated_( true ) // LTO
    , isJammed_( isJammed )
{
    RegisterSelf( *this );
    kernel::KnowledgeGroupType* pType = types.Find( type_ );
    delay_ = pType ? pType->ShowCommunicationDelay() : "0m0s";
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

// LTO begin
// -----------------------------------------------------------------------------
// Name: KnowledgeGroup IsActivated
// Created: SLG 2009-12-18
// -----------------------------------------------------------------------------
bool KnowledgeGroup::IsActivated() const
{
    return activated_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::IsJammed
// Created: JSR 2013-06-20
// -----------------------------------------------------------------------------
bool KnowledgeGroup::IsJammed() const
{
    return isJammed_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::IsCrowd
// Created: JSR 2013-07-03
// -----------------------------------------------------------------------------
bool KnowledgeGroup::IsCrowd() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetType
// Created: FHD 2010-01-15
// -----------------------------------------------------------------------------
std::string KnowledgeGroup::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::DoUpdate
// Created: FHD 2009-12-21
// -----------------------------------------------------------------------------
void KnowledgeGroup::DoUpdate( const sword::KnowledgeGroupUpdate& message )
{
    if( message.type() != type_ )
    {
        type_ = message.type();
        kernel::KnowledgeGroupType* pType = types_.Find( type_ );
        delay_ = pType ? pType->ShowCommunicationDelay() : "0m0s";
    }
    if( message.enabled() != activated_ )
        activated_ = message.enabled();
    Touch();
}
// LTO end

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::CreateDictionary
// Created: FHD 2009-12-21
// -----------------------------------------------------------------------------
void KnowledgeGroup::CreateDictionary( kernel::Controller& controller )
{
    kernel::PropertiesDictionary* dictionary = new kernel::PropertiesDictionary( controller );
    Attach( *dictionary );
    dictionary->Register( *this, tools::translate( "KnowledgeGroup", "Info/Identifier" ), id_ );
    dictionary->Register( *this, tools::translate( "KnowledgeGroup", "Info/Name" ), name_ );
    dictionary->Register( *this, tools::translate( "KnowledgeGroup", "Type/Name" ), type_ );
    dictionary->Register( *this, tools::translate( "KnowledgeGroup", "Type/Delay" ), delay_ ); // LTO
}
