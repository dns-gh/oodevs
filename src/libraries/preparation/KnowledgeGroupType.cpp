// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "KnowledgeGroupType.h"

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupType constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
KnowledgeGroupType::KnowledgeGroupType( const QString& name, const QString& value )
    : name_( name )
    , value_( value )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: KnowledgeGroupType destructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
KnowledgeGroupType::~KnowledgeGroupType()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: KnowledgeGroupType::GetName
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
const QString& KnowledgeGroupType::GetName() const
{
    return name_;
}
    
// -----------------------------------------------------------------------------
// Name: KnowledgeGroupType::GetValue
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
const QString& KnowledgeGroupType::GetValue() const
{
    return value_;
}
