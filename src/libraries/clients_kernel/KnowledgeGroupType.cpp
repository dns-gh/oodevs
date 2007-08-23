// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "KnowledgeGroupType.h"
#include "xeumeuleu/xml.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupType constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
KnowledgeGroupType::KnowledgeGroupType( xml::xistream& xis )
{
    std::string name;
    xis >> xml::attribute( "name", name );
    name_ = name.c_str();
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
