// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCapacities.cpp $
// $Author: Nld $
// $Modtime: 26/01/05 14:45 $
// $Revision: 4 $
// $Workfile: PHY_DotationCapacities.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_DotationCapacities.h"

#include "PHY_DotationType.h"
#include "PHY_DotationCapacity.h"
#include "PHY_DotationGroupContainer.h"
#include "PHY_DotationStockContainer.h"

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacities constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationCapacities::PHY_DotationCapacities( const std::string& strParentTagName, MIL_InputArchive& archive )
{
    if ( !archive.BeginList( strParentTagName, MIL_InputArchive::eNothing ) )
        return;

    while ( archive.NextListElement() )
    {
        archive.Section( "Dotation" );

        std::string strDotationType;
        archive.ReadAttribute( "nom", strDotationType );

        const PHY_DotationType* pDotationType = PHY_DotationType::FindDotationType( strDotationType );
        if ( !pDotationType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown dotation type", archive.GetContext() );
       
        ReadDotationCategories( archive, *pDotationType );
    
        archive.EndSection(); // Dotation
    }
    archive.EndList(); // Contenance
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacities::ReadDotationCategories
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
void PHY_DotationCapacities::ReadDotationCategories( MIL_InputArchive& archive, const PHY_DotationType& dotationType )
{
    archive.BeginList( "Categories" );

    while ( archive.NextListElement() )
    {
        archive.Section( "Categorie" );
        
        std::string strCategoryName;
        archive.ReadAttribute( "nom", strCategoryName );

        const PHY_DotationCategory* pDotationCategory = dotationType.FindDotationCategory( strCategoryName );
        if ( !pDotationCategory )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown dotation category", archive.GetContext() );

        PHY_DotationCapacity*& pDotation = dotationCapacities_[ pDotationCategory ];
        if( pDotation )        
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Dotation already defined", archive.GetContext() );
        pDotation = new PHY_DotationCapacity( *pDotationCategory, archive );

        archive.EndSection(); // Categorie
    }
    archive.EndList(); // Categories    
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacities destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationCapacities::~PHY_DotationCapacities()
{
    for( CIT_DotationCapacityMap it = dotationCapacities_.begin(); it != dotationCapacities_.end(); ++it )
        delete it->second;
    dotationCapacities_.clear();
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacities::RegisterCapacities
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_DotationCapacities::RegisterCapacities( PHY_DotationGroupContainer& container ) const
{
    for( CIT_DotationCapacityMap it = dotationCapacities_.begin(); it != dotationCapacities_.end(); ++it )
        container.AddCapacity( *it->second );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacities::UnregisterCapacities
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
void PHY_DotationCapacities::UnregisterCapacities( PHY_DotationGroupContainer& container ) const
{
    for( CIT_DotationCapacityMap it = dotationCapacities_.begin(); it != dotationCapacities_.end(); ++it )
        container.RemoveCapacity( *it->second );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacities::RegisterCapacities
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_DotationCapacities::RegisterCapacities( PHY_DotationStockContainer& container ) const
{
    for( CIT_DotationCapacityMap it = dotationCapacities_.begin(); it != dotationCapacities_.end(); ++it )
        container.AddCapacity( *it->second );    
}
