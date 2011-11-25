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
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

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
        >> xml::end;
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
    xis >> xml::optional >> xml::attribute( "inherits", strBase );
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
    , bAvoidObjects_( true )
    , rObstructionThreshold_( 10000 )
{
    if( pCopyFrom )
        *this = *pCopyFrom;

    xis >> xml::optional >> xml::attribute( "cost-out-of-channeling", rCostOutsideOfChanneling_ )
        >> xml::optional >> xml::attribute( "channeling-range", rChannelingRange_ )
        >> xml::list( "object-costs", *this, &DEC_Population_PathClass::ReadObjectsCost );
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
// Name: DEC_Population_PathClass::ReadObjectsCost
// Created: CMA 2011-11-24
// -----------------------------------------------------------------------------
void DEC_Population_PathClass::ReadObjectsCost( xml::xistream& xis )
{
    xis >> xml::attribute( "avoid", bAvoidObjects_ )
        >> xml::attribute( "threshold", rObstructionThreshold_ )
        >> xml::list( "object-cost", *this, &DEC_Population_PathClass::ReadObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathClass::ReadObject
// Created: CMA 2011-11-24
// -----------------------------------------------------------------------------
void DEC_Population_PathClass::ReadObject( xml::xistream& xis )
{
    std::string strType( xis.attribute< std::string >( "type", std::string() ) );
    try
    {
        const MIL_ObjectType_ABC& objectType = MIL_ObjectFactory::FindType( strType );
        unsigned int id = objectType.GetID();
        if( objectCosts_.size() <= id )
            objectCosts_.resize( id + 1, 0 );
        assert( objectCosts_.size() > id );
        xis >> xml::attribute( "value", objectCosts_[ id ] );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathClass::GetObjectCost
// Created: CMA 2011-11-24
// -----------------------------------------------------------------------------
double DEC_Population_PathClass::GetObjectCost( const MIL_ObjectType_ABC& objectType ) const
{
   if( objectCosts_.size() <= objectType.GetID() )
        return 0.;
    return objectCosts_[ objectType.GetID() ];
}
