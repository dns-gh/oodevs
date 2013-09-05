// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "SensorType.h"
#include "SensorTypeAgent.h"
#include "SensorTypeObject.h"
#include "Sensor.h"
#include "MT_Tools/MT_Stl.h"
#include "tools/Codec.h"
#include "wrapper/Hook.h"
#include <algorithm>
#include <xeumeuleu/xml.hpp>

using namespace sword::perception;

DECLARE_HOOK( ConvertSecondsToSim, double, ( double seconds ) )

SensorType::T_SensorTypeMap SensorType::sensorTypes_;
unsigned int SensorType::nNextID_ = 0;

// -----------------------------------------------------------------------------
// Name: SensorType::Initialize
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void SensorType::Initialize( xml::xistream& xis )
{
    xis >> xml::start( "sensors" )
            >> xml::list( "sensor", &ReadSensor )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SensorType::ReadSensor
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void SensorType::ReadSensor( xml::xistream& xis )
{
    std::string strSensorName;
    xis >> xml::attribute( "name", strSensorName );

    const SensorType*& pSensorType = sensorTypes_[ strSensorName ];
    if( pSensorType )
        throw xml::exception( xis.context() + "Unknown sensor type" );
    pSensorType = new SensorType( strSensorName, xis );
}

// -----------------------------------------------------------------------------
// Name: SensorType::Terminate
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void SensorType::Terminate()
{
    for( auto it = sensorTypes_.begin(); it != sensorTypes_.end(); ++it )
        delete it->second;
    sensorTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: SensorType constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
SensorType::SensorType( const std::string& strName, xml::xistream& xis )
    : nID_        ( nNextID_++ )
    , strName_    ( strName )
    , pTypeObject_( 0 )
    , pTypeAgent_ ( 0 )
{
    std::string time;
    xis >> xml::list( "unit-detection", *this, &SensorType::newSensorTypeAgent )
        >> xml::list( "object-detection", *this, &SensorType::newSensorTypeObject )
        >> xml::attribute( "detection-delay", time );
    tools::DecodeTime( time, delay_ );
    delay_ = static_cast< unsigned int >( GET_HOOK( ConvertSecondsToSim )( delay_ ) );
}

// -----------------------------------------------------------------------------
// Name: SensorType::newSensorTypeAgent
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void SensorType::newSensorTypeAgent( xml::xistream& xis )
{
    pTypeAgent_ = new SensorTypeAgent( *this, xis );
}

// -----------------------------------------------------------------------------
// Name: SensorType::newSensorTypeObject
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void SensorType::newSensorTypeObject( xml::xistream& xis )
{
    pTypeObject_ = new SensorTypeObject( *this, xis );
}

// -----------------------------------------------------------------------------
// Name: SensorType destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
SensorType::~SensorType()
{
    if( pTypeAgent_ )
        delete pTypeAgent_;

    if( pTypeObject_ )
        delete pTypeObject_;
}

// -----------------------------------------------------------------------------
// Name: SensorType::InstanciateSensor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
Sensor& SensorType::InstanciateSensor( double rHeight ) const
{
    return *new Sensor( *this, rHeight );
}

// -----------------------------------------------------------------------------
// Name: SensorType::FindSensorType
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
const SensorType* SensorType::FindSensorType( const std::string& strType )
{
    CIT_SensorTypeMap it = sensorTypes_.find( strType );
    return it == sensorTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: SensorType::FindSensorType
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const SensorType* SensorType::FindSensorType( const unsigned int nID )
{
    // $$$$ JVT : Recherche linéaire, mais n'est utilisé que lors de la reprise de la sim depuis un checkpoint
    CIT_SensorTypeMap it = std::find_if( sensorTypes_.begin(), sensorTypes_.end(), std::compose1( std::bind2nd( std::equal_to< unsigned int >(), nID ), std::compose1( std::mem_fun( &SensorType::GetID ), std::select2nd< T_SensorTypeMap::value_type >() ) ) );
    return it == sensorTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetTypeObject
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
const SensorTypeObject* SensorType::GetTypeObject() const
{
    return pTypeObject_;
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetTypeAgent
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
const SensorTypeAgent* SensorType::GetTypeAgent() const
{
    return pTypeAgent_;
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetName
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
const std::string& SensorType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetID
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
unsigned int SensorType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetDelay
// Created: LDC 2010-05-05
// -----------------------------------------------------------------------------
const unsigned int SensorType::GetDelay() const
{
    return delay_;
}
