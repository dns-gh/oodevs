// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceComposanteState.inl $
// $Author: Nld $
// $Modtime: 10/01/05 17:33 $
// $Revision: 2 $
// $Workfile: PHY_MaintenanceComposanteState.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::SetConsign
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
inline
void PHY_MaintenanceComposanteState::SetConsign( PHY_MaintenanceConsign_ABC* pConsign )
{
    if( pConsign == pConsign_ )
        return;
        
    pConsign_    = pConsign;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::SetComposantePosition
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
inline
void PHY_MaintenanceComposanteState::SetComposantePosition( const MT_Vector2D& vPosition )
{
    vComposantePosition_ = vPosition;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::GetComposantePosition
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& PHY_MaintenanceComposanteState::GetComposantePosition() const
{
    return vComposantePosition_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::NeedDiagnosis
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
inline
bool PHY_MaintenanceComposanteState::NeedDiagnosis() const
{
    return bDiagnosed_ == false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::NotifyDiagnosed
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
inline
void PHY_MaintenanceComposanteState::NotifyDiagnosed()
{
    bDiagnosed_ = true;
}

