// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Posture/PHY_RolePion_Posture.inl $
// $Author: Jvt $
// $Modtime: 11/05/05 15:24 $
// $Revision: 6 $
// $Workfile: PHY_RolePion_Posture.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::GetLastPosture
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const PHY_Posture& PHY_RolePion_Posture::GetLastPosture() const
{
    assert( pLastPosture_ );
    return *pLastPosture_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::GetCurrentPosture
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const PHY_Posture& PHY_RolePion_Posture::GetCurrentPosture() const
{
    assert( pCurrentPosture_ );
    return *pCurrentPosture_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::GetPostureCompletionPercentage
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Posture::GetPostureCompletionPercentage() const
{
    return rPostureCompletionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::EnableDiscreteMode
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Posture::EnableDiscreteMode()
{
    bDiscreteModeEnabled_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::DisableDiscreteMode
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Posture::DisableDiscreteMode()
{
    bDiscreteModeEnabled_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::HasChanged
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Posture::HasChanged() const
{
    return bPosturesHasChanged_ || bPourcentageHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::HLAStatusHasChanged
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Posture::HLAStatusHasChanged() const
{
    return bPosturesHasChanged_ || bPercentageCrossed50_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Posture::Clean()
{
    bPosturesHasChanged_      = false;
    bPourcentageHasChanged_   = false;
    bStealthFactorHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::SetStealthFactor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Posture::SetStealthFactor( MT_Float rValue )
{
    assert( rValue >= 0. && rValue <= 1. );
    if( rValue != rStealthFactor_ )
    {
        bStealthFactorHasChanged_ = true;
        rStealthFactor_ = rValue;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::SetTimingFactor
// Created: JVT 2005-05-11
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Posture::SetTimingFactor( MT_Float rFactor )
{
    assert( rFactor > 0. );
    rTimingFactor_ = rFactor == 0. ? 1. : std::abs( rFactor );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::SetElongationFactor
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Posture::SetElongationFactor( MT_Float rFactor )
{
    assert( rFactor > 0. );
    if ( rFactor > 0. )
        rElongationFactor_ = rFactor;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::GetElongationFactor
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Posture::GetElongationFactor() const
{
    return rElongationFactor_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::IsStealth
// Created: AGE 2004-12-08
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Posture::IsStealth() const
{
    return bIsStealth_;
}
