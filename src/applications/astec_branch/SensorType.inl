//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: SensorType::GetName
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
const std::string& SensorType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: SensorType::GetAngle
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
float SensorType::GetAngle() const
{
    return rAngle_;
}

// -----------------------------------------------------------------------------
// Name: SensorType::ComputeExtinction
// Created: JVT 2004-09-28
// -----------------------------------------------------------------------------
inline
float SensorType::ComputeExtinction( /*const RawVisionData::Iterator& env,*/ const Agent& srcAgent ) const
{
    return ComputeExtinction( /*env,*/ srcAgent, rDetectionDist_ );
}

// -----------------------------------------------------------------------------
// Name: SensorType::InterpreteNRJ
// Created: JVT 2004-09-28
// -----------------------------------------------------------------------------
inline
E_PerceptionResult SensorType::InterpreteNRJ( float rNRJ ) const
{
    if ( rNRJ >= rDetectionDist_ - rIdentificationDist_ )
        return eIdentification;
    if ( rNRJ >= rDetectionDist_ - rRecognitionDist_ )
        return  eRecognition;
    if ( rNRJ >= 0 )
        return eDetection;
    return eNotSeen;
}
