// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_SensorTypeAgent.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetSquareProximityDistance
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
inline
MT_Float PHY_SensorTypeAgent::GetSquareProximityDistance() const
{
    return rSquareProximityDist_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetMaxDistance
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
inline
MT_Float PHY_SensorTypeAgent::GetMaxDistance() const
{
    return rDetectionDist_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetAngle
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
inline
MT_Float PHY_SensorTypeAgent::GetAngle() const
{
    return rAngle_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetType
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
const PHY_SensorType& PHY_SensorTypeAgent::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
inline
MT_Float PHY_SensorTypeAgent::GetFactor( const PHY_Volume& volume ) const
{
    assert( volumeFactors_.size() > volume.GetID() );
    return volumeFactors_[ volume.GetID() ];
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::CanScan
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
inline
bool PHY_SensorTypeAgent::CanScan() const
{
    return bScanningAllowed_;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ConvertEnvironementToObjectIdx
// Created: JVT 03-04-28
//-----------------------------------------------------------------------------
inline
uint PHY_SensorTypeAgent::ConvertEnvironementToObjectIdx( PHY_RawVisionData::E_VisionObject obj )
{
    if ( obj == PHY_RawVisionData::eVisionEmpty )
        return 0;

    uint res = 1;
    for ( uint idx = 1; !( idx & obj ); idx <<= 1 )
        ++res;
    return res;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ConvertObjectIdxToEnvironnement
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
inline
PHY_RawVisionData::E_VisionObject PHY_SensorTypeAgent::ConvertObjectIdxToEnvironnement( uint val )
{
    return (PHY_RawVisionData::E_VisionObject)( val ? 1 << ( val - 1 ) : 0 );
}
