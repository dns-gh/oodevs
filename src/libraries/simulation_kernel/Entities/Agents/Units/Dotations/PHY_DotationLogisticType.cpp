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

#include "simulation_kernel_pch.h"
#include "PHY_DotationLogisticType.h"
#include "MT_Tools/MT_Logger.h"

PHY_DotationLogisticType::T_DotationLogisticTypeMap PHY_DotationLogisticType::dotationLogisticTypes_;

PHY_DotationLogisticType PHY_DotationLogisticType::uniteEssence_  ( "Unite essence", eUniteEssence   );
PHY_DotationLogisticType PHY_DotationLogisticType::uniteFeuTD_    ( "Unite tranche D" , eUniteFeuTD     );
PHY_DotationLogisticType PHY_DotationLogisticType::uniteFeuSansTD_( "Unite non tranche D", eUniteFeuSansTD );
PHY_DotationLogisticType PHY_DotationLogisticType::uniteVivre_    ( "Unite vivre"  , eUniteVivre     );
PHY_DotationLogisticType PHY_DotationLogisticType::pieces_        ( "Pieces"       , ePieces         );

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

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType constructor
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
PHY_DotationLogisticType::PHY_DotationLogisticType( const std::string& strName, E_DotationLogisticType nType )
    : strName_( strName )
    , nType_  ( nType   )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType destructor
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
PHY_DotationLogisticType::~PHY_DotationLogisticType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::Find
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
const PHY_DotationLogisticType* PHY_DotationLogisticType::Find( const std::string& strName )
{
    CIT_DotationLogisticTypeMap it = dotationLogisticTypes_.find( strName );
    if( it == dotationLogisticTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::Find
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
const PHY_DotationLogisticType* PHY_DotationLogisticType::Find( unsigned int nID )
{
    for( CIT_DotationLogisticTypeMap it = dotationLogisticTypes_.begin(); it != dotationLogisticTypes_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::GetDotationLogisticTypes
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
const PHY_DotationLogisticType::T_DotationLogisticTypeMap& PHY_DotationLogisticType::GetDotationLogisticTypes()
{
    return dotationLogisticTypes_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::GetID
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
unsigned int PHY_DotationLogisticType::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::GetName
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
const std::string& PHY_DotationLogisticType::GetName() const
{
    return strName_;
}
