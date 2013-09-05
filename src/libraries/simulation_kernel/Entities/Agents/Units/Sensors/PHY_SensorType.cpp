// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Sensors/PHY_SensorType.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 14:12 $
// $Revision: 5 $
// $Workfile: PHY_SensorType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_SensorType.h"
#include "PHY_SensorTypeAgent.h"
#include "PHY_SensorTypeObject.h"
#include "PHY_SensorTypeDisaster.h"
#include "PHY_Sensor.h"
#include "Tools/MIL_Tools.h"
#include "MT_Tools/MT_Stl.h"
#include "tools/Codec.h"
#include <xeumeuleu/xml.hpp>

PHY_SensorType::T_SensorTypeMap PHY_SensorType::sensorTypes_;
unsigned int PHY_SensorType::nNextID_ = 0;

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::Initialize
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_SensorType::Initialize( xml::xistream& xis, const ObjectTypeResolver_ABC& resolver )
{
    xis >> xml::start( "sensors" )
            >> xml::list( "sensor", boost::bind( &PHY_SensorType::ReadSensor, _1, boost::cref( resolver ) ) )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::ReadSensor
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void PHY_SensorType::ReadSensor( xml::xistream& xis, const ObjectTypeResolver_ABC& resolver )
{
    std::string strSensorName;
    xis >> xml::attribute( "name", strSensorName );

    const PHY_SensorType*& pSensorType = sensorTypes_[ strSensorName ];
    if( pSensorType )
        throw MASA_EXCEPTION( xis.context() + "Unknown sensor type" );
    pSensorType = new PHY_SensorType( strSensorName, xis, resolver );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::Terminate
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_SensorType::Terminate()
{
    for( auto it = sensorTypes_.begin(); it != sensorTypes_.end(); ++it )
        delete it->second;
    sensorTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_SensorType::PHY_SensorType( const std::string& strName, xml::xistream& xis, const ObjectTypeResolver_ABC& resolver )
    : nID_                ( nNextID_++ )
    , strName_            ( strName )
    , pTypeObject_        ( 0 )
    , pTypeAgent_         ( 0 )
    , pTypeDisaster_      ( 0 )
    , activationOnRequest_( false )
{
    std::string time;
    xis >> xml::list( "unit-detection", *this, &PHY_SensorType::newSensorTypeAgent )
        >> xml::list( "object-detection", *this, &PHY_SensorType::newSensorTypeObject, resolver )
        >> xml::list( "disaster-detection", *this, &PHY_SensorType::newSensorTypeDisaster )
        >> xml::attribute( "detection-delay", time )
        >> xml::optional
        >> xml::attribute( "activation-on-request", activationOnRequest_ );
    tools::DecodeTime( time, delay_ );
    delay_ = static_cast< unsigned int>( MIL_Tools::ConvertSecondsToSim( delay_ ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::newSensorTypeAgent
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void PHY_SensorType::newSensorTypeAgent( xml::xistream& xis )
{
    pTypeAgent_ = new PHY_SensorTypeAgent( *this, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::newSensorTypeObject
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void PHY_SensorType::newSensorTypeObject( xml::xistream& xis, const ObjectTypeResolver_ABC& resolver )
{
    pTypeObject_ = new PHY_SensorTypeObject( *this, xis, resolver );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::newSensorTypeDisaster
// Created: LGY 2012-12-05
// -----------------------------------------------------------------------------
void PHY_SensorType::newSensorTypeDisaster( xml::xistream& xis )
{
    pTypeDisaster_ = new PHY_SensorTypeDisaster( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_SensorType::~PHY_SensorType()
{
    if( pTypeAgent_ )
        delete pTypeAgent_;

    if( pTypeObject_ )
        delete pTypeObject_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::InstanciateSensor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_Sensor& PHY_SensorType::InstanciateSensor( double rHeight ) const
{
    return *new PHY_Sensor( *this, rHeight );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::FindSensorType
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
const PHY_SensorType* PHY_SensorType::FindSensorType( const std::string& strType )
{
    CIT_SensorTypeMap it = sensorTypes_.find( strType );

    return it == sensorTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::FindSensorType
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_SensorType* PHY_SensorType::FindSensorType( const unsigned int nID )
{
    // $$$$ JVT : Recherche linéaire, mais n'est utilisé que lors de la reprise de la sim depuis un checkpoint
    CIT_SensorTypeMap it = std::find_if( sensorTypes_.begin(), sensorTypes_.end(), std::compose1( std::bind2nd( std::equal_to< unsigned int >(), nID ), std::compose1( std::mem_fun( &PHY_SensorType::GetID ), std::select2nd< T_SensorTypeMap::value_type >() ) ) );

    return it == sensorTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::GetTypeObject
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
const PHY_SensorTypeObject* PHY_SensorType::GetTypeObject() const
{
    return pTypeObject_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::GetTypeAgent
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
const PHY_SensorTypeAgent* PHY_SensorType::GetTypeAgent() const
{
    return pTypeAgent_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::GetName
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
const std::string& PHY_SensorType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::GetID
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
unsigned int PHY_SensorType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::GetDelay
// Created: LDC 2010-05-05
// -----------------------------------------------------------------------------
const unsigned int PHY_SensorType::GetDelay() const
{
    return delay_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::GetTypeDisaster
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
const PHY_SensorTypeDisaster* PHY_SensorType::GetTypeDisaster() const
{
    return pTypeDisaster_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::IsActivatedOnRequest
// Created: LGY 2012-12-12
// -----------------------------------------------------------------------------
bool PHY_SensorType::IsActivatedOnRequest() const
{
    return activationOnRequest_;
}
