// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationConsumptions.cpp $
// $Author: Jvt $
// $Modtime: 7/02/05 11:29 $
// $Revision: 3 $
// $Workfile: PHY_DotationConsumptions.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_DotationConsumptions.h"

#include "PHY_DotationType.h"
#include "PHY_DotationConsumption.h"
#include "PHY_DotationGroupContainer.h"

// -----------------------------------------------------------------------------
// Name: PHY_DotationConsumptions constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationConsumptions::PHY_DotationConsumptions( const std::string& strParentTagName, MIL_InputArchive& archive )
{
if( !archive.BeginList( strParentTagName, MIL_InputArchive::eNothing ) )
        return;

    while ( archive.NextListElement() )
    {
        archive.Section( "Dotation" );

        std::string strDotationType;
        archive.ReadAttribute( "nom", strDotationType );

        const PHY_DotationType* pDotationType = PHY_DotationType::FindDotationType( strDotationType );
        if( !pDotationType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown dotation type", archive.GetContext() );
       
        ReadDotationCategories( archive, *pDotationType );
    
        archive.EndSection(); // Dotation
    }
    archive.EndList(); // Contenance
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConsumptions::ReadDotationCategories
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
void PHY_DotationConsumptions::ReadDotationCategories( MIL_InputArchive& archive, const PHY_DotationType& dotationType )
{
    archive.BeginList( "Categories" );

    while( archive.NextListElement() )
    {
        archive.Section( "Categorie" );
        
        std::string strCategoryName;
        archive.ReadAttribute( "nom", strCategoryName );

        const PHY_DotationCategory* pDotationCategory = dotationType.FindDotationCategory( strCategoryName );
        if ( !pDotationCategory )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown dotation category", archive.GetContext() );

        PHY_DotationConsumption*& pDotation = dotationConsumptions_[ pDotationCategory ];
        if ( pDotation )        
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Dotation category already defined", archive.GetContext() );
        pDotation = new PHY_DotationConsumption( *pDotationCategory, archive );

        archive.EndSection(); // Categorie
    }
    archive.EndList(); // Categories    
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConsumptions destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationConsumptions::~PHY_DotationConsumptions()
{
    for( CIT_DotationConsumptionMap it = dotationConsumptions_.begin(); it != dotationConsumptions_.end(); ++it )
        delete it->second;
    dotationConsumptions_.clear();
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationConsumptions::RegisterConsumptionReservations
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
bool PHY_DotationConsumptions::RegisterConsumptionReservations( PHY_DotationGroupContainer& container ) const
{
    for( CIT_DotationConsumptionMap it = dotationConsumptions_.begin(); it != dotationConsumptions_.end(); ++it )
    {
        const MT_Float rConsumption = it->second->GetConsumption();
        if( container.AddConsumptionReservation( *it->first, rConsumption ) < rConsumption )
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConsumptions::AddConsumptionValues
// Created: JVT 2005-02-07
// -----------------------------------------------------------------------------
void PHY_DotationConsumptions::AddConsumptionValues( T_ConsumptionValue& result ) const
{
    for ( CIT_DotationConsumptionMap it = dotationConsumptions_.begin(); it != dotationConsumptions_.end(); ++it )
    {
        const PHY_DotationConsumption* pConsumption = it->second;
        
        if ( pConsumption )
            result[ it->first ] += pConsumption->GetConsumption();
    }
}

