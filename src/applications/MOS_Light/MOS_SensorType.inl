//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_SensorType::GetName
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
const std::string& MOS_SensorType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MOS_SensorType::GetAngle
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
MT_Float MOS_SensorType::GetAngle() const
{
    return rAngle_;
}

// -----------------------------------------------------------------------------
// Name: MOS_SensorType::ComputeExtinction
// Created: JVT 2004-09-28
// -----------------------------------------------------------------------------
inline
MT_Float MOS_SensorType::ComputeExtinction( const MOS_RawVisionData::Iterator& env, const MOS_Agent& srcAgent ) const
{
    return ComputeExtinction( env, srcAgent, rDetectionDist_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_SensorType::InterpreteNRJ
// Created: JVT 2004-09-28
// -----------------------------------------------------------------------------
inline
E_PerceptionResult MOS_SensorType::InterpreteNRJ( MT_Float rNRJ ) const
{
    if ( rNRJ >= rDetectionDist_ - rIdentificationDist_ )
        return eIdentification;
    if ( rNRJ >= rDetectionDist_ - rRecognitionDist_ )
        return  eRecognition;
    if ( rNRJ >= 0 )
        return eDetection;
    return eNotSeen;
}
