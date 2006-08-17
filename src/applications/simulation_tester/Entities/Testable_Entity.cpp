// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Testable_Entity.h"
#include "Automat.h"
#include "Pawn.h"
#include "GenObject.h"
#include "Actions/Scheduler.h"
#include "Types/TacticalLineManager.h"
#include "Tools/Position.h"
#include "Tools/Path.h"
#include "Tools/Location.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Testable_Entity constructor
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
Testable_Entity::Testable_Entity( const Workspace& workspace )
    : workspace_( workspace )
    , nLeftLimit_( 0 )
    , nRightLimit_( 0 )
    , nLastRecompletion_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Testable_Entity destructor
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
Testable_Entity::~Testable_Entity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetAutomat
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
const Automat* Testable_Entity::GetAutomat() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_ID
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
T_EntityId Testable_Entity::GetTestParam_ID() const
{
    // for oid_obstacle_planifie
    return ( uint )rand();
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_Agent
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
T_EntityId Testable_Entity::GetTestParam_Agent() const
{
    // return pawn ID
    return GetId();
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_AgentList
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
T_IdVector& Testable_Entity::GetTestParam_AgentList() const
{
    // get up to 5 agent ids from same knowledge group != GetId()
    return workspace_.GetEntityManager().GetTestParam_AgentList( 5, *this );
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_Automat
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
T_EntityId Testable_Entity::GetTestParam_Automate() const
{
    if( !GetAutomat() )
        throw std::runtime_error( "Trying to get \"automat\" for an entity with no Automat" );
    return GetAutomat()->GetId();
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_AutomateList
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
T_IdVector& Testable_Entity::GetTestParam_AutomateList() const
{
    // get up to 5 automats ids from same knowledge group != GetId()
    return workspace_.GetEntityManager().GetTestParam_AutomateList( 5, *this );
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_PopulationKnowledge
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
T_EntityId Testable_Entity::GetTestParam_PopulationKnowledge() const
{
    if( !GetAutomat() )
        throw std::runtime_error( "Trying to get \"population knowledge\" for an entity with no Automat/Knowledge group" );
    return GetAutomat()->GetKnowledgeGroup().GetTestParam_KnowledgePopulation();
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_AgentKnowledge
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
T_EntityId Testable_Entity::GetTestParam_AgentKnowledge() const
{
    if( !GetAutomat() )
        throw std::runtime_error( "Trying to get \"agent knowledge\" for an entity with no Automat/Knowledge group" );
    return GetAutomat()->GetKnowledgeGroup().GetTestParam_Knowledge();
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_AgentKnowledgeList
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
T_IdVector& Testable_Entity::GetTestParam_AgentKnowledgeList() const
{
    // get up to 5 knowledges (enemies)
    if( !GetAutomat() )
        throw std::runtime_error( "Trying to get \"agent knowledge list\" for an entity with no Automat/Knowledge group" );
    return GetAutomat()->GetKnowledgeGroup().GetTestParam_Knowledges( 5 );
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_ObjectKnowledge
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
T_EntityId Testable_Entity::GetTestParam_ObjectKnowledge() const
{
    if( !GetAutomat() )
        throw std::runtime_error( "Trying to get \"object knowledge\" for an entity with no Automat/Knowledge group" );
    return GetAutomat()->GetTeam().GetTestParam_Object();
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_ObjectKnowledgeList
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
T_IdVector& Testable_Entity::GetTestParam_ObjectKnowledgeList() const
{
    if( !GetAutomat() )
        throw std::runtime_error( "Trying to get \"object knowledge list\" for an entity with no Automat/Knowledge group" );
    return GetAutomat()->GetTeam().GetTestParam_Objects();
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_GenObject
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
GenObject& Testable_Entity::GetTestParam_GenObject() const
{
    return GenObject::GetTestParam_GenObject( *this );
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_GenObjectList
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
T_GenObjectVector& Testable_Entity::GetTestParam_GenObjectList() const
{
    return GenObject::GetTestParam_GenObjectList( *this, 5 );
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_Position
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Position& Testable_Entity::GetTestParam_Point() const
{
    double rX = GetPosition().GetSimX();
    double rY = GetPosition().GetSimY();

    rX += 2000.0 * ( rand() * 1.0 / RAND_MAX - 0.5 );
    rY += 2000.0 * ( rand() * 1.0 / RAND_MAX - 0.5 );
    Position& pos = *new Position();
    pos.SetSimCoordinates( rX, rY );
    return pos;
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_PointList
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
T_PositionVector& Testable_Entity::GetTestParam_PointList() const
{
    T_PositionVector& points = *new T_PositionVector();
    // generate 5 points
    for( uint i = 0; i < 4; ++i )
        points.push_back( &GetTestParam_Point() );
    return points;
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_Direction
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
uint Testable_Entity::GetTestParam_Direction() const
{
    return GetTestParam_Numeric( 0, 359 );
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_Path
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
Path& Testable_Entity::GetTestParam_Path() const
{
    return Path::GetTestParam_Path( GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_PathList
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
T_PathVector& Testable_Entity::GetTestParam_PathList() const
{
    return Path::GetTestParam_PathList( GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_Bool
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
bool Testable_Entity::GetTestParam_Bool() const
{
    return GetTestParam_Numeric( 0, 1 ) ? true : false;
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_Enumeration
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
uint Testable_Entity::GetTestParam_Enumeration( uint nMin, uint nMax ) const
{
    return GetTestParam_Numeric( nMin, nMax );
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_Numeric
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
int Testable_Entity::GetTestParam_Numeric( int nMin /* = 0 */, int nMax /* = std::numeric_limits< int >::max() */ ) const
{
    if( nMin > nMax )
        return 0;
    return ( uint )( rand() * ( double )( nMax - nMin ) / RAND_MAX ) + nMin;
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_Location
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
Location& Testable_Entity::GetTestParam_Location() const
{
    return Location::GetTestParam_Location( GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_LocationList
// Created: SBO 2006-03-15
// -----------------------------------------------------------------------------
T_LocationVector& Testable_Entity::GetTestParam_LocationList() const
{
    // retrieve 3 random lines
    return Location::GetTestParam_LocationList( GetPosition(), 3, EnumTypeLocalisation::line );
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_Polygon
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
Location& Testable_Entity::GetTestParam_Polygon() const
{
    return Location::GetTestParam_Location( GetPosition(), EnumTypeLocalisation::polygon );
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_PolygonList
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
T_LocationVector& Testable_Entity::GetTestParam_PolygonList() const
{
    // retrieve 3 random polygons
    return Location::GetTestParam_LocationList( GetPosition(), 3, EnumTypeLocalisation::polygon );
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_NatureAtlas
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
uint Testable_Entity::GetTestParam_NatureAtlas() const
{
    // 12 atlas natures
    static int nAtlasNatures[ 12 ] = { 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 
                                       0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010 };
    return nAtlasNatures[ GetTestParam_Numeric( 0, 11 ) ];
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_GDH
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
uint Testable_Entity::GetTestParam_GDH() const
{
    return time( 0 );
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_MedicalPriorities
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
T_IdVector& Testable_Entity::GetTestParam_MedicalPriorities() const
{
    T_IdVector& priorities = *new T_IdVector();
    priorities.push_back( EnumHumanWound::blesse_urgence_extreme );
    priorities.push_back( EnumHumanWound::blesse_urgence_1 );
    priorities.push_back( EnumHumanWound::blesse_urgence_2 );
    priorities.push_back( EnumHumanWound::blesse_urgence_3 );

    return priorities;
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_DotationDType
// Created: NLD 2006-08-08
// -----------------------------------------------------------------------------
uint Testable_Entity::GetTestParam_DotationDType() const
{
    std::cerr << "[Testable_Entity::GetTestParam_DotationDType] Parameter type 'DotationDType' not set" << std::endl;
    return 0; //$$$$ TODO
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_Limas
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
T_IdVector& Testable_Entity::GetTestParam_Limas() const
{
    T_IdVector& limas = *new T_IdVector();
    for( uint i = 0; i < 10; ++i )
    {
        uint nId = workspace_.GetTacticalLineManager().GetLimaByType( ( ASN1T_EnumTypeLima )i );
        if( nId )
            limas.push_back( nId );
    }
    return limas;
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_LeftLimit
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
T_EntityId Testable_Entity::GetTestParam_LeftLimit()
{
    // if left limit is not set get the next limit
    if( nLeftLimit_ == 0 )
        nLeftLimit_ = workspace_.GetTacticalLineManager().GetNextLimitId();
    // at least world border limits should exist
    if( !nLeftLimit_ )
        throw std::runtime_error( "Unable to generate required limit parameter" );
    return nLeftLimit_;
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::GetTestParam_RightLimit
// Created: SBO 2005-08-16
// -----------------------------------------------------------------------------
T_EntityId Testable_Entity::GetTestParam_RightLimit()
{
    // if right limit is not set get the next limit
    if( nRightLimit_ == 0 )
        nRightLimit_ = workspace_.GetTacticalLineManager().GetNextLimitId();
    // at least world border limits should exist
    if( !nRightLimit_ )
        throw std::runtime_error( "Unable to generate required limit parameter" );
    return nRightLimit_;
}

// -----------------------------------------------------------------------------
// Name: Testable_Entity::MustRecomplete
// Created: SBO 2006-06-05
// -----------------------------------------------------------------------------
bool Testable_Entity::MustRecomplete( const Scheduler& scheduler )
{
    return scheduler.MustRecomplete( nLastRecompletion_ );
}
