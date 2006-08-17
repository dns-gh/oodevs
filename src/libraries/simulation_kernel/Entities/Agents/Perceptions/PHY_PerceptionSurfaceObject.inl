//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************


// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceObject::IsInitialized
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionSurfaceObject::IsInitialized() const
{
    return pSensorType_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceObject::operator=
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
PHY_PerceptionSurfaceObject& PHY_PerceptionSurfaceObject::operator=( const PHY_PerceptionSurfaceObject& rhs )
{
    pSensorType_ = rhs.pSensorType_;
    vOrigin_     = rhs.vOrigin_;
    rHeight_     = rhs.rHeight_;
    return *this;
}
