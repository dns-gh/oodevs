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
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

PHY_DotationNature::T_DotationNatureMap PHY_DotationNature::natures_;
unsigned int PHY_DotationNature::nNextID_ = 0;

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

    xis >> xml::start( "natures" )
            >> xml::list( "nature", loader, &LoadingWrapper::ReadNature )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature::ReadNature
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_DotationNature::ReadNature( xml::xistream& xis )
{
    std::string strName;
    xis >> xml::attribute( "type", strName );

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

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature constructor
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
PHY_DotationNature::PHY_DotationNature( const std::string& strName )
    : strName_( strName )
    , nID_    ( nNextID_++ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature destructor
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
PHY_DotationNature::~PHY_DotationNature()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature::GetName
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
const std::string& PHY_DotationNature::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature::Find
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
const PHY_DotationNature* PHY_DotationNature::Find( const std::string& strName )
{
    CIT_DotationNatureMap it = natures_.find( strName );
    if( it == natures_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature::operator==
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
bool PHY_DotationNature::operator==( const PHY_DotationNature& rhs ) const
{
    return nID_ == rhs.nID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature::operator!=
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
bool PHY_DotationNature::operator!=( const PHY_DotationNature& rhs ) const
{
    return nID_ != rhs.nID_;
}
