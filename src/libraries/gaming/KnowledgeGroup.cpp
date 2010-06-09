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

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( unsigned long nId, kernel::Controller& controller, const std::string& type, const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types )
    : kernel::EntityImplementation< kernel::KnowledgeGroup_ABC >( controller, nId, QString( tools::translate( "KnowledgeGroup", "Group %1" ) ).arg( nId ) )
    , types_( types )
    , type_( type )
    , activated_( true ) // LTO
{
    RegisterSelf( *this );
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
void KnowledgeGroup::DoUpdate( const MsgsSimToClient::MsgKnowledgeGroupUpdate& message )
{
    if( message.type() != type_ )
        type_ = message.type();
    if( message.enabled() != activated_ )
        activated_ = message.enabled();
    Touch();
}
// LTO end
