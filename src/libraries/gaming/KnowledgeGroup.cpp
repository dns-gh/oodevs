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
#include "clients_kernel/Controller.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/ActionController.h"

using namespace kernel;

IDManager KnowledgeGroup::idManager_( 0 );

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( unsigned long nId, Controller& controller )
    : controller_( controller )
    , nID_ ( nId )
{
    RegisterSelf( *this );
    idManager_.LockIdentifier( nID_ );
    name_ = QString( "Gtia %1" ).arg( nID_ ); // $$$$ AGE 2006-08-23: 
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    controller_.Delete( *(KnowledgeGroup_ABC*)this );
    idManager_.ReleaseIdentifier( nID_ );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void KnowledgeGroup::DoUpdate( const kernel::InstanciationComplete& )
{
    controller_.Create( *(KnowledgeGroup_ABC*)this );
}   

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetId
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
unsigned long KnowledgeGroup::GetId() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetName
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
QString KnowledgeGroup::GetName() const
{
    return name_;
}

