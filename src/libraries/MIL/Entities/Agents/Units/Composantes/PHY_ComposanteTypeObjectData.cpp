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

#include "MIL_Pch.h"

#include "PHY_ComposanteTypeObjectData.h"

#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypeObjectData constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
PHY_ComposanteTypeObjectData::PHY_ComposanteTypeObjectData( const MIL_RealObjectType& objectType, MIL_InputArchive& archive )
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
    MT_Float rTmp;
    if( archive.ReadTimeField( "TempsInitialConstruction", rTmp, CheckValueGreaterOrEqual( 0. ), MIL_InputArchive::eNothing, MIL_InputArchive::eNothing ) ) 
        rTimeBaseConstruction_ = rTmp;
    if( archive.ReadTimeField( "TempsInitialDestruction", rTmp, CheckValueGreaterOrEqual( 0. ), MIL_InputArchive::eNothing, MIL_InputArchive::eNothing ) ) 
        rTimeBaseDestruction_ = rTmp;
    if( archive.ReadTimeField( "TempsValorisation", rTmp, CheckValueGreaterOrEqual( 0. ), MIL_InputArchive::eNothing, MIL_InputArchive::eNothing ) ) 
        rTimeMining_ = rTmp;
    if( archive.ReadTimeField( "TempsDevalorisation", rTmp, CheckValueGreaterOrEqual( 0. ), MIL_InputArchive::eNothing, MIL_InputArchive::eNothing ) ) 
        rTimeDemining_ = rTmp;
    if( archive.ReadTimeField( "TempsConstructionCoef", rTmp, CheckValueGreaterOrEqual( 0. ), MIL_InputArchive::eNothing, MIL_InputArchive::eNothing ) ) 
        rTimeConstructionCoef_ = rTmp;
    if( archive.ReadTimeField( "TempsDestructionCoef", rTmp, CheckValueGreaterOrEqual( 0. ), MIL_InputArchive::eNothing, MIL_InputArchive::eNothing ) ) 
        rTimeDestructionCoef_ = rTmp;
    if( archive.ReadField( "CoefGainContournement", rTmp, CheckValueGreaterOrEqual( 0. ), MIL_InputArchive::eNothing, MIL_InputArchive::eNothing ) ) 
        rCoefTimeBypass_ = rTmp;

    if ( archive.ReadField( "VitesseNonContourne", rSpeedWithinWhenNotBypassed_, MIL_InputArchive::eNothing ) )
    {
        if ( rSpeedWithinWhenNotBypassed_ < 0 )
            rSpeedWithinWhenNotBypassed_ = std::numeric_limits< MT_Float >::max();
        else
            rSpeedWithinWhenNotBypassed_ = MIL_Tools::ConvertSpeedMosToSim( rSpeedWithinWhenNotBypassed_  );
    }

    if ( archive.ReadField( "VitesseContourne", rSpeedWithinWhenBypassed_, MIL_InputArchive::eNothing ) )
    {
        if ( rSpeedWithinWhenBypassed_ < 0 )
            rSpeedWithinWhenBypassed_ = std::numeric_limits< MT_Float >::max();
        else
            rSpeedWithinWhenBypassed_ = MIL_Tools::ConvertSpeedMosToSim( rSpeedWithinWhenBypassed_  );
    }   

    std::string strConsumptionMode;
    if ( archive.ReadField( "ModeConsommation", strConsumptionMode, MIL_InputArchive::eNothing ) )
    {
        pConsumptionMode_ = PHY_ConsumptionType::FindConsumptionType( strConsumptionMode );
        if( !pConsumptionMode_ )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown consumption mode '%s'", strConsumptionMode.c_str() ) );
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
