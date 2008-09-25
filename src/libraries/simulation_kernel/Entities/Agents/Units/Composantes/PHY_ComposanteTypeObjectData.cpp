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

#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include <xeumeuleu/xml.h>



// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
PHY_ComposanteTypeObjectData::PHY_ComposanteTypeObjectData( const MIL_RealObjectType& objectType, xml::xistream& xis )
    : rTimeBaseConstruction_      ( std::numeric_limits< MT_Float >::max() )
    , rTimeBaseDestruction_       ( std::numeric_limits< MT_Float >::max() )
    , rTimeConstructionCoef_      ( 1 )
    , rTimeDestructionCoef_       ( 1 )
    , rTimeMining_                ( std::numeric_limits< MT_Float >::max() )
    , rTimeDemining_              ( std::numeric_limits< MT_Float >::max() )
    , rCoefTimeBypass_            ( 1. )
    , rSpeedWithinWhenNotBypassed_( objectType.GetDefaultSpeedWhenNotBypassed() )
    , rSpeedWithinWhenBypassed_   ( objectType.GetDefaultSpeedWhenBypassed   () )
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
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData::GetMaxSpeed
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
MT_Float PHY_ComposanteTypeObjectData::GetMaxSpeed( const MIL_RealObject_ABC& object ) const
{
    if( object.IsBypassed() )
        return rSpeedWithinWhenBypassed_;
    else 
        return rSpeedWithinWhenNotBypassed_;
}
