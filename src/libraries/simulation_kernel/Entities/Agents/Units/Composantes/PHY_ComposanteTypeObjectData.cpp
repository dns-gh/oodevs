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
#include "tools/xmlcodecs.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
PHY_ComposanteTypeObjectData::PHY_ComposanteTypeObjectData( xml::xistream& xis )
    : rTimeBaseConstruction_      ( std::numeric_limits< MT_Float >::max() )
    , rTimeBaseDestruction_       ( std::numeric_limits< MT_Float >::max() )
    , rTimeConstructionCoef_      ( 1 )
    , rTimeDestructionCoef_       ( 1 )
    , rTimeMining_                ( std::numeric_limits< MT_Float >::max() )
    , rTimeDemining_              ( std::numeric_limits< MT_Float >::max() )
    , rCoefTimeBypass_            ( 1. )
    , rSpeedWithinWhenNotBypassed_( std::numeric_limits< MT_Float >::max() )
    , rSpeedWithinWhenBypassed_   ( std::numeric_limits< MT_Float >::max() )
    , pConsumptionMode_           ( 0 )
{
    std::string strConsumptionMode = "";
    tools::ReadTimeAttribute( xis, "initial-construction-time", rTimeBaseConstruction_       );
    tools::ReadTimeAttribute( xis, "initial-destruction-time" , rTimeBaseDestruction_        );
    tools::ReadTimeAttribute( xis, "valorization-time"        , rTimeMining_                 );
    tools::ReadTimeAttribute( xis, "devalorization-time"      , rTimeDemining_               );
    tools::ReadTimeAttribute( xis, "construction-time-factor" , rTimeConstructionCoef_       );
    tools::ReadTimeAttribute( xis, "destruction-time-factor"  , rTimeDestructionCoef_        );
    tools::ReadTimeAttribute( xis, "bypass-gain-factor"       , rCoefTimeBypass_             );
    tools::ReadTimeAttribute( xis, "non-bypassed-speed"       , rSpeedWithinWhenNotBypassed_ );
    tools::ReadTimeAttribute( xis, "bypassed-speed"           , rSpeedWithinWhenBypassed_    );

    if ( rSpeedWithinWhenNotBypassed_ < 0 )
        rSpeedWithinWhenNotBypassed_ = std::numeric_limits< MT_Float >::max();

    if ( rSpeedWithinWhenBypassed_ < 0 )
        rSpeedWithinWhenBypassed_ = std::numeric_limits< MT_Float >::max();

    xis >> xml::optional() >> xml::attribute( "consumption-mode", strConsumptionMode );

    if ( strConsumptionMode != "" )
    {
        pConsumptionMode_ = PHY_ConsumptionType::FindConsumptionType( strConsumptionMode );
        if( !pConsumptionMode_ )
            xis.error( "Unknown consumption mode '" + strConsumptionMode + "'" );
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
MT_Float PHY_ComposanteTypeObjectData::GetMaxSpeed( const MIL_Object_ABC& object ) const
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
    return rTimeBaseConstruction_ != std::numeric_limits< MT_Float >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::CanDestroy
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypeObjectData::CanDestroy() const
{
    return rTimeBaseDestruction_ != std::numeric_limits< MT_Float >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::CanMine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypeObjectData::CanMine() const
{
    return rTimeMining_ != std::numeric_limits< MT_Float >::max();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::CanDemine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool PHY_ComposanteTypeObjectData::CanDemine() const
{
    return rTimeDemining_ != std::numeric_limits< MT_Float >::max();
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
MT_Float PHY_ComposanteTypeObjectData::GetMiningTime() const
{
    return rTimeMining_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetDeminingTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
MT_Float PHY_ComposanteTypeObjectData::GetDeminingTime() const
{
    return rTimeDemining_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetBypassTime
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
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
const PHY_ConsumptionType* PHY_ComposanteTypeObjectData::GetConsumptionMode() const
{
    return pConsumptionMode_;
}
