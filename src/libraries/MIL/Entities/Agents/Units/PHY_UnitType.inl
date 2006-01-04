// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/PHY_UnitType.inl $
// $Author: Nld $
// $Modtime: 7/03/05 10:43 $
// $Revision: 5 $
// $Workfile: PHY_UnitType.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetTC1Capacities
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
inline
const PHY_DotationCapacities& PHY_UnitType::GetTC1Capacities() const
{
    return dotationCapacitiesTC1_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetCommanderRepartition
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
inline
const PHY_UnitType::T_CommanderRepartitionMap& PHY_UnitType::GetCommanderRepartition() const
{
    return commandersRepartition_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetCoupDeSondeLength
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
inline
MT_Float PHY_UnitType::GetCoupDeSondeLength() const
{
    return rCoupDeSondeLength_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetCoupDeSondeWidth
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
inline
MT_Float PHY_UnitType::GetCoupDeSondeWidth() const
{
    return rCoupDeSondeWidth_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetCoefDecontaminationPerTimeStep
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
inline
MT_Float PHY_UnitType::GetCoefDecontaminationPerTimeStep() const
{
    return rCoefDecontaminationPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::CanFly
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
inline
bool PHY_UnitType::CanFly() const
{
    return bCanFly_;
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::IsAutonomous
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
inline
bool PHY_UnitType::IsAutonomous() const
{
    return bIsAutonomous_;
}
