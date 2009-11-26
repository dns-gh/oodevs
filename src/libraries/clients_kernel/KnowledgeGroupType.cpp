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

#include "Tools.h"

#include "tools/xmlcodecs.h"

#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupType constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
KnowledgeGroupType::KnowledgeGroupType( xml::xistream& xis )
: name_()
, rCommunicationDelay_( 0. )
{
    const std::string& delayAttribute( "communication-delay" );

    xis >> xml::attribute( "name", name_ );
    if( xis.has_attribute( delayAttribute ) )
    {
        if( tools::ReadTimeAttribute( xis, delayAttribute, rCommunicationDelay_ ) )
            if( rCommunicationDelay_ < 0 )
                xis.error( delayAttribute + " <= 0" );
    }
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
const std::string& KnowledgeGroupType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupType::GetCommunicationDelay
// Created: SYD 2009-11-20
// -----------------------------------------------------------------------------
double KnowledgeGroupType::GetCommunicationDelay() const
{
    return rCommunicationDelay_;
}
