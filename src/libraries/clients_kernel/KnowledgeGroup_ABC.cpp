// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "KnowledgeGroup_ABC.h"
#include "ActionController.h"

using namespace kernel;

const QString KnowledgeGroup_ABC::typeName_ = "knowledgeGroup";

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC constructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC::KnowledgeGroup_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC destructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC::~KnowledgeGroup_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC::DoUpdate
// Created: SYD 2009-12-26
// -----------------------------------------------------------------------------
void KnowledgeGroup_ABC::DoUpdate( const ASN1T_MsgKnowledgeGroupUpdate& )
{
    throw std::exception( __FUNCTION__ " not implemented" ); 
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC::GetTypeName
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
QString KnowledgeGroup_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC::Select
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
void KnowledgeGroup_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC::ContextMenu
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
void KnowledgeGroup_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC::Activate
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
void KnowledgeGroup_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Entity_ABC*)this );
}
