// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/PHY_Breakdown.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 17:17 $
// $Revision: 4 $
// $Workfile: PHY_Breakdown.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown::GetRepairTime
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
inline
uint PHY_Breakdown::GetRepairTime() const
{
    return nRepairTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown::AffectMobility
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
inline
bool PHY_Breakdown::AffectMobility() const
{
    assert( pType_ );
    return pType_->AffectMobility();
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown::GetNeededParts
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
inline
const PHY_BreakdownType::T_PartMap& PHY_Breakdown::GetNeededParts() const
{
    assert( pType_ );
    return pType_->GetParts();
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown::GetMosID
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
inline
uint PHY_Breakdown::GetID() const
{
    assert( pType_ );
    return pType_->GetID();
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown::AffectElectronic
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
inline
bool PHY_Breakdown::AffectElectronic() const
{
    assert( pType_ ); 
    return pType_->AffectElectronic();
}

// -----------------------------------------------------------------------------
// Name: PHY_Breakdown::GetMaintenanceLevel
// Created: JVT 2005-02-03
// -----------------------------------------------------------------------------
inline
const PHY_MaintenanceLevel& PHY_Breakdown::GetMaintenanceLevel() const
{
    assert( pType_ );
    return pType_->GetMaintenanceLevel();
}
