//*****************************************************************************
//
// $Created: JDY 03-04-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathType.cpp $
// $Author: Nld $
// $Modtime: 21/07/05 15:54 $
// $Revision: 11 $
// $Workfile: DEC_PathType.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "DEC_PathType.h"
#include "DEC_Path.h"
#include "DEC_PathClass.h"
#include "Entities/Agents/MIL_AgentPion.h"

DEC_PathType::T_Rules DEC_PathType::rules_;

//-----------------------------------------------------------------------------
// Name: DEC_PathType constructor
// Created: JDY 03-04-14
//-----------------------------------------------------------------------------
DEC_PathType::DEC_PathType( E_PathType nType )
    : nPathType_( nType )
{
}


//-----------------------------------------------------------------------------
// Name: DEC_PathType destructor
// Created: JDY 03-04-14
//-----------------------------------------------------------------------------
DEC_PathType::~DEC_PathType()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_PathType::CreateRule
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
TerrainRule_ABC& DEC_PathType::CreateRule( const DEC_Path& path, const MT_Vector2D& from, const MT_Vector2D& to )
{
    const bool bCanFly = path.GetQueryMaker().CanFly();
    const DEC_PathClass* pClass = rules_[ T_RuleType( ConvertPathTypeToString(), bCanFly ) ];
    assert( pClass );
    return pClass->CreateRule( path, from, to );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathType::InitializeRules
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_PathType::InitializeRules( MIL_InputArchive& archive )
{
    archive.BeginList( "Rules" );
    while( archive.NextListElement() )
    {
        archive.Section( "Rule" );
        std::string strType;
        archive.ReadAttribute( "type", strType );
        bool bFlying;
        archive.ReadAttribute( "flying", bFlying);
        std::string strBase;
        const DEC_PathClass* pBase = 0;
        if( archive.ReadAttribute( "inherits", strBase, MIL_InputArchive::eNothing ) )
        {
            pBase  = rules_[ T_RuleType( strBase, bFlying ) ];
            if( ! pBase )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "base rule '" + strBase + "' is undefined", archive.GetContext() );
        }
        DEC_PathClass*& pRule = rules_[ T_RuleType( strType, bFlying ) ];
        if( pRule )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Rule '" + strType + "' already defined", archive.GetContext() );
        pRule = new DEC_PathClass( archive, pBase );
        archive.EndSection(); // Rule
    };
    archive.EndList(); // Rules

    CheckRulesExistence();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathType::CheckRulesExistence
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_PathType::CheckRulesExistence()
{
    for( int type = 0; type < eNbrPathType; ++type )
    {
        const std::string typeName = DEC_PathType( E_PathType( type ) ).ConvertPathTypeToString();
        if( ! rules_[ T_RuleType( typeName, false ) ] )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Rule '" + typeName + "' is not defined for non flying units"  );
        if( ! rules_[ T_RuleType( typeName, true ) ] )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Rule '" + typeName + "' is not defined for flying units" );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PathType::ConvertPathTypeToString
// Created: NLD 2003-09-25
// -----------------------------------------------------------------------------
std::string DEC_PathType::ConvertPathTypeToString() const
{
    static std::string names[ eNbrPathType ] = {
        "movement",
        "recon",
        "infiltration",
        "assault",
        "retreat",
        "backup",
        "minesweep"
    };
    if( nPathType_ < eNbrPathType )
        return names[ nPathType_ ];
    return "bad type";
}
