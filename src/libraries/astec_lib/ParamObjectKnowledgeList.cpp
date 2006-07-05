// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ParamObjectKnowledgeList.h"
#include "Object.h"
#include "ObjectKnowledge.h"

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledgeList::ParamObjectKnowledgeList( QWidget* pParent, ASN1T_ListKnowledgeObject& asn, const std::string& label, const std::string& menu )
    : EntityListParameter< ObjectKnowledge >( pParent, asn.n, asn.elem, label, menu )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamObjectKnowledgeList::~ParamObjectKnowledgeList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamObjectKnowledgeList::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamObjectKnowledgeList::NotifyContextMenu( const Object& entity, QPopupMenu& menu )
{
    // $$$$ AGE 2006-03-14: TODO !
}
