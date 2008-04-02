// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationNature.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 17:27 $
// $Revision: 8 $
// $Workfile: PHY_DotationNature.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_DotationNature.h"
#include "xeumeuleu/xml.h"

using namespace xml;

PHY_DotationNature::T_DotationNatureMap PHY_DotationNature::natures_;
uint                                    PHY_DotationNature::nNextID_ = 0;

// =============================================================================
// MANAGER
// =============================================================================

struct PHY_DotationNature::LoadingWrapper
{
    void ReadNature( xml::xistream& xis )
    {
        PHY_DotationNature::ReadNature( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature::Initialize
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
void PHY_DotationNature::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing dotation natures" );
    LoadingWrapper loader;

    xis >> start( "natures" )
            >> xml::list( "nature", loader, &LoadingWrapper::ReadNature )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature::ReadNature
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_DotationNature::ReadNature( xml::xistream& xis )
{
    std::string strName;
    xis >> attribute( "type", strName );

    const PHY_DotationNature*& pDotationNature = natures_[ strName ];
    if( pDotationNature )
        xis.error( "Dotation nature '" + strName + "' already registered" );
    pDotationNature = new PHY_DotationNature( strName );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature::Terminate
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
void PHY_DotationNature::Terminate()
{
    for( CIT_DotationNatureMap it = natures_.begin(); it != natures_.end(); ++it )
        delete it->second;
    natures_.clear();
}

// =============================================================================
// INSTANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature constructor
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
PHY_DotationNature::PHY_DotationNature( const std::string& strName )
    : strName_( strName )
    , nID_    ( nNextID_++ )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature destructor
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
PHY_DotationNature::~PHY_DotationNature()
{

}
