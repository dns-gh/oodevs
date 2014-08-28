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
#include "clients_kernel/KnowledgeGroupType.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

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
KnowledgeGroup::KnowledgeGroup( unsigned long nId,
                                const std::string& name,
                                bool crowd,
                                kernel::Controller& controller,
                                const std::string& type,
                                const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types,
                                const T_CanBeRenamedFunctor& canBeRenamedFunctor )
    : gui::EntityImplementation< kernel::KnowledgeGroup_ABC >( controller, nId, ComputeName( name, nId ), canBeRenamedFunctor )
    , type_( type )
    , types_( types )
    , activated_( true )
    , crowd_( crowd )
{
    AddExtension( *this );
    kernel::KnowledgeGroupType* pType = types.Find( type_ );
    delay_ = pType ? pType->ShowCommunicationDelay() : "0m0s";
    CreateDictionary();
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
// Name: KnowledgeGroup IsActivated
// Created: SLG 2009-12-18
// -----------------------------------------------------------------------------
bool KnowledgeGroup::IsActivated() const
{
    return activated_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::IsCrowd
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
bool KnowledgeGroup::IsCrowd() const
{
    return crowd_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetType
// Created: FHD 2010-01-15
// -----------------------------------------------------------------------------
const std::string& KnowledgeGroup::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::DoUpdate
// Created: FHD 2009-12-21
// -----------------------------------------------------------------------------
void KnowledgeGroup::DoUpdate( const sword::KnowledgeGroupUpdate& message )
{
    if( message.has_type() && message.type() != type_ )
    {
        type_ = message.type();
        kernel::KnowledgeGroupType* pType = types_.Find( type_ );
        delay_ = pType ? pType->ShowCommunicationDelay() : "0m0s";
    }
    if( message.has_enabled() )
        activated_ = message.enabled();
    if( message.has_name() )
        name_ = QString::fromStdString( message.name() );
    Touch();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::CreateDictionary
// Created: FHD 2009-12-21
// -----------------------------------------------------------------------------
void KnowledgeGroup::CreateDictionary()
{
    gui::PropertiesDictionary& dictionary = Get< gui::PropertiesDictionary >();
    dictionary.Register( *this, tools::translate( "KnowledgeGroup", "Type/Name" ), type_ );
    dictionary.Register( *this, tools::translate( "KnowledgeGroup", "Type/Delay" ), delay_ );
}
