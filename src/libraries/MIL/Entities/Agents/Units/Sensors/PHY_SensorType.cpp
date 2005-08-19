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

#include "MIL_pch.h"

#include "PHY_SensorType.h"

#include "PHY_SensorTypeAgent.h"
#include "PHY_SensorTypeObject.h"
#include "PHY_Sensor.h"

PHY_SensorType::T_SensorTypeMap PHY_SensorType::sensorTypes_;
uint                            PHY_SensorType::nNextID_ = 0;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::Initialize
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_SensorType::Initialize( MIL_InputArchive& archive )
{
    archive.BeginList( "Senseurs" );

    while ( archive.NextListElement() )
    {
        archive.Section( "Senseur" );

        std::string strSensorName;
        archive.ReadAttribute( "nom", strSensorName );

        const PHY_SensorType*& pSensorType = sensorTypes_[ strSensorName ];
        if( pSensorType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown sensor type", archive.GetContext() );
        pSensorType = new PHY_SensorType( strSensorName, archive );

        archive.EndSection(); // Senseur
    }

    archive.EndList(); // Senseurs
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
PHY_SensorType::PHY_SensorType( const std::string& strName, MIL_InputArchive& archive )
    : nID_        ( nNextID_++ )
    , strName_    ( strName )
    , pTypeObject_( 0 )
    , pTypeAgent_ ( 0 )
{
    if ( archive.Section( "DetectionAgents", MIL_InputArchive::eNothing ) )
    {
        pTypeAgent_ = new PHY_SensorTypeAgent( *this, archive );
        archive.EndSection(); // DetectionAgents
    }

    if ( archive.Section( "DetectionObjets", MIL_InputArchive::eNothing ) )
    {
        pTypeObject_ = new PHY_SensorTypeObject( *this, archive );
        archive.EndSection(); // DetectionObjets
    }
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
