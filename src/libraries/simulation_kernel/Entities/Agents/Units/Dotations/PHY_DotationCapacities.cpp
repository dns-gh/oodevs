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

#include "simulation_kernel_pch.h"
#include "PHY_DotationCapacities.h"
#include "PHY_DotationType.h"
#include "PHY_DotationCapacity.h"
#include "PHY_DotationGroupContainer.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacities constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationCapacities::PHY_DotationCapacities( const std::string& strParentTagName, xml::xistream& xis )
{
    xis >> xml::optional
        >> xml::start( strParentTagName )
            >> xml::list( "category", *this, &PHY_DotationCapacities::ReadCategory )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacities::ReadCategory
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_DotationCapacities::ReadCategory( xml::xistream& xis )
{
    std::string strDotationType;
    xis >> xml::attribute( "name", strDotationType );

    const PHY_DotationType* pDotationType = PHY_DotationType::FindDotationType( strDotationType );
    if( !pDotationType )
        xis.error( "Unknown dotation type" );

    ReadDotationCategories( xis, *pDotationType );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacities::ReadDotationCategories
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
void PHY_DotationCapacities::ReadDotationCategories( xml::xistream& xis, const PHY_DotationType& dotationType )
{
    xis >> xml::list( "resource", *this, &PHY_DotationCapacities::ReadDotation, dotationType );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacities::ReadDotation
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void PHY_DotationCapacities::ReadDotation( xml::xistream& xis, const PHY_DotationType& /*dotationType*/ )
{
    std::string strCategoryName;
    xis >> xml::attribute( "name", strCategoryName );

    const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( strCategoryName );
    if( !pDotationCategory )
        xis.error( "Unknown dotation category" );

    PHY_DotationCapacity*& pDotation = dotationCapacities_[ pDotationCategory ];
    if( pDotation )
        xis.error( "Dotation already defined" );
    pDotation = new PHY_DotationCapacity( *pDotationCategory, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacities destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationCapacities::~PHY_DotationCapacities()
{
    for( auto it = dotationCapacities_.begin(); it != dotationCapacities_.end(); ++it )
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
void PHY_DotationCapacities::RegisterCapacities( PHY_DotationGroupContainer& container, std::map< const PHY_DotationCategory*, double >* dotations ) const
{
    for( auto it = dotationCapacities_.begin(); it != dotationCapacities_.end(); ++it )
    {
        double quantity = std::numeric_limits< double >::max();
        if( dotations )
        {
            std::map< const PHY_DotationCategory*, double >::const_iterator itDotation = dotations->find( it->first );
            if( itDotation != dotations->end() )
                quantity = itDotation->second;
        }
        container.AddCapacity( *it->second, quantity );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacities::UnregisterCapacities
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
std::map< const PHY_DotationCategory*, double > PHY_DotationCapacities::UnregisterCapacities( PHY_DotationGroupContainer& container ) const
{
    std::map< const PHY_DotationCategory*, double > result;
    for( auto it = dotationCapacities_.begin(); it != dotationCapacities_.end(); ++it )
        result[ &( it->second->GetCategory() ) ] = container.RemoveCapacity( *it->second );
    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacities::GetDefaultLogisticThreshold
// Created: JSR 2013-04-29
// -----------------------------------------------------------------------------
double PHY_DotationCapacities::GetDefaultLogisticThreshold( const PHY_DotationCategory& category ) const
{
    auto it = dotationCapacities_.find( &category );
    if( it != dotationCapacities_.end() )
        return it->second->GetDefaultThreshold();
    return -1;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCapacities::GetNormalizedConsumption
// Created: JSR 2013-08-07
// -----------------------------------------------------------------------------
double PHY_DotationCapacities::GetNormalizedConsumption( const PHY_DotationCategory& category ) const
{
    auto it = dotationCapacities_.find( &category );
    if( it != dotationCapacities_.end() )
        return it->second->GetNormalizedConsumption();
    return 0;
}
