// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Composantes/PHY_ComposanteTypeObjectData.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_ComposanteTypeObjectData.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::CanConstruct
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypeObjectData::CanConstruct() const
{
    return rTimeBaseConstruction_ != std::numeric_limits< MT_Float >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::CanDestroy
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypeObjectData::CanDestroy() const
{
    return rTimeBaseDestruction_ != std::numeric_limits< MT_Float >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::CanMine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypeObjectData::CanMine() const
{
    return rTimeMining_ != std::numeric_limits< MT_Float >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::CanDemine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypeObjectData::CanDemine() const
{
    return rTimeDemining_ != std::numeric_limits< MT_Float >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::CanBypass
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypeObjectData::CanBypass( bool bObjectIsMined ) const
{
    return CanDestroy() && ( !bObjectIsMined || CanDemine() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetConstructionTime
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypeObjectData::GetConstructionTime( MT_Float rSizeCoef ) const
{
    if( rTimeBaseConstruction_ == std::numeric_limits< MT_Float >::max() )
        return std::numeric_limits< MT_Float >::max();
    return rTimeBaseConstruction_ + ( rSizeCoef * rTimeConstructionCoef_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetDestructionTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypeObjectData::GetDestructionTime( MT_Float rSizeCoef ) const
{
    if( rTimeBaseDestruction_ == std::numeric_limits< MT_Float >::max() )
        return std::numeric_limits< MT_Float >::max();
    return rTimeBaseDestruction_ + ( rSizeCoef * rTimeDestructionCoef_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetMiningTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypeObjectData::GetMiningTime() const
{
    return rTimeMining_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetDeminingTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypeObjectData::GetDeminingTime() const
{
    return rTimeDemining_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetBypassTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypeObjectData::GetBypassTime( MT_Float rSizeCoef, bool bObjectIsMined ) const
{
    MT_Float rTime = GetDestructionTime( rSizeCoef );
    if( rTime == std::numeric_limits< MT_Float >::max() )
        return std::numeric_limits< MT_Float >::max();
    if( !bObjectIsMined )
        return rTime;

    if( GetDeminingTime() == std::numeric_limits< MT_Float >::max() )
        return std::numeric_limits< MT_Float >::max();
    return ( rTime + GetDeminingTime() ) * rCoefTimeBypass_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetConsumptionMode
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
inline
const PHY_ConsumptionType* PHY_ComposanteTypeObjectData::GetConsumptionMode() const
{
    return pConsumptionMode_;
}
