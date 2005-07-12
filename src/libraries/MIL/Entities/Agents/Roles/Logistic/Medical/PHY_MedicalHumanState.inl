// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_MedicalHumanState.inl $
// $Author: Jvt $
// $Modtime: 28/04/05 17:43 $
// $Revision: 4 $
// $Workfile: PHY_MedicalHumanState.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::GetHuman
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
inline
const PHY_Human& PHY_MedicalHumanState::GetHuman() const
{
    assert( pHuman_ );
    return *pHuman_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::SetConsign
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
inline
void PHY_MedicalHumanState::SetConsign( PHY_MedicalConsign_ABC* pConsign )
{
    if( pConsign == pConsign_ )
        return;
        
    pConsign_    = pConsign;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::SetHumanPosition
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
inline
void PHY_MedicalHumanState::SetHumanPosition( const MT_Vector2D& vPosition )
{
    vHumanPosition_ = vPosition;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::GetHumanPosition
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& PHY_MedicalHumanState::GetHumanPosition() const
{
    return vHumanPosition_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::NeedDiagnosis
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
inline
bool PHY_MedicalHumanState::NeedDiagnosis() const
{
    return bDiagnosed_ == false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::NotifyDiagnosed
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
inline
void PHY_MedicalHumanState::NotifyDiagnosed()
{
    bDiagnosed_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::NotifyHumanChanged
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
inline
void PHY_MedicalHumanState::NotifyHumanChanged()
{
    bHumanStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::ShouldGoBackToWar
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
inline
bool PHY_MedicalHumanState::ShouldGoBackToWar() const
{
    return bShouldGoBackToWar_;
}
