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

#include "MIL_Pch.h"

#include "PHY_DotationType.h"
#include "PHY_DotationCategory.h"

PHY_DotationType PHY_DotationType::munition_ ( "munition" , eMunition  );
PHY_DotationType PHY_DotationType::carburant_( "carburant", eCarburant );
PHY_DotationType PHY_DotationType::mine_     ( "mine"     , eMine      );
PHY_DotationType PHY_DotationType::explosif_ ( "explosif" , eExplosif  );
PHY_DotationType PHY_DotationType::barbele_  ( "barbele"  , eBarbele   );
PHY_DotationType PHY_DotationType::piece_    ( "piece"    , ePiece     );
PHY_DotationType PHY_DotationType::ration_   ( "ration"   , eRation    );

PHY_DotationType::T_DotationTypeMap       PHY_DotationType::dotationTypes_;
PHY_DotationType::T_DotationCategoryIDMap PHY_DotationType::dotationCategorieIDs_;

//=============================================================================
// STATIC INITIALIZATION 
//=============================================================================

//-----------------------------------------------------------------------------
// Name: PHY_DotationType::Initialize
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
void PHY_DotationType::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing dotation types" );

    dotationTypes_[ munition_ .GetName() ] = &munition_;
    dotationTypes_[ carburant_.GetName() ] = &carburant_;
    dotationTypes_[ mine_     .GetName() ] = &mine_;
    dotationTypes_[ explosif_ .GetName() ] = &explosif_;
    dotationTypes_[ barbele_  .GetName() ] = &barbele_;
    dotationTypes_[ piece_    .GetName() ] = &piece_;
    dotationTypes_[ ration_   .GetName() ] = &ration_;

    archive.BeginList( "Dotations" );
    while( archive.NextListElement() )
    {
        archive.Section( "Dotation" );

        std::string strTypeName;
        archive.ReadAttribute( "nom", strTypeName );

        CIT_DotationTypeMap it = dotationTypes_.find( strTypeName );
        if( it == dotationTypes_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Invalid dotation type name", archive.GetContext() );

        archive.BeginList( "Categories" );
        while( archive.NextListElement() )
        {
            archive.Section( "Categorie" );
            std::string strCategoryName;
            archive.ReadAttribute( "nom", strCategoryName );
            const_cast< PHY_DotationType& >( *it->second ).RegisterDotationCategory( strCategoryName, archive );
            archive.EndSection(); // Categorie
        }
        archive.EndList(); // Categories

        archive.EndSection(); // Dotation
    }
    archive.EndList(); // Dotations
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
PHY_DotationType::PHY_DotationType( const std::string& strName, E_DotationType nType )
    : strName_( strName )
    , nType_  ( nType )
{   
}

//-----------------------------------------------------------------------------
// Name: PHY_DotationType::Initialize
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
void PHY_DotationType::RegisterDotationCategory( const std::string& strCategoryName, MIL_InputArchive& archive )
{
    const PHY_DotationCategory*& pCategory = dotationCategories_[ strCategoryName ];
    if( pCategory )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Dotation category '%s' already registered", strCategoryName.c_str() ), archive.GetContext() );

    pCategory = new PHY_DotationCategory( *this, strCategoryName, archive );

    const PHY_DotationCategory*& pCategoryID = dotationCategorieIDs_[ pCategory->GetMosID() ];
    if( pCategoryID )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Dotation category ID already registered", strCategoryName.c_str() ), archive.GetContext() );
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
