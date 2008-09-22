// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "MissionParameterHeading.h"
#include "Point.h"
#include "SerializationTools.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include <boost/bind.hpp>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: MissionParameterHeading constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterHeading::MissionParameterHeading( xml::xistream& xis, const kernel::OrderParameter& type, const dispatcher::Agent& agent )
    : MissionParameter_ABC( type )
    , angle_( 0 )
{
    const Point entityPosition( agent.position_.latitude, agent.position_.longitude );
    const Point enemyPosition( xis, NS( "AbstractAbsolutePoint", "cbml" ) );
    angle_ = entityPosition.ComputeBearing( enemyPosition );
}

namespace
{
    struct PositionComputer
    {
        PositionComputer() : latitude_( 0 ), longitude_( 0 ), count_( 0 ) {}

        void AddAgent( const dispatcher::Agent& entity )
        {
            latitude_  += entity.position_.latitude;
            longitude_ += entity.position_.longitude;
            ++count_;
        }
        void AddAutomat( const dispatcher::Automat& entity )
        {
            entity.automats_.Apply( boost::bind( &PositionComputer::AddAutomat, boost::ref( *this ), _1 ) );
            entity.agents_.Apply( boost::bind( &PositionComputer::AddAgent, boost::ref( *this ), _1 ) );
        }
        
        double latitude_;
        double longitude_;
        unsigned int count_;
    };
}

// -----------------------------------------------------------------------------
// Name: MissionParameterHeading constructor
// Created: SBO 2008-05-23
// -----------------------------------------------------------------------------
MissionParameterHeading::MissionParameterHeading( xml::xistream& xis, const kernel::OrderParameter& type, const dispatcher::Automat& automat )
    : MissionParameter_ABC( type )
    , angle_( 0 )
{
    PositionComputer computer;
    automat.automats_.Apply( boost::bind( &PositionComputer::AddAutomat, boost::ref( computer ), _1 ) );
    automat.agents_.Apply( boost::bind( &PositionComputer::AddAgent, boost::ref( computer ), _1 ) );
    if( computer.count_ > 0 )
    {
        const Point entityPosition( computer.latitude_ / computer.count_, computer.longitude_ / computer.count_ );
        const Point enemyPosition( xis, NS( "AbstractAbsolutePoint", "cbml" ) );
        angle_ = entityPosition.ComputeBearing( enemyPosition );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameterHeading destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterHeading::~MissionParameterHeading()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterHeading::Serialize
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterHeading::Serialize( ASN1T_MissionParameter& parameter ) const
{
    parameter.null_value = 0;
    parameter.value.t = T_MissionParameter_value_heading;
    parameter.value.u.heading = angle_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameterHeading::Clean
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterHeading::Clean( ASN1T_MissionParameter& ) const
{
    // NOTHING
}
