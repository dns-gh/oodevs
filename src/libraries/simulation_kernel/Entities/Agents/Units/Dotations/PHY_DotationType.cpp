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
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "PHY_DotationType.h"
#include "PHY_DotationCategory.h"
#include "PHY_DotationLogisticType.h"
#include "protocol/Protocol.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

PHY_DotationType* PHY_DotationType::munition_ = 0;
PHY_DotationType* PHY_DotationType::carburant_ = 0;
PHY_DotationType* PHY_DotationType::mine_ = 0;
PHY_DotationType* PHY_DotationType::explosif_ = 0;
PHY_DotationType* PHY_DotationType::barbele_ = 0;
PHY_DotationType* PHY_DotationType::piece_ = 0;
PHY_DotationType* PHY_DotationType::ration_ = 0;
PHY_DotationType* PHY_DotationType::agentExtincteur_ = 0;
PHY_DotationType* PHY_DotationType::energie_ = 0;

PHY_DotationType::T_DotationTypeMap       PHY_DotationType::dotationTypes_;
PHY_DotationType::T_DotationCategoryIDMap PHY_DotationType::dotationCategorieIDs_;

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
    PHY_DotationType::munition_        = new PHY_DotationType( "munition"         , eMunition       , PHY_DotationLogisticType::uniteFeuSansTD_ );
    PHY_DotationType::carburant_       = new PHY_DotationType( "carburant"        , eCarburant      , PHY_DotationLogisticType::uniteEssence_   );
    PHY_DotationType::mine_            = new PHY_DotationType( "mine"             , eMine           , PHY_DotationLogisticType::uniteFeuSansTD_ );
    PHY_DotationType::explosif_        = new PHY_DotationType( "explosif"         , eExplosif       , PHY_DotationLogisticType::uniteFeuSansTD_ );
    PHY_DotationType::barbele_         = new PHY_DotationType( "barbele"          , eBarbele        , PHY_DotationLogisticType::uniteFeuSansTD_ );
    PHY_DotationType::piece_           = new PHY_DotationType( "piece"            , ePiece          , PHY_DotationLogisticType::pieces_         );
    PHY_DotationType::ration_          = new PHY_DotationType( "ration"           , eRation         , PHY_DotationLogisticType::uniteVivre_     );
    PHY_DotationType::agentExtincteur_ = new PHY_DotationType( "agent extincteur" , eAgentExtincteur, PHY_DotationLogisticType::uniteVivre_     );
    PHY_DotationType::energie_ = new PHY_DotationType( "energie" , eEnergie, PHY_DotationLogisticType::uniteVivre_ );

    dotationTypes_[ munition_       ->GetName() ] = munition_;
    dotationTypes_[ carburant_      ->GetName() ] = carburant_;
    dotationTypes_[ mine_           ->GetName() ] = mine_;
    dotationTypes_[ explosif_       ->GetName() ] = explosif_;
    dotationTypes_[ barbele_        ->GetName() ] = barbele_;
    dotationTypes_[ piece_          ->GetName() ] = piece_;
    dotationTypes_[ ration_         ->GetName() ] = ration_;
    dotationTypes_[ agentExtincteur_->GetName() ] = agentExtincteur_;
    dotationTypes_[ energie_->GetName() ] = energie_;
    LoadingWrapper loader;
    xis >> xml::start( "resources" )
            >> xml::list( "resource", loader, &LoadingWrapper::ReadDotation )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::ReadDotation
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_DotationType::ReadDotation( xml::xistream& xis )
{
    std::string name;
    std::string category;

    xis >> xml::attribute( "name", name )
        >> xml::attribute( "category", category );

    CIT_DotationTypeMap it = dotationTypes_.find( category );
    if( it == dotationTypes_.end() )
        xis.error( "Invalid dotation category name" );

    const_cast< PHY_DotationType& >( *it->second ).RegisterDotation( name, xis );
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
    dotationCategorieIDs_.clear();
}

//-----------------------------------------------------------------------------
// Name: PHY_DotationType::RegisterDotation
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
void PHY_DotationType::RegisterDotation( const std::string& strCategoryName, xml::xistream& xis )
{
    const PHY_DotationCategory*& pCategory = dotationCategories_[ strCategoryName ];
    if( pCategory )
        xis.error( "Dotation category '" + strCategoryName + "' already registered" );
    pCategory = new PHY_DotationCategory( *this, strCategoryName, xis );
    const PHY_DotationCategory*& pCategoryID = dotationCategorieIDs_[ pCategory->GetMosID() ];
    if( pCategoryID )
        xis.error( "Dotation category ID already registered" );
    pCategoryID = pCategory;
}

//-----------------------------------------------------------------------------
// Name: PHY_DotationType constructor
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
PHY_DotationType::PHY_DotationType( const std::string& strName, E_DotationType nType, const PHY_DotationLogisticType& defaultLogisticType )
    : strName_            ( strName )
    , nType_              ( nType )
    , defaultLogisticType_( defaultLogisticType )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: PHY_DotationType::Initialize
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
PHY_DotationType::~PHY_DotationType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::InternalFindDotationCategory
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
const PHY_DotationCategory* PHY_DotationType::InternalFindDotationCategory( unsigned int nID ) const
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

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::FindDotationType
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
const PHY_DotationType* PHY_DotationType::FindDotationType( const std::string& strName )
{
    CIT_DotationTypeMap itDotationType = dotationTypes_.find( strName );
    if( itDotationType == dotationTypes_.end() )
        return 0;
    return itDotationType->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::FindDotationType
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
const PHY_DotationType* PHY_DotationType::FindDotationType( unsigned int nID )
{
    for( CIT_DotationTypeMap it = dotationTypes_.begin(); it != dotationTypes_.end(); ++it )
    {
        if( it->second->GetID() == nID )
            return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::FindDotationCategory
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
const PHY_DotationCategory* PHY_DotationType::FindDotationCategory( unsigned int nID )
{
    for( CIT_DotationTypeMap it = dotationTypes_.begin(); it != dotationTypes_.end(); ++it )
    {
        const PHY_DotationCategory* pDotationCategory = it->second->InternalFindDotationCategory( nID );
        if( pDotationCategory )
            return pDotationCategory;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::FindDotationCategory
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
const PHY_DotationCategory* PHY_DotationType::FindDotationCategory( const std::string& strName )
{
    for( CIT_DotationTypeMap it = dotationTypes_.begin(); it != dotationTypes_.end(); ++it )
    {
        const PHY_DotationCategory* pDotationCategory = it->second->InternalFindDotationCategory( strName );
        if( pDotationCategory )
            return pDotationCategory;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::GetName
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
const std::string& PHY_DotationType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::GetID
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
unsigned int PHY_DotationType::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::GetDefaultLogisticType
// Created: NLD 2006-01-04
// -----------------------------------------------------------------------------
const PHY_DotationLogisticType& PHY_DotationType::GetDefaultLogisticType() const
{
    return defaultLogisticType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::operator==
// Created: NLD 2005-02-28
// -----------------------------------------------------------------------------
bool PHY_DotationType::operator==( const PHY_DotationType& rhs ) const
{
    return rhs.nType_ == nType_;
}
