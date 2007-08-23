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

#include "simulation_kernel_pch.h"

#include "PHY_DotationConsumptions.h"

#include "PHY_DotationType.h"
#include "PHY_DotationConsumption.h"
#include "PHY_DotationGroupContainer.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: PHY_DotationConsumptions constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_DotationConsumptions::PHY_DotationConsumptions( const std::string& strParentTagName, xml::xistream& xis )
{
    xis >> list( "dotation", *this, &PHY_DotationConsumptions::ReadDotation );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationConsumptions::ReadDotation
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_DotationConsumptions::ReadDotation( xml::xistream& xis )
{
    std::string category, name;
    xis >> attribute( "category", category )
        >> attribute( "name", name );

    const PHY_DotationType* pDotationType = PHY_DotationType::FindDotationType( category );
    if( !pDotationType )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown dotation type " + category ); // $$$$ ABL 2007-07-20: error context

    const PHY_DotationCategory* pDotationCategory = pDotationType->FindDotationCategory( name );
    if ( !pDotationCategory )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown dotation category" ); // $$$$ ABL 2007-07-20: error context

    PHY_DotationConsumption*& pDotation = dotationConsumptions_[ pDotationCategory ];
    if ( pDotation )        
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Dotation category already defined" ); // $$$$ ABL 2007-07-20: error context
    pDotation = new PHY_DotationConsumption( *pDotationCategory, xis );
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

