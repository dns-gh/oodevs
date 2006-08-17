//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::IsInitialized
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionSurfaceAgent::IsInitialized()
{
    return pSensorType_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionSurfaceAgent::operator=
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
PHY_PerceptionSurfaceAgent& PHY_PerceptionSurfaceAgent::operator=( const PHY_PerceptionSurfaceAgent& rhs )
{
    pSensorType_ = rhs.pSensorType_;
    vOrigin_     = rhs.vOrigin_;
    rHeight_     = rhs.rHeight_;
    sectors_     = rhs.sectors_;
    return *this;
}
