// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Population_PathClass.h"
#include "Decision/Path/DEC_PathType.h"

DEC_Population_PathClass::T_Rules DEC_Population_PathClass::rules_;

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathClass::Initialize
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
void DEC_Population_PathClass::Initialize( MIL_InputArchive& archive )
{
    archive.BeginList( "PopulationRules" );
    while( archive.NextListElement() )
    {
        archive.Section( "Rule" );
        std::string strType;

        archive.ReadAttribute( "type", strType );

        std::string strBase;
        const DEC_Population_PathClass* pBase = 0;
        if( archive.ReadAttribute( "inherits", strBase, MIL_InputArchive::eNothing ) )
        {
            pBase  = rules_[ strBase ];
            if( !pBase )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "base rule '" + strBase + "' is undefined", archive.GetContext() );
        }
        DEC_Population_PathClass*& pRule = rules_[ strType ];
        if( pRule )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Rule '" + strType + "' already defined", archive.GetContext() );
        pRule = new DEC_Population_PathClass( archive, pBase );
        archive.EndSection(); // Rule
    };
    archive.EndList(); // PopulationRules
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
DEC_Population_PathClass::DEC_Population_PathClass( MIL_InputArchive& archive, const DEC_Population_PathClass* pCopyFrom /*= 0*/ )
    : rCostOutsideOfChanneling_( 10.f )
    , rChannelingRange_( 1000.f )
{
    if( pCopyFrom )
        *this = *pCopyFrom;

    archive.ReadField( "CostOutsideOfChanneling", rCostOutsideOfChanneling_, MIL_InputArchive::eNothing );
    archive.ReadField( "ChannelingRange"        , rChannelingRange_        , MIL_InputArchive::eNothing );
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
