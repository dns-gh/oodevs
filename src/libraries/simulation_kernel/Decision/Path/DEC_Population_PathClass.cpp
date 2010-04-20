// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Population_PathClass.h"
#include <xeumeuleu/xml.h>



DEC_Population_PathClass::T_Rules DEC_Population_PathClass::rules_;

struct DEC_Population_PathClass::LoadingWrapper
{
    void ReadPopulationRule( xml::xistream& xis )
    {
        DEC_Population_PathClass::ReadPopulationRule( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathClass::Initialize
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
void DEC_Population_PathClass::Initialize( xml::xistream& xis )
{
    LoadingWrapper loader;

    xis >> xml::start( "population-rules" )
            >> xml::list( "rule", loader, &LoadingWrapper::ReadPopulationRule )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathClass::ReadPopulationRule
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void DEC_Population_PathClass::ReadPopulationRule( xml::xistream& xis )
{
    std::string strType;

    xis >> xml::attribute( "type", strType );

    std::string strBase;
    const DEC_Population_PathClass* pBase = 0;
    strBase = "nothing";
    xis >> xml::optional() >> xml::attribute( "inherits", strBase );
    if( strBase != "nothing" )
    {
        pBase  = rules_[ strBase ];
        if( !pBase )
            xis.error( "base rule '" + strBase + "' is undefined" );
    }
    DEC_Population_PathClass*& pRule = rules_[ strType ];
    if( pRule )
        xis.error( "Rule '" + strType + "' already defined" );
    pRule = new DEC_Population_PathClass( xis, pBase );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Population_PathClass::Terminate
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
void DEC_Population_PathClass::Terminate()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Population_PathClass::GetPathClass
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
const DEC_Population_PathClass& DEC_Population_PathClass::GetPathClass( const std::string& type )
{
    const DEC_Population_PathClass* pClass = rules_[ type ];
    if( !pClass )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unable to find PathClass matching PathType: " + type );
    return *pClass;
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathClass constructor
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
DEC_Population_PathClass::DEC_Population_PathClass( xml::xistream& xis, const DEC_Population_PathClass* pCopyFrom /*= 0*/ )
    : rCostOutsideOfChanneling_( 10.f )
    , rChannelingRange_( 1000.f )
{
    if( pCopyFrom )
        *this = *pCopyFrom;

    xis >> xml::optional() >> xml::attribute( "cost-out-of-channeling", rCostOutsideOfChanneling_ )
        >> xml::optional() >> xml::attribute( "channeling-range", rChannelingRange_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathClass destructor
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
DEC_Population_PathClass::~DEC_Population_PathClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathClass::GetCostOutsideOfChanneling
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
double DEC_Population_PathClass::GetCostOutsideOfChanneling() const
{
    return rCostOutsideOfChanneling_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathClass::GetChannelingRange
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
double DEC_Population_PathClass::GetChannelingRange() const
{
    return rChannelingRange_;
}
