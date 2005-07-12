// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_MedicalConsign_ABC.inl $
// $Author: Jvt $
// $Modtime: 11/04/05 15:28 $
// $Revision: 4 $
// $Workfile: PHY_MedicalConsign_ABC.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::Clean
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
void PHY_MedicalConsign_ABC::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::SetTimer
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
void PHY_MedicalConsign_ABC::SetState( E_State nNewState )
{
    nState_ = nNewState;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::GetState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
PHY_MedicalConsign_ABC::E_State PHY_MedicalConsign_ABC::GetState() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::GetHumanState
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
const PHY_MedicalHumanState& PHY_MedicalConsign_ABC::GetHumanState() const
{
    assert( pHumanState_ );
    return *pHumanState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::HasChanged
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
bool PHY_MedicalConsign_ABC::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::IsFinished
// Created: NLD 2005-03-08
// -----------------------------------------------------------------------------
inline
bool PHY_MedicalConsign_ABC::IsFinished() const
{
    return nState_ == eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::GetPionMedical
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
inline
PHY_RolePionLOG_Medical& PHY_MedicalConsign_ABC::GetPionMedical()
{
    assert( pMedical_ );
    return *pMedical_;
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MedicalConsign_ABC::serialize( Archive& file, const uint )
{
    file & pMedical_   
         & pHumanState_
         & nTimer_
         & nState_;
}
