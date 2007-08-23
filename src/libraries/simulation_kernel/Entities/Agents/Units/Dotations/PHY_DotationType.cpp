// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationType.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 17:27 $
// $Revision: 8 $
// $Workfile: PHY_DotationType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_DotationType.h"

#include "PHY_DotationCategory.h"
#include "PHY_DotationLogisticType.h"
#include "xeumeuleu/xml.h"

using namespace xml;

PHY_DotationType PHY_DotationType::munition_ ( "munition" , eMunition , EnumDotationFamily::munition , PHY_DotationLogisticType::uniteFeuSansTD_ );
PHY_DotationType PHY_DotationType::carburant_( "carburant", eCarburant, EnumDotationFamily::carburant, PHY_DotationLogisticType::uniteEssence_   );
PHY_DotationType PHY_DotationType::mine_     ( "mine"     , eMine     , EnumDotationFamily::mine     , PHY_DotationLogisticType::uniteFeuSansTD_ );
PHY_DotationType PHY_DotationType::explosif_ ( "explosif" , eExplosif , EnumDotationFamily::explosif , PHY_DotationLogisticType::uniteFeuSansTD_ );
PHY_DotationType PHY_DotationType::barbele_  ( "barbele"  , eBarbele  , EnumDotationFamily::barbele  , PHY_DotationLogisticType::uniteFeuSansTD_ );
PHY_DotationType PHY_DotationType::piece_    ( "piece"    , ePiece    , EnumDotationFamily::piece    , PHY_DotationLogisticType::pieces_         );
PHY_DotationType PHY_DotationType::ration_   ( "ration"   , eRation   , EnumDotationFamily::ration   , PHY_DotationLogisticType::uniteVivre_     );

PHY_DotationType::T_DotationTypeMap       PHY_DotationType::dotationTypes_;
PHY_DotationType::T_DotationCategoryIDMap PHY_DotationType::dotationCategorieIDs_;

//=============================================================================
// STATIC INITIALIZATION
//=============================================================================

struct PHY_DotationType::LoadingWrapper
{
    void ReadDotation( xml::xistream& xis )
    {
        PHY_DotationType::ReadDotation( xis );
    }
};

//-----------------------------------------------------------------------------
// Name: PHY_DotationType::Initialize
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
void PHY_DotationType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing dotation types" );

    dotationTypes_[ munition_ .GetName() ] = &munition_;
    dotationTypes_[ carburant_.GetName() ] = &carburant_;
    dotationTypes_[ mine_     .GetName() ] = &mine_;
    dotationTypes_[ explosif_ .GetName() ] = &explosif_;
    dotationTypes_[ barbele_  .GetName() ] = &barbele_;
    dotationTypes_[ piece_    .GetName() ] = &piece_;
    dotationTypes_[ ration_   .GetName() ] = &ration_;

    LoadingWrapper loader;

    xis >> start( "dotations" )
            >> list( "dotation", loader, &LoadingWrapper::ReadDotation )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::ReadDotation
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_DotationType::ReadDotation( xml::xistream& xis )
{
    std::string strTypeName;
    std::string strCategoryName;

    xis >> attribute( "name", strTypeName )
        >> attribute( "category", strCategoryName );

    CIT_DotationTypeMap it = dotationTypes_.find( strTypeName );
    if( it == dotationTypes_.end() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Invalid dotation type name" ); // $$$$ ABL 2007-07-19: error context

    const_cast< PHY_DotationType& >( *it->second ).RegisterDotationCategory( strCategoryName, xis );
}

//-----------------------------------------------------------------------------
// Name: PHY_DotationType::Terminate
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
void PHY_DotationType::Terminate()
{
    for( CIT_DotationTypeMap itDotationType = dotationTypes_.begin(); itDotationType != dotationTypes_.end(); ++itDotationType )
        delete itDotationType->second;
    dotationTypes_.clear();
}

//=============================================================================
//
//=============================================================================

//-----------------------------------------------------------------------------
// Name: PHY_DotationType::Initialize
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
PHY_DotationType::PHY_DotationType( const std::string& strName, E_DotationType nType, ASN1T_EnumDotationFamily nAsnID, const PHY_DotationLogisticType& defaultLogisticType )
    : strName_            ( strName )
    , nType_              ( nType )
    , nAsnID_             ( nAsnID )
    , defaultLogisticType_( defaultLogisticType )
    , dotationCategories_ ()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: PHY_DotationType::Initialize
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
void PHY_DotationType::RegisterDotationCategory( const std::string& strCategoryName, xml::xistream& xis )
{
    const PHY_DotationCategory*& pCategory = dotationCategories_[ strCategoryName ];
    if( pCategory )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Dotation category '%s' already registered", strCategoryName.c_str() ) ); // $$$$ ABL 2007-07-19: error context

    pCategory = new PHY_DotationCategory( *this, strCategoryName, xis );

    const PHY_DotationCategory*& pCategoryID = dotationCategorieIDs_[ pCategory->GetMosID() ];
    if( pCategoryID )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Dotation category ID already registered", strCategoryName.c_str() ) ); // $$$$ ABL 2007-07-19: error context
    pCategoryID = pCategory;
}

//-----------------------------------------------------------------------------
// Name: PHY_DotationType::Initialize
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
PHY_DotationType::~PHY_DotationType()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::InternalFindDotationCategory
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
const PHY_DotationCategory* PHY_DotationType::InternalFindDotationCategory( uint nID ) const
{
    for( CIT_DotationCategoryMap it = dotationCategories_.begin(); it != dotationCategories_.end(); ++it )
    {
        if( it->second->GetMosID() == nID )
            return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::FindDotationCategory
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
const PHY_DotationCategory* PHY_DotationType::InternalFindDotationCategory( const std::string& strName ) const
{
    CIT_DotationCategoryMap it = dotationCategories_.find( strName );
    if( it == dotationCategories_.end() )
        return 0;
    return it->second;
}
