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

PHY_DotationNature::T_DotationNatureMap PHY_DotationNature::natures_;
uint                                    PHY_DotationNature::nNextID_ = 0;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature::Initialize
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
void PHY_DotationNature::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing dotation natures" );

    archive.BeginList( "Natures" );

    while( archive.NextListElement() )
    {
        archive.Section( "Nature" );

        std::string strName;
        archive.Read( strName );

        const PHY_DotationNature*& pDotationNature = natures_[ strName ];
        if( pDotationNature )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Dotation nature '%s' already registered", strName.c_str() ) );
        pDotationNature = new PHY_DotationNature( strName );

        archive.EndSection(); // Nature

    }

    archive.EndList(); // Natures
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
