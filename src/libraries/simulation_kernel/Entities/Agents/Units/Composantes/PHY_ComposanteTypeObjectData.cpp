// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Composantes/PHY_ComposanteTypeObjectData.cpp $
// $Author: Nld $
// $Modtime: 8/03/05 16:49 $
// $Revision: 5 $
// $Workfile: PHY_ComposanteTypeObjectData.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_ComposanteTypeObjectData.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "tools/Codec.h"

namespace
{
    void ReadTimeAttribute( xml::xistream& xis, const std::string& name, double& value )
    {
        double oldValue = value;
        if( !tools::ReadTimeAttribute( xis, name, value ) )
            value = oldValue;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
PHY_ComposanteTypeObjectData::PHY_ComposanteTypeObjectData( xml::xistream& xis )
    : rTimeBaseConstruction_      ( std::numeric_limits< double >::max() )
    , rTimeBaseDestruction_       ( std::numeric_limits< double >::max() )
    , rTimeConstructionCoef_      ( 1 )
    , rTimeDestructionCoef_       ( 1 )
    , rTimeMining_                ( std::numeric_limits< double >::max() )
    , rTimeDemining_              ( std::numeric_limits< double >::max() )
    , rTimeExtinguishing_         ( std::numeric_limits< double >::max() )
    , rCoefTimeBypass_            ( 1. )
    , rSpeedWithinWhenNotBypassed_( std::numeric_limits< double >::max() )
    , rSpeedWithinWhenBypassed_   ( std::numeric_limits< double >::max() )
    , pConsumptionMode_           ( 0 )
{
    std::string strConsumptionMode = "";
    ReadTimeAttribute( xis, "initial-construction-time", rTimeBaseConstruction_);
    ReadTimeAttribute( xis, "initial-destruction-time" , rTimeBaseDestruction_ );
    ReadTimeAttribute( xis, "valorization-time"        , rTimeMining_          );
    ReadTimeAttribute( xis, "devalorization-time"      , rTimeDemining_        );
    ReadTimeAttribute( xis, "extinguishing-time"       , rTimeExtinguishing_   );
    ReadTimeAttribute( xis, "construction-time-factor" , rTimeConstructionCoef_);
    ReadTimeAttribute( xis, "destruction-time-factor"  , rTimeDestructionCoef_ );

    xis >> xml::optional >> xml::attribute( "bypass-gain-factor", rCoefTimeBypass_             );
    xis >> xml::optional >> xml::attribute( "non-bypassed-speed", rSpeedWithinWhenNotBypassed_ );
    xis >> xml::optional >> xml::attribute( "bypassed-speed",     rSpeedWithinWhenBypassed_    );

    if( rSpeedWithinWhenNotBypassed_ < 0 )
        rSpeedWithinWhenNotBypassed_ = std::numeric_limits< double >::max();

    if( rSpeedWithinWhenBypassed_ < 0 )
        rSpeedWithinWhenBypassed_ = std::numeric_limits< double >::max();

    xis >> xml::optional >> xml::attribute( "consumption-mode", strConsumptionMode );

    if( strConsumptionMode != "" )
    {
        pConsumptionMode_ = PHY_ConsumptionType::FindConsumptionType( strConsumptionMode );
        if( !pConsumptionMode_ )
            throw MASA_EXCEPTION( xis.context() + "Unknown consumption mode '" + strConsumptionMode + "'" );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData destructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
PHY_ComposanteTypeObjectData::~PHY_ComposanteTypeObjectData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetMaxSpeed
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
double PHY_ComposanteTypeObjectData::GetMaxSpeed( const MIL_Object_ABC& object ) const
{
    if( object().IsBypassed() )
        return rSpeedWithinWhenBypassed_;
    else
        return rSpeedWithinWhenNotBypassed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::CanConstruct
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypeObjectData::CanConstruct() const
{
    return rTimeBaseConstruction_ != std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::CanDestroy
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypeObjectData::CanDestroy() const
{
    return rTimeBaseDestruction_ != std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::CanMine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypeObjectData::CanMine() const
{
    return rTimeMining_ != std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::CanExtinguish
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypeObjectData::CanExtinguish() const
{
    return rTimeExtinguishing_ != std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::CanDemine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypeObjectData::CanDemine() const
{
    return rTimeDemining_ != std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::CanBypass
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypeObjectData::CanBypass( bool bObjectIsMined ) const
{
    return CanDestroy() && ( !bObjectIsMined || CanDemine() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetConstructionTime
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
double PHY_ComposanteTypeObjectData::GetConstructionTime( double rSizeCoef ) const
{
    if( rTimeBaseConstruction_ == std::numeric_limits< double >::max() )
        return std::numeric_limits< double >::max();
    return rTimeBaseConstruction_ + ( rSizeCoef * rTimeConstructionCoef_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetDestructionTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposanteTypeObjectData::GetDestructionTime( double rSizeCoef ) const
{
    if( rTimeBaseDestruction_ == std::numeric_limits< double >::max() )
        return std::numeric_limits< double >::max();
    return rTimeBaseDestruction_ + ( rSizeCoef * rTimeDestructionCoef_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetMiningTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposanteTypeObjectData::GetMiningTime() const
{
    return rTimeMining_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetDeminingTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposanteTypeObjectData::GetDeminingTime() const
{
    return rTimeDemining_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetBypassTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
double PHY_ComposanteTypeObjectData::GetBypassTime( double rSizeCoef, bool bObjectIsMined ) const
{
    double rTime = GetDestructionTime( rSizeCoef );
    if( rTime == std::numeric_limits< double >::max() )
        return std::numeric_limits< double >::max();
    if( !bObjectIsMined )
        return rTime;

    if( GetDeminingTime() == std::numeric_limits< double >::max() )
        return std::numeric_limits< double >::max();
    return ( rTime + GetDeminingTime() ) * rCoefTimeBypass_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetConsumptionMode
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
const PHY_ConsumptionType* PHY_ComposanteTypeObjectData::GetConsumptionMode() const
{
    return pConsumptionMode_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetExtinguishingTime
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
double PHY_ComposanteTypeObjectData::GetExtinguishingTime() const
{
    return rTimeExtinguishing_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::CanRemoveFromPath
// Created: BCI 2011-06-20
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypeObjectData::CanRemoveFromPath( bool bObjectIsMined ) const
{
    return CanBypass( bObjectIsMined );
}
