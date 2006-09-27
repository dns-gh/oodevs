// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "KnowledgeGroupHierarchy.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controller.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchy constructor
// Created: SBO 2006-09-27
// -----------------------------------------------------------------------------
KnowledgeGroupHierarchy::KnowledgeGroupHierarchy( kernel::Controller& controller )
    : controller_( controller )
    , group_( 0 )
{
    controller_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchy destructor
// Created: SBO 2006-09-27
// -----------------------------------------------------------------------------
KnowledgeGroupHierarchy::~KnowledgeGroupHierarchy()
{
    controller_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchy::GetKnowledgeGroup
// Created: SBO 2006-09-27
// -----------------------------------------------------------------------------
const kernel::KnowledgeGroup_ABC* KnowledgeGroupHierarchy::GetKnowledgeGroup() const
{
    return group_;
}

//- ----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchy::DoSerialize
// Created: SBO 2006-09-27
// -----------------------------------------------------------------------------
void KnowledgeGroupHierarchy::DoSerialize( xml::xostream& xos ) const
{
    if( group_ )
        xos << attribute( "knowledge-group", long( group_->GetId() ) );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchy::NotifyUpdated
// Created: SBO 2006-09-27
// -----------------------------------------------------------------------------
void KnowledgeGroupHierarchy::NotifyUpdated( const kernel::CommunicationHierarchies& hierarchies )
{
    const Entity_ABC* superior = hierarchies.GetSuperior();
    if( !superior )
        group_ = 0;
    else
    {
        const KnowledgeGroup_ABC* group = dynamic_cast< const KnowledgeGroup_ABC* >( superior );
        group_ = group ? group : 0;
    }
}
