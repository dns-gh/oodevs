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
#include "PHY_Sensor.h"
#include <xeumeuleu/xml.h>



PHY_SensorType::T_SensorTypeMap PHY_SensorType::sensorTypes_;
uint                            PHY_SensorType::nNextID_ = 0;

// =============================================================================
// MANAGER
// =============================================================================

struct PHY_SensorType::LoadingWrapper
{
    void ReadSensor( xml::xistream& xis )
    {
        PHY_SensorType::ReadSensor( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::Initialize
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_SensorType::Initialize( xml::xistream& xis )
{
    LoadingWrapper loader;

    xis >> xml::start( "sensors" )
            >> xml::list( "sensor", loader, &LoadingWrapper::ReadSensor )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::ReadSensor
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void PHY_SensorType::ReadSensor( xml::xistream& xis )
{
    std::string strSensorName;
    xis >> xml::attribute( "name", strSensorName );

    const PHY_SensorType*& pSensorType = sensorTypes_[ strSensorName ];
    if( pSensorType )
        xis.error( "Unknown sensor type" );
    pSensorType = new PHY_SensorType( strSensorName, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::Terminate
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_SensorType::Terminate()
{
    for( CIT_SensorTypeMap it = sensorTypes_.begin(); it != sensorTypes_.end(); ++it )
        delete it->second;
    sensorTypes_.clear();
}

// =============================================================================
// INIT
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_SensorType constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_SensorType::PHY_SensorType( const std::string& strName, xml::xistream& xis )
    : nID_        ( nNextID_++ )
    , strName_    ( strName )
    , pTypeObject_( 0 )
    , pTypeAgent_ ( 0 )
{
    xis >> xml::list( "unit-detection", *this, &PHY_SensorType::newSensorTypeAgent )
        >> xml::list( "object-detection", *this, &PHY_SensorType::newSensorTypeObject );
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
void PHY_SensorType::newSensorTypeObject( xml::xistream& xis )
{
    pTypeObject_ = new PHY_SensorTypeObject( *this, xis );
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

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::InstanciateSensor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_Sensor& PHY_SensorType::InstanciateSensor( MT_Float rHeight ) const
{
    return *new PHY_Sensor( *this, rHeight );
}
