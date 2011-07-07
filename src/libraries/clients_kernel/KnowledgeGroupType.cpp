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

#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupType constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
KnowledgeGroupType::KnowledgeGroupType( xml::xistream& xis )
: name_()
, rCommunicationDelay_( 0. )
{
    xis >> xml::attribute( "name", name_ );
    tools::ReadTimeAttribute( xis, "communication-delay", rCommunicationDelay_ );
    if( rCommunicationDelay_ < 0 )
        xis.error( "communication-delay < 0" );
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

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupType::ShowCommunicationDelay
// Created: LDC 2009-11-30
// -----------------------------------------------------------------------------
const std::string KnowledgeGroupType::ShowCommunicationDelay() const
{
    std::string strCommunicationDelay;
    // $$$$ SYD 2009-11-23: TODO kernel::UnitedValue should be able to represent a duration,
    //                      a duration in hh:mm:ss format where hh and ss are not displayed
    //                      when not necessary => custom formater + custom editor

    int communicationDelayHours = 0;
    int communicationDelayMins = 0;
    int communicationDelaySecs = 0;

    double rCommunicationDelay = GetCommunicationDelay();
    communicationDelayHours = (int)( rCommunicationDelay / 3600. );
    rCommunicationDelay -= communicationDelayHours * 3600.;
    communicationDelayMins = (int)( rCommunicationDelay / 60. );
    rCommunicationDelay -= communicationDelayMins * 60.;
    communicationDelaySecs = (int)( rCommunicationDelay );

    if( communicationDelayHours > 0 )
        strCommunicationDelay = boost::lexical_cast<std::string>( communicationDelayHours ) + "h";
    if( communicationDelayMins > 0 )
        strCommunicationDelay += boost::lexical_cast<std::string>( communicationDelayMins ) + "m";
    strCommunicationDelay += boost::lexical_cast<std::string>( communicationDelaySecs ) + "s";

    return strCommunicationDelay;
}