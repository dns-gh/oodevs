// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationLogisticType.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 17:27 $
// $Revision: 8 $
// $Workfile: PHY_DotationLogisticType.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_DotationLogisticType.h"

PHY_DotationLogisticType::T_DotationLogisticTypeMap PHY_DotationLogisticType::dotationLogisticTypes_;

PHY_DotationLogisticType PHY_DotationLogisticType::uniteEssence_  ( "Unite essence", eUniteEssence   );
PHY_DotationLogisticType PHY_DotationLogisticType::uniteFeuTD_    ( "Unite feu TD" , eUniteFeuTD     );
PHY_DotationLogisticType PHY_DotationLogisticType::uniteFeuSansTD_( "Unite feu -TD", eUniteFeuSansTD );
PHY_DotationLogisticType PHY_DotationLogisticType::uniteVivre_    ( "Unite vivre"  , eUniteVivre     );
PHY_DotationLogisticType PHY_DotationLogisticType::pieces_        ( "Pieces"       , ePieces         );

//=============================================================================
// STATIC INITIALIZATION 
//=============================================================================

//-----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::Initialize
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
void PHY_DotationLogisticType::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing logistic dotation types" );

    dotationLogisticTypes_[ uniteEssence_  .GetName() ] = &uniteEssence_;
    dotationLogisticTypes_[ uniteFeuTD_    .GetName() ] = &uniteFeuTD_;
    dotationLogisticTypes_[ uniteFeuSansTD_.GetName() ] = &uniteFeuSansTD_;
    dotationLogisticTypes_[ uniteVivre_    .GetName() ] = &uniteVivre_;
    dotationLogisticTypes_[ pieces_        .GetName() ] = &pieces_;    
}

//-----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::Terminate
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
void PHY_DotationLogisticType::Terminate()
{
    dotationLogisticTypes_.clear();
}

//=============================================================================
// 
//=============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType constructor
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
PHY_DotationLogisticType::PHY_DotationLogisticType( const std::string& strName, E_DotationLogisticType nType )
    : strName_( strName )
    , nType_  ( nType   )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType destructor
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
PHY_DotationLogisticType::~PHY_DotationLogisticType()
{

}
