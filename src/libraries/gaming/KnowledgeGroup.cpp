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
#include "clients_kernel/ModelVisitor_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( unsigned long nId, Controller& controller )
    : EntityImplementation< KnowledgeGroup_ABC >( controller, nId, QString( tools::translate( "KnowledgeGroup", "Group %1" ) ).arg( nId ) )
    , isActivated_ ( true )
{
    // NOTHING
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
// Name: KnowledgeGroup Desactivate
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
bool KnowledgeGroup::IsActivated() const
{
    return isActivated_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void KnowledgeGroup::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
