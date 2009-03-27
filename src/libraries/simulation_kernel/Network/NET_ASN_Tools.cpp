//*****************************************************************************
//
// $Created: NLD 2003-02-28 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_ASN_Tools.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 18:08 $
// $Revision: 11 $
// $Workfile: NET_ASN_Tools.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "NET_ASN_Tools.h"

#include "Tools/MIL_Tools.h"
#include "MIL_AgentServer.h"
#include "simulation_terrain/TER_Localisation.h"
#include "MT_Tools/MT_Ellipse.h"

#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/MIL_Army.h"

#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/DEC_KnowledgeResolver_ABC.h"

#include "Decision/DEC_Gen_Object.h"
#include "Decision/DEC_Objective.h"
#include "Decision/DEC_Tools.h"

#include "Network/NET_AsnException.h"

#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;

typedef std::vector< DEC_Gen_Object* >    T_GenObjectVector;
typedef T_GenObjectVector::iterator       IT_GenObjectVector;
typedef T_GenObjectVector::const_iterator CIT_GenObjectVector;

typedef std::vector< DEC_Objective* >     T_ObjectiveVector;
typedef T_ObjectiveVector::iterator       IT_ObjectiveVector;
typedef T_ObjectiveVector::const_iterator CIT_ObjectiveVector;

//=============================================================================
// PARAMETERS TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadDirection
// Created: AGN 03-02-04
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::ReadDirection( const ASN1T_Heading& asnDir, MT_Vector2D& vDir )
{
    vDir = MT_Vector2D( 0, 1 ); // North vector

    MT_Float rRadAngle = asnDir * MT_PI / 180.;
    vDir.Rotate( rRadAngle );
    vDir.Normalize();
    assert( !vDir.IsZero() );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadLocation
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
// static 
bool NET_ASN_Tools::ReadLocation( const ASN1T_Location& asnLocalisation, TER_Localisation& localisation )
{
    //$$$$ ACCEDER DIRECTEMENT A LA LOCALISATION (EVITER COPIE)
    T_PointVector pointVector;
    pointVector.reserve( asnLocalisation.coordinates.n );
    for( uint i = 0; i < asnLocalisation.coordinates.n; ++i )
    {
        MT_Vector2D vPos;
        NET_ASN_Tools::ReadPoint( asnLocalisation.coordinates.elem[i], vPos );
        pointVector.push_back( vPos );
    }
    return localisation.Reset( (TER_Localisation::E_LocationType)asnLocalisation.type, pointVector );
}


//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadPoint
// Created: AGN 03-01-20
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::ReadPoint( const ASN1T_CoordLatLong& asnPoint, MT_Vector2D& vect )
{
    MIL_Tools::ConvertCoordMosToSim( asnPoint, vect );
}


//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadPoint
// Created: NLD 2003-03-26
//-----------------------------------------------------------------------------
// static
bool NET_ASN_Tools::ReadPoint( const ASN1T_Point& asnLocalisation, MT_Vector2D& vect )
{
    if( asnLocalisation.type != EnumLocationType::point )
        return false;
    if( asnLocalisation.coordinates.n != 1 )
        return false;

    ReadPoint( asnLocalisation.coordinates.elem[0], vect );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadLine
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadLine( const ASN1T_Line& asn, T_PointVector& points )
{
    if( asn.type != EnumLocationType::line || asn.coordinates.n < 2 )
        return false;

    points.clear();   
    points.reserve( asn.coordinates.n );
    for( uint i = 0; i < asn.coordinates.n; ++i )
    {
        MT_Vector2D vPosTmp;
        MIL_Tools::ConvertCoordMosToSim( asn.coordinates.elem[i], vPosTmp );
        points.push_back( vPosTmp );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadLine
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadLine( const ASN1T_Line& asn, TER_Localisation& localisation )
{
    if( asn.type != EnumLocationType::line || asn.coordinates.n < 2 )
        return false;
    return ReadLocation( asn, localisation );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadPath
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadPath( const ASN1T_Line& asn, T_PointVector& itineraire )
{
    if( asn.type != EnumLocationType::line || asn.coordinates.n < 1 )
        return false;

    itineraire.clear(); itineraire.reserve( asn.coordinates.n );
    for( uint i = 0; i < asn.coordinates.n; ++i )
    {
        MT_Vector2D vPos;
        ReadPoint( asn.coordinates.elem[i], vPos );
        itineraire.push_back( vPos );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadPolygon
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadPolygon( const ASN1T_Polygon&      asn, TER_Localisation& localisation )
{
    if( asn.type != EnumLocationType::polygon || asn.coordinates.n < 3 )
        return false;
    return ReadLocation( asn, localisation );
}


//---

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadLocationList
// Created: NLD 2003-08-11
// -----------------------------------------------------------------------------
// static
bool NET_ASN_Tools::ReadLocationList( const ASN1T_LocationList& asnLocationList, T_LocalisationPtrVector& localisationVector, uint nNbrEltMin )
{
    localisationVector.clear(); localisationVector.reserve( asnLocationList.n );
    for( uint i = 0; i < asnLocationList.n; ++i )
    {
        TER_Localisation* pLocalisation = new TER_Localisation(); //$$$ RAM
        if( !ReadLocation( asnLocationList.elem[i], *pLocalisation ) )
        {
            for( IT_LocalisationPtrVector itLocalisation = localisationVector.begin(); itLocalisation != localisationVector.end(); ++itLocalisation )
                delete *itLocalisation;
            localisationVector.clear();
            return false;
        }
        localisationVector.push_back( pLocalisation );
    }   
    return localisationVector.size() >= nNbrEltMin;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadListPolygon
// Created: NLD 2003-08-11
// -----------------------------------------------------------------------------
// static
bool NET_ASN_Tools::ReadPolygonList( const ASN1T_PolygonList& asn, T_LocalisationPtrVector& localisationVector, uint nNbrEltMin )
{
    for( uint i = 0; i < asn.n; ++i )
    {
        if( asn.elem[i].type != EnumLocationType::polygon || asn.elem[i].coordinates.n < 3 )
            return false;
    }
    
    localisationVector.clear(); localisationVector.reserve( asn.n );
    for( uint i = 0; i < asn.n; ++i )
    {
        TER_Localisation* pLocalisation = new TER_Localisation(); //$$$ RAM
        if( !ReadLocation( asn.elem[i], *pLocalisation ) )
        {
            for( IT_LocalisationPtrVector itLocalisation = localisationVector.begin(); itLocalisation != localisationVector.end(); ++itLocalisation )
                delete *itLocalisation;
            localisationVector.clear();
            return false;
        }
        localisationVector.push_back( pLocalisation );
    }   
    return localisationVector.size() >= nNbrEltMin;
}


//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadListPoint
// Created: AGN 03-01-20
//-----------------------------------------------------------------------------
// static
bool NET_ASN_Tools::ReadPointList( const ASN1T_PointList& asn, T_PointVector& pointVector )
{
    pointVector.clear(); pointVector.reserve( asn.n );
    for( uint i = 0; i < asn.n; ++i )
    {
        MT_Vector2D vPos;
        ReadPoint( asn.elem[i], vPos );
        pointVector.push_back( vPos );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadListItineraire
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadPathList( const ASN1T_PathList&   asn, T_ItinerairePtrVector& itineraireVector )
{
    itineraireVector.clear(); itineraireVector.reserve( asn.n );
    for( uint i = 0; i < asn.n; ++i )
    {
        T_PointVector* pItineraire = new T_PointVector(); //$$$ RAM
        if( ReadPath( asn.elem[i], *pItineraire ) )
            itineraireVector.push_back( pItineraire );
        else
        {
            MT_DELETEOWNED( itineraireVector );
            itineraireVector.clear();
            return false;
        }
    }
    return true;
}


// ---


//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadAgent
// Created: NLD 2003-03-24
//-----------------------------------------------------------------------------
// static
DEC_RolePion_Decision* NET_ASN_Tools::ReadAgent( const ASN1T_Unit& asnAgent )
{
    MIL_AgentPion* pPion = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( asnAgent );
    if( !pPion )
        return 0;
    return &pPion->GetDecision();
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadListAgent
// Created: AGN 03-01-20
//-----------------------------------------------------------------------------
// static
bool NET_ASN_Tools::ReadAgentList( const ASN1T_UnitList& asnListAgent, T_ObjectVector& unitList )
{
    unitList.clear(); unitList.reserve( asnListAgent.n );
    for( uint n = 0; n < asnListAgent.n; ++n )
    {
        DEC_RolePion_Decision* pAgent = ReadAgent( asnListAgent.elem[n] );
        if( !pAgent )
        {
            unitList.clear();
            return false;
        }
        unitList.push_back( pAgent );
    }
    return true;
}


//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadAutomate
// Created: NLD 2003-03-24
//-----------------------------------------------------------------------------
// static
DEC_AutomateDecision* NET_ASN_Tools::ReadAutomate( const ASN1T_Unit& asnAgent )
{
    MIL_Automate* pAutomate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( asnAgent );
    if( !pAutomate ) 
        return 0;
    return &pAutomate->GetDecision();
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadAutomatList
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadAutomateList( const ASN1T_AutomatList& asnListAgent, T_ObjectVector& unitList )
{
    unitList.clear(); unitList.reserve( asnListAgent.n );
    for( uint n = 0; n < asnListAgent.n; ++n )
    {
        DEC_AutomateDecision* pAgent = ReadAutomate( asnListAgent.elem[n] );
        if( !pAgent)
        {
            unitList.clear();
            return false;
        }
        unitList.push_back( pAgent );
    }
    return true;
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadKnowledgeAgent
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
// static $$NLDJVT$$ Interet de cette fonction
DEC_Knowledge_Agent* NET_ASN_Tools::ReadAgentKnowledge( const ASN1T_UnitKnowledge& asnAgent, const DEC_KnowledgeResolver_ABC& resolver )
{
    return resolver.ResolveKnowledgeAgent( asnAgent );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadListKnowledgeAgent
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadAgentKnowledgeList( const ASN1T_UnitKnowledgeList& asnListAgent, T_KnowledgeAgentDiaIDVector& knowledges, const DEC_KnowledgeResolver_ABC& resolver )
{
    knowledges.clear(); knowledges.reserve( asnListAgent.n );
    for( uint n = 0; n < asnListAgent.n; ++n )
    {        
        DEC_Knowledge_Agent* pKnowledge = ReadAgentKnowledge( asnListAgent.elem[n], resolver );
        if( pKnowledge == 0 )
            return false;
        knowledges.push_back( (void*)pKnowledge->GetID() );
    }
    return true;        
}


//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadObjectKnowledge
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
// static
DEC_Knowledge_Object* NET_ASN_Tools::ReadObjectKnowledge( const ASN1T_ObjectKnowledge& asnObject, const DEC_KnowledgeResolver_ABC& resolver )
{
    return resolver.ResolveKnowledgeObject( asnObject );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadListKnowledgeObject
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadObjectKnowledgeList( const ASN1T_ObjectKnowledgeList& asnListObject, T_KnowledgeObjectDiaIDVector& knowledges, const DEC_KnowledgeResolver_ABC& resolver )
{
    knowledges.clear(); knowledges.reserve( asnListObject.n );
    for( uint n = 0; n < asnListObject.n; ++n )
    {        
        DEC_Knowledge_Object* pKnowledge = ReadObjectKnowledge( asnListObject.elem[n], resolver );
        if( !pKnowledge )
            return false;
        knowledges.push_back( (void*)pKnowledge->GetID() );
    }
    return true;        
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadPopulationKnowledge
// Created: HME 05-12-22
//-----------------------------------------------------------------------------
DEC_Knowledge_Population* NET_ASN_Tools::ReadPopulationKnowledge( const ASN1T_PopulationKnowledge& asnPopulation, const DEC_KnowledgeResolver_ABC& resolver )
{
    return resolver.ResolveKnowledgePopulation( asnPopulation );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadGDH
// Created: NLD 2004-01-15
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ReadGDH( const ASN1T_DateTime& asn, uint& realTimeSec )
{
    bpt::ptime time = bpt::from_iso_string( std::string( (const char*)asn.data, 15 ) );
    realTimeSec = ( time - bpt::from_time_t( 0 ) ).total_seconds();
}

//=============================================================================
// ENCODING TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteDirection
// Created: NLD 2003-05-13
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WriteDirection( const MT_Vector2D& vDir, ASN1T_Heading& asnDir )
{
    static const MT_Vector2D vNorth( 0, 1 );
 
    assert( !vDir.IsZero() );

    MT_Float rAngle = Angle( vNorth, vDir ); // Trigo ...[-PI, PI]

    if( rAngle < 0 )
        rAngle += (2 * MT_PI); // Trigo ...[0, 2PI]

    rAngle = (2 * MT_PI) - rAngle; // Clockwise [0, 2PI]
    
    asnDir = (uint)( rAngle * 180. / MT_PI );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteLocation
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
void NET_ASN_Tools::WriteLocation( const TER_Localisation& localisation, ASN1T_Location& asnLocalisation )
{
    assert( localisation.GetType() != TER_Localisation::eNone );
    asnLocalisation.type            = (ASN1T_EnumLocationType)localisation.GetType();
    asnLocalisation.coordinates.n   = localisation.GetPoints().size(); 
    if( localisation.GetPoints().empty() )
        return;

    ASN1T_CoordLatLong* pCoord = new ASN1T_CoordLatLong[ localisation.GetPoints().size() ]; //$$$ RAM
    asnLocalisation.coordinates.elem = pCoord;
    uint i = 0;
    for( CIT_PointVector itPoint = localisation.GetPoints().begin(); itPoint != localisation.GetPoints().end(); ++itPoint )
        WritePoint( *itPoint, pCoord[i++] );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WritePoint
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WritePoint( const MT_Vector2D& vPos, ASN1T_CoordLatLong& asnCoordUTM )
{
    MIL_Tools::ConvertCoordSimToMos( vPos, asnCoordUTM );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WritePoint
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WritePoint( const MT_Vector2D& vPos, ASN1T_Point& asnLocalisation )
{
    ASN1T_CoordLatLong* pCoord = new ASN1T_CoordLatLong[1]; //$$$ RAM

    WritePoint( vPos, *pCoord );

    asnLocalisation.type = EnumLocationType::point;
    asnLocalisation.coordinates.n     = 1;
    asnLocalisation.coordinates.elem  = pCoord;
}


// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteLine
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteLine( const TER_Localisation& localisation, ASN1T_Line& asn )
{
    WriteLocation( localisation, asn );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteCoordinates
// Created: AGE 2007-07-06
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::WriteCoordinates( const T_PointVector& points , ASN1T_CoordLatLongList& asn )
{
    asn.n    = points.size();
    asn.elem = asn.n ? new ASN1T_CoordLatLong[ asn.n ] : 0;
    uint i = 0;
    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
        WritePoint( *it, asn.elem[i++] );
    return true;
}   

// -----------------------------------------------------------------------------
// Name NET_ASN_Tools::WriteLine
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::WriteLine( const T_PointVector& points, ASN1T_Line& asn )
{
    asn.type = EnumLocationType::line;
    if( points.size() < 2 )
    {
        asn.coordinates.n = 0;
        return false;
    }
    return WriteCoordinates( points, asn.coordinates );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WritePath
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WritePath( const T_PointVector& pointVector, ASN1T_Path& asn )
{
    asn.type          = EnumLocationType::line;
    asn.coordinates.n = pointVector.size(); 
    if( pointVector.empty() )
        return;
    WriteCoordinates( pointVector, asn.coordinates );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WritePath
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WritePath( const T_PointList& points, ASN1T_Path& asn )
{
    asn.type          = EnumLocationType::line;
    asn.coordinates.n = points.size(); 
    if( points.empty() )
        return;

    ASN1T_CoordLatLong* pCoord = new ASN1T_CoordLatLong[ points.size() ]; //$$$ RAM
    asn.coordinates.elem = pCoord;
    uint i = 0;
    for( CIT_PointList itPoint = points.begin(); itPoint != points.end(); ++itPoint )
        WritePoint( *itPoint, pCoord[i++] );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WritePolygon
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WritePolygon( const TER_Localisation& localisation, ASN1T_Line& asn )
{
    WriteLocation( localisation, asn );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteEllipse
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void NET_ASN_Tools::WriteEllipse( const MT_Ellipse& ellipse, ASN1T_Location& asnLocalisation )
{
    asnLocalisation.type = EnumLocationType::ellipse;
    asnLocalisation.coordinates.n = 3;
    asnLocalisation.coordinates.elem = new ASN1T_CoordLatLong[3];
    WritePoint( ellipse.GetCenter(), asnLocalisation.coordinates.elem[0] );
    WritePoint( ellipse.GetMajorAxisHighPoint(), asnLocalisation.coordinates.elem[1] );
    WritePoint( ellipse.GetMinorAxisHighPoint(), asnLocalisation.coordinates.elem[2] );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListPoint
// Created: AGN 03-01-20
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WritePointList( const T_PointVector& pointVector, ASN1T_PointList& asn )
{
    asn.n = pointVector.size();
    if( pointVector.empty() )
        return;

    ASN1T_Location* pCoord = new ASN1T_Location[ pointVector.size() ]; //$$$ RAM
    asn.elem = pCoord;

    uint i = 0;
    for( CIT_PointVector itPoint = pointVector.begin(); itPoint != pointVector.end(); ++itPoint )
        WritePoint( *itPoint, pCoord[i++] );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListPoint
// Created: AGN 03-01-20
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WritePointList( const T_PointList& points, ASN1T_PointList& asn )
{
    asn.n = points.size();
    if( points.empty() )
        return;

    ASN1T_Location* pCoord = new ASN1T_Location[ points.size() ]; //$$$ RAM
    asn.elem = pCoord;

    uint i = 0;
    for( CIT_PointList itPoint = points.begin(); itPoint != points.end(); ++itPoint )
        WritePoint( *itPoint, pCoord[i++] );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteLocationList
// Created: AGN 03-08-18
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WriteLocationList( const T_LocalisationPtrVector& locaList, ASN1T_LocationList& asnList )
{
    if( locaList.empty() )
    {
        asnList.n = 0;
        return;
    }

    ASN1T_Location* pLcal = new ASN1T_Location[ locaList.size() ];
    asnList.n = locaList.size();
    asnList.elem = pLcal;

    for( CIT_LocalisationPtrVector it = locaList.begin(); it != locaList.end(); ++it )
        WriteLocation( **it, pLcal[it - locaList.begin()] );
}


//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListPolygon
// Created: AGN 03-08-18
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WritePolygonList( const T_LocalisationPtrVector& locaList, ASN1T_PolygonList& asnList )
{
    if( locaList.empty() )
    {
        asnList.n = 0;
        return;
    }

    ASN1T_Location* pLcal = new ASN1T_Location[ locaList.size() ];
    asnList.n = locaList.size();
    asnList.elem = pLcal;

    for( CIT_LocalisationPtrVector it = locaList.begin(); it != locaList.end(); ++it )
        WriteLocation( **it, pLcal[it - locaList.begin()] );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListItineraire
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WritePathList( const T_ItinerairePtrVector& itineraireVector, ASN1T_PathList&   asn )
{
    if( itineraireVector.empty() )
    {
        asn.n = 0;
        return;
    }

    ASN1T_Location* pLcal = new ASN1T_Location[ itineraireVector.size() ];
    asn.n = itineraireVector.size();
    asn.elem = pLcal;

    for( CIT_ItinerairePtrVector it = itineraireVector.begin(); it != itineraireVector.end(); ++it )
        WritePath( **it, pLcal[it - itineraireVector.begin()] );    
}

// ---

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteAgent
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteAgent( const DEC_RolePion_Decision& pion, ASN1T_Unit& asn )
{
    asn = pion.GetPion().GetID();
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteAutomate
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteAutomate( const DEC_AutomateDecision& automate, ASN1T_Unit& asn )
{
    asn = automate.GetAutomate().GetID();    
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListAgent
// Created: AGN 03-01-20
//-----------------------------------------------------------------------------
void NET_ASN_Tools::WriteAgentList( const T_ObjectVector& unitList, ASN1T_UnitList& asnListAgent )
{
    if( unitList.empty() )
    {
        asnListAgent.n = 0;
        return;
    }
    
    ASN1T_OID* pOID = new ASN1T_OID[ unitList.size() ]; //$$$ RAM
    asnListAgent.n    = unitList.size();
    asnListAgent.elem = pOID;

    uint i = 0;
    for( CIT_ObjectVector itUnit = unitList.begin(); itUnit != unitList.end(); ++itUnit )
        pOID[i++] = static_cast< DEC_RolePion_Decision* >( *itUnit )->GetPion().GetID();
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteAutomatList
// Created: AGN 03-01-20
//-----------------------------------------------------------------------------
void NET_ASN_Tools::WriteAutomateList( const T_ObjectVector& unitList, ASN1T_AutomatList& asnListAgent )
{
    if( unitList.empty() )
    {
        asnListAgent.n = 0;
        return;
    }
    
    ASN1T_OID* pOID = new ASN1T_OID[ unitList.size() ]; //$$$ RAM
    asnListAgent.n    = unitList.size();
    asnListAgent.elem = pOID;

    uint i = 0;
    for( CIT_ObjectVector itUnit = unitList.begin(); itUnit != unitList.end(); ++itUnit )
        pOID[i++] = static_cast< DEC_AutomateDecision* >( *itUnit )->GetAutomate().GetID();
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteAgentKnowledge( const DEC_Knowledge_Agent& knowledge, ASN1T_UnitKnowledge& asnKnowledge )
{
    asnKnowledge = knowledge.GetID();
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteAgentKnowledgeList( const T_KnowledgeAgentDiaIDVector& knowledges, ASN1T_UnitKnowledgeList& asnListKnowledge, const DEC_KnowledgeResolver_ABC& resolver )
{
    asnListKnowledge.n = knowledges.size();
    if( knowledges.empty() )
        return;
    
    asnListKnowledge.elem = new ASN1T_OID[ knowledges.size() ];
    uint i = 0;
    for( CIT_KnowledgeAgentDiaIDVector itKnowledge = knowledges.begin(); itKnowledge != knowledges.end(); ++itKnowledge )    
    {
        const DEC_Knowledge_Agent* pKnowledge = resolver.ResolveKnowledgeAgent( (uint)*itKnowledge );
        if( pKnowledge )
            asnListKnowledge.elem[i] = pKnowledge->GetID();
        else
            asnListKnowledge.elem[i] = 0;       
        ++i;
    }
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteObjectKnowledge
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteObjectKnowledge( const DEC_Knowledge_Object& knowledge, ASN1T_ObjectKnowledge& asnKnowledge )
{
    asnKnowledge = knowledge.GetID();
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListKnowledgeObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteObjectKnowledgeList( const T_KnowledgeObjectDiaIDVector& knowledges, ASN1T_ObjectKnowledgeList& asnListKnowledge, const DEC_KnowledgeResolver_ABC& resolver )
{
    asnListKnowledge.n = knowledges.size();
    if( knowledges.empty() )
        return;
    
    asnListKnowledge.elem = new ASN1T_OID[ knowledges.size() ];
    uint i = 0;
    for( CIT_KnowledgeObjectDiaIDVector itKnowledge = knowledges.begin(); itKnowledge != knowledges.end(); ++itKnowledge )    
    {
        const DEC_Knowledge_Object* pKnowledge = resolver.ResolveKnowledgeObject( (uint)*itKnowledge );
        if( pKnowledge )
            asnListKnowledge.elem[i] = pKnowledge->GetID();
        else
            asnListKnowledge.elem[i] = 0;       
        ++i;
    }
}

//=============================================================================
// MISC TOOLS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_MissionObjectiveList& asn )
{
    if( asn.n > 0 )
    {
        for( uint i = 0; i < asn.n; ++i )
            Delete( asn.elem[i] );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_MissionObjective& asn )
{
    NET_ASN_Tools::Delete( asn.localisation );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_PlannedWorkList& asn )
{
    if( asn.n > 0 )
    {
        for( uint i = 0; i < asn.n; ++i )
            Delete( asn.elem[i] );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_PlannedWork& asn )
{
    NET_ASN_Tools::Delete( asn.position );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_LogMedicalPriorities& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_LogMaintenancePriorities& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_UnitList& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_AutomatList& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: AGE 2007-07-06
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_CoordLatLongList& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_Polygon& asn )
{
    if( asn.coordinates.n > 0 )
        delete [] asn.coordinates.elem;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_PolygonList& asn )
{
    if( asn.n > 0 )
    {
        for( uint i = 0; i < asn.n; ++i )
            Delete( asn.elem[ i ] );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_PointList& asn )
{
    if( asn.n > 0 )
    {
        for( uint i = 0; i < asn.n; ++i )
            Delete( asn.elem[ i ] );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_PathList& asn )
{
    if( asn.n > 0 )
    {
        for( uint i = 0; i < asn.n; ++i )
            Delete( asn.elem[ i ] );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_LocationList& asn )
{
    if( asn.n > 0 )
    {
        for( uint i = 0; i < asn.n; ++i )
            Delete( asn.elem[i] );
        delete [] asn.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_UnitKnowledgeList& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_ObjectKnowledgeList& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// =============================================================================
// MISSION PARAMETERS : KNOWLEDGE OBJECT DIA - SIM - ASN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyObjectKnowledge
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyObjectKnowledge( const ASN1T_ObjectKnowledge& asn, DIA_Variable_ABC& dia, const DEC_KnowledgeResolver_ABC& resolver )
{
    DEC_Knowledge_Object* pKnowledge = NET_ASN_Tools::ReadObjectKnowledge( asn, resolver );
    if( !pKnowledge ) 
        return false;

    dia.SetValue( (void*)pKnowledge->GetID(), &DEC_Tools::GetTypeConnaissanceObjet() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyObjectKnowledge
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyObjectKnowledge( const DIA_Variable_ABC& dia, ASN1T_ObjectKnowledge& asn, const DEC_KnowledgeResolver_ABC& resolver )
{
    assert( DEC_Tools::CheckTypeConnaissanceObjet( dia ) );

    DEC_Knowledge_Object* pKnowledge = resolver.ResolveKnowledgeObject( dia );
    if( !pKnowledge )
        return false;
    
    NET_ASN_Tools::WriteObjectKnowledge( *pKnowledge, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyObjectKnowledge
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyObjectKnowledge( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeConnaissanceObjet( diaFrom ) );
    assert( DEC_Tools::CheckTypeConnaissanceObjet( diaTo ) );

    assert( &diaFrom != &diaTo );
    diaTo.SetValue( diaFrom.ToPtr(), &DEC_Tools::GetTypeConnaissanceObjet() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListKnowledgeObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyObjectKnowledgeList( const ASN1T_ObjectKnowledgeList& asn, DIA_Variable_ABC& dia, const DEC_KnowledgeResolver_ABC& resolver )
{
    T_KnowledgeObjectDiaIDVector knowledges;
    
    if( !NET_ASN_Tools::ReadObjectKnowledgeList( asn, knowledges, resolver ) )
        return false;

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceObjet() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListKnowledgeObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyObjectKnowledgeList( const DIA_Variable_ABC& dia, ASN1T_ObjectKnowledgeList& asn, const DEC_KnowledgeResolver_ABC& resolver )
{
    assert( DEC_Tools::CheckTypeListeConnaissancesObjet( dia ) );

    const DIA_Variable_ObjectList& diaObjectList = static_cast< const DIA_Variable_ObjectList& >( dia );

    T_KnowledgeObjectDiaIDVector knowledges = diaObjectList.ToUserTypeList( knowledges );
    NET_ASN_Tools::WriteObjectKnowledgeList( knowledges, asn, resolver );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListKnowledgeObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyObjectKnowledgeList( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeListeConnaissancesObjet( diaFrom ) );
    assert( DEC_Tools::CheckTypeListeConnaissancesObjet( diaTo   ) );

    const DIA_Variable_ObjectList& diaListFrom = static_cast< const DIA_Variable_ObjectList& >( diaFrom );
          DIA_Variable_ObjectList& diaListTo   = static_cast<       DIA_Variable_ObjectList& >( diaTo );

    T_KnowledgeObjectDiaIDVector knowledges = diaListFrom.ToUserTypeList( knowledges );
    diaListTo.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceObjet() );
}

// =============================================================================
// MISSION PARAMETERS : KNOWLEDGE AGENT DIA - SIM - ASN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgentKnowledge( const ASN1T_UnitKnowledge& asn, DIA_Variable_ABC& dia, const DEC_KnowledgeResolver_ABC& resolver )
{
    DEC_Knowledge_Agent* pKnowledge = NET_ASN_Tools::ReadAgentKnowledge( asn, resolver );
    if( !pKnowledge ) 
        return false;

    dia.SetValue( (void*)pKnowledge->GetID(), &DEC_Tools::GetTypeConnaissanceAgent() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgentKnowledge( const DIA_Variable_ABC& dia, ASN1T_UnitKnowledge& asn, const DEC_KnowledgeResolver_ABC& resolver )
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( dia ) );

    DEC_Knowledge_Agent* pKnowledge = resolver.ResolveKnowledgeAgent( dia );
    if( !pKnowledge )
        return false;
    
    asn = pKnowledge->GetID();
    return true;    
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyAgentKnowledge( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( diaFrom ) );
    assert( DEC_Tools::CheckTypeConnaissanceAgent( diaTo ) );
    assert( &diaFrom != &diaTo );
    diaTo.SetValue( diaFrom.ToPtr(), &DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgentKnowledgeList( const ASN1T_UnitKnowledgeList& asn, DIA_Variable_ABC& dia, const DEC_KnowledgeResolver_ABC& resolver )
{
    T_KnowledgeAgentDiaIDVector knowledges;
    
    if( !NET_ASN_Tools::ReadAgentKnowledgeList( asn, knowledges, resolver ) )
        return false;

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgentKnowledgeList( const DIA_Variable_ABC& dia, ASN1T_UnitKnowledgeList& asn, const DEC_KnowledgeResolver_ABC& resolver )
{
    assert( DEC_Tools::CheckTypeListeConnaissancesAgent( dia ) );

    const DIA_Variable_ObjectList& diaObjectList = static_cast< const DIA_Variable_ObjectList& >( dia );

    T_KnowledgeAgentDiaIDVector knowledges = diaObjectList.ToUserTypeList( knowledges );
    NET_ASN_Tools::WriteAgentKnowledgeList( knowledges, asn, resolver );
    return true;

}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyAgentKnowledgeList( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeListeConnaissancesAgent( diaFrom ) );
    assert( DEC_Tools::CheckTypeListeConnaissancesAgent( diaTo ) );

    const DIA_Variable_ObjectList& diaListFrom = static_cast< const DIA_Variable_ObjectList& >( diaFrom );
          DIA_Variable_ObjectList& diaListTo   = static_cast<       DIA_Variable_ObjectList& >( diaTo );

    T_KnowledgeAgentDiaIDVector knowledges = diaListFrom.ToUserTypeList( knowledges );
    diaListTo.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// =============================================================================
// MISSION PARAMETERS TOOLS : AGENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyAgent
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgent( const ASN1T_Unit& asn, DIA_Variable_ABC& dia )
{
    DEC_RolePion_Decision* pAgent = NET_ASN_Tools::ReadAgent( asn );
    dia.SetValue( *pAgent );
    return pAgent != 0;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyAgent
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgent( const DIA_Variable_ABC& dia, ASN1T_Unit& asn )
{
    assert( DEC_Tools::CheckTypePion( dia ) );

    DEC_RolePion_Decision* pAgent = dia.ToUserObject( pAgent );
    if( !pAgent )
    {
        asn = 0;
        return false;
    }
    else
    {
        NET_ASN_Tools::WriteAgent( *pAgent, asn );
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyAgent
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgent( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypePion( diaFrom ) );
    assert( DEC_Tools::CheckTypePion( diaTo ) );

    diaTo.SetValue( *diaFrom.ToObject() );
    return true;
}


// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyAutomate
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAutomate( const ASN1T_Unit& asn, DIA_Variable_ABC& dia )
{
    DEC_AutomateDecision* pAgent = NET_ASN_Tools::ReadAutomate( asn );
    dia.SetValue( *pAgent );
    return pAgent != 0;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyAutomate
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAutomate( const DIA_Variable_ABC& dia, ASN1T_Unit& asn )
{
    assert( DEC_Tools::CheckTypeAutomate( dia ) );

    DEC_AutomateDecision* pAutomate = dia.ToUserObject( pAutomate );
    if( !pAutomate )
    {
        asn = 0;
        return false;
    }
    else
    {
        NET_ASN_Tools::WriteAutomate( *pAutomate, asn );
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyAutomate
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAutomate( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeAutomate( diaFrom ) );
    assert( DEC_Tools::CheckTypeAutomate( diaTo   ) );

    diaTo.SetValue( *diaFrom.ToObject() );
    return true;
}

   
// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListAgent
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgentList( const ASN1T_UnitList& asn, DIA_Variable_ABC& dia )
{
    T_ObjectVector agents;
    if( !NET_ASN_Tools::ReadAgentList( asn, agents ) )
        return false;
    dia.SetValue( agents );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListAgent
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgentList( const DIA_Variable_ABC& dia, ASN1T_UnitList& asn )
{
    assert( DEC_Tools::CheckTypeListePions( dia ) );

    NET_ASN_Tools::WriteAgentList( dia.ToSelection(), asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListAgent
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgentList( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeListePions( diaFrom ) );
    assert( DEC_Tools::CheckTypeListePions( diaTo   ) );
    diaTo.SetValue( diaFrom.ToSelection() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyAutomatList
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAutomateList( const ASN1T_AutomatList& asn, DIA_Variable_ABC& dia )
{
    T_ObjectVector automates;
    if( !NET_ASN_Tools::ReadAutomateList( asn, automates ) )
        return false;
    dia.SetValue( automates );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyAutomatList
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAutomateList( const DIA_Variable_ABC& dia, ASN1T_AutomatList& asn )
{
    assert( DEC_Tools::CheckTypeListeAutomates( dia ) );
    NET_ASN_Tools::WriteAutomateList( dia.ToSelection(), asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyAutomatList
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAutomateList( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeListeAutomates( diaFrom ) );
    assert( DEC_Tools::CheckTypeListeAutomates( diaTo ) );
    diaTo.SetValue( diaFrom.ToSelection() );
    return true;
}

// =============================================================================
// Mission parameters tools : LOG specific
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyMaintenancePriorities
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyMaintenancePriorities( const ASN1T_LogMaintenancePriorities& asn, DIA_Variable_ABC& dia )
{
    T_MaintenancePriorityVector* pData = new T_MaintenancePriorityVector();
    pData->reserve( asn.n );
    for( uint i = 0; i < asn.n; ++i )
    {
        uint nCompTypeMosID = asn.elem[i];
        const PHY_ComposanteTypePion* pType = PHY_ComposanteTypePion::Find( nCompTypeMosID );
        if( !pType )
        {
            dia.SetValue( 0, &DEC_Tools::GetTypeMaintenancePriorities() );
            delete pData;
            return false;
        }
        pData->push_back( pType );
    }

    dia.SetValue( pData, &DEC_Tools::GetTypeMaintenancePriorities());
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyMaintenancePriorities
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyMaintenancePriorities( const DIA_Variable_ABC& dia, ASN1T_LogMaintenancePriorities& asn )
{
    assert( DEC_Tools::CheckTypeMaintenancePriorities( dia ) );

    T_MaintenancePriorityVector* pData = dia.ToUserPtr( pData );
    if( !pData )
        asn.n = 0;
    else
    {
        asn.n    = pData->size();
        asn.elem = new ASN1T_EquipmentType[ pData->size() ];
        uint i = 0;
        for( CIT_MaintenancePriorityVector it = pData->begin(); it != pData->end(); ++it )
            asn.elem[ i++ ] = (**it).GetMosID();
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyMaintenancePriorities
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyMaintenancePriorities( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeMaintenancePriorities( diaFrom ) );
    const T_MaintenancePriorityVector* pSrc = diaFrom.ToUserPtr( pSrc );
    assert( pSrc );

    T_MaintenancePriorityVector* pDst = new T_MaintenancePriorityVector( *pSrc );
    diaTo.SetValue( pDst, &DEC_Tools::GetTypeMaintenancePriorities() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyMedicalPriorities
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyMedicalPriorities( const ASN1T_LogMedicalPriorities& asn, DIA_Variable_ABC& dia )
{
    T_MedicalPriorityVector* pData = new T_MedicalPriorityVector();
    pData->reserve( asn.n );
    for( uint i = 0; i < asn.n; ++i )
    {
        ASN1T_EnumHumanWound nWoundID = asn.elem[i];
        const PHY_HumanWound* pWound = PHY_HumanWound::Find( nWoundID );
        if( !pWound )
        {
            dia.SetValue( 0, &DEC_Tools::GetTypeSantePriorites() );
            delete pData;
            return false;
        }
        pData->push_back( pWound );
    }
    dia.SetValue( pData, &DEC_Tools::GetTypeSantePriorites() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyMedicalPriorities
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyMedicalPriorities( const DIA_Variable_ABC& dia, ASN1T_LogMedicalPriorities& asn )
{
    assert( DEC_Tools::CheckTypeSantePriorites( dia ) );

    T_MedicalPriorityVector* pData = dia.ToUserPtr( pData );
    if( !pData || pData->empty() )
        asn.n = 0;
    else
    {
        asn.n = pData->size();
        asn.elem = new ASN1T_EnumHumanWound[ pData->size() ];
        uint i = 0;
        for( CIT_MedicalPriorityVector it = pData->begin(); it != pData->end(); ++it )
            asn.elem[ i++ ] = (**it).GetAsnID();
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyMedicalPriorities
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyMedicalPriorities( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeSantePriorites( diaFrom ) );
    const T_MedicalPriorityVector* pSrc = diaFrom.ToUserPtr( pSrc );
    assert( pSrc );

    T_MedicalPriorityVector* pDest = new T_MedicalPriorityVector( *pSrc );
    diaTo.SetValue( pDest, &DEC_Tools::GetTypeSantePriorites() );
    return true;
}

// =============================================================================
// Mission parameters tools : GEN specific
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyGenObject
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyGenObject( const ASN1T_PlannedWork& asn, DIA_Variable_ABC& dia )
{
    DEC_Gen_Object* pGenObject = new DEC_Gen_Object( asn );
    dia.SetValue( pGenObject, &DEC_Tools::GetTypeGenObjet() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyGenObject
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyGenObject( const DIA_Variable_ABC& dia, ASN1T_PlannedWork& asn )
{
    assert( DEC_Tools::CheckTypeGenObjet( dia ) );
    const DEC_Gen_Object* pTmp = dia.ToUserPtr( pTmp );
    if( !pTmp )
    {
        assert( false );
        return false;
    }
    pTmp->Serialize( asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyGenObject
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyGenObject( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeGenObjet( diaFrom ) );
    assert( DEC_Tools::CheckTypeGenObjet( diaTo ) );

    const DEC_Gen_Object* pSrc = diaFrom.ToUserPtr( pSrc );
    assert( pSrc );
    DEC_Gen_Object* pDest = new DEC_Gen_Object( *pSrc );
    diaTo.SetValue( pDest, &DEC_Tools::GetTypeGenObjet() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListGenObjects
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyGenObjectList( const ASN1T_PlannedWorkList& asn, DIA_Variable_ABC& dia )
{
    T_GenObjectVector objects;
    objects.reserve( asn.n );
    for( uint i = 0; i < asn.n; ++i )
    {
        try
        {
            DEC_Gen_Object* pGenObject = new DEC_Gen_Object( asn.elem[i] );
            objects.push_back( pGenObject );
        }
        catch( NET_AsnException< ASN1T_EnumOrderErrorCode >& )
        {
            for( IT_GenObjectVector it = objects.begin(); it != objects.end(); ++it )
                delete *it;
            throw;
        }
    }
    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    diaObjectList.SetValueUserType( objects, DEC_Tools::GetTypeGenObjet() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListGenObjects
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyGenObjectList( const DIA_Variable_ABC& dia, ASN1T_PlannedWorkList& asn )
{
    assert( DEC_Tools::CheckTypeListeGenObjets( dia ) );

    const DIA_Variable_ObjectList& diaObjectList = static_cast< const DIA_Variable_ObjectList& >( dia );

    T_GenObjectVector objects = diaObjectList.ToUserTypeList( objects );

    asn.n = objects.size();
    if( !objects.empty() )
    {
        asn.elem = new ASN1T_PlannedWork[ objects.size() ];
        for( CIT_GenObjectVector it = objects.begin(); it != objects.end(); ++it )
            (**it).Serialize( asn.elem[ it - objects.begin() ] );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyGenObjectList
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyGenObjectList( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{                       
    assert( DEC_Tools::CheckTypeListeGenObjets( diaFrom ) );
    assert( DEC_Tools::CheckTypeListeGenObjets( diaTo ) );

    const DIA_Variable_ObjectList& diaListFrom = static_cast< const DIA_Variable_ObjectList& >( diaFrom );
          DIA_Variable_ObjectList& diaListTo   = static_cast<       DIA_Variable_ObjectList& >( diaTo );

    T_GenObjectVector objectsFrom = diaListFrom.ToUserTypeList( objectsFrom );
    T_GenObjectVector objectsTo;

    objectsTo.reserve( objectsFrom.size() );
    for( CIT_GenObjectVector it = objectsFrom.begin(); it != objectsFrom.end(); ++it )
    {
        DEC_Gen_Object* pNewObject = new DEC_Gen_Object( **it );
        objectsTo.push_back( pNewObject );
    }
    diaListTo.SetValueUserType( objectsTo, DEC_Tools::GetTypeGenObjet() );
}

// =============================================================================
// Mission parameters tools : Objective
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyObjective
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyObjective( const ASN1T_MissionObjective& asn, DIA_Variable_ABC& dia )
{
    DEC_Objective* pObjective = new DEC_Objective( asn );
    dia.SetValue( pObjective, &DEC_Tools::GetTypeObjectif() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyObjective
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyObjective( const DIA_Variable_ABC& dia, ASN1T_MissionObjective& asn )
{
    assert( DEC_Tools::CheckTypeObjectif( dia ) );
    const DEC_Objective* pTmp = dia.ToUserPtr( pTmp );
    if( !pTmp )
    {
        assert( false );
        return false;
    }
    pTmp->Serialize( asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyObjective
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyObjective( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeObjectif( diaFrom ) );
    assert( DEC_Tools::CheckTypeObjectif( diaTo ) );

    const DEC_Objective* pSrc = diaFrom.ToUserPtr( pSrc );
    assert( pSrc );
    DEC_Objective* pDest = new DEC_Objective( *pSrc );
    diaTo.SetValue( pDest, &DEC_Tools::GetTypeObjectif() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListObjectives
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyObjectiveList( const ASN1T_MissionObjectiveList& asn, DIA_Variable_ABC& dia )
{
    T_ObjectiveVector objects;
    objects.reserve( asn.n );
    for( uint i = 0; i < asn.n; ++i )
    {
        try
        {
            DEC_Objective* pObjective = new DEC_Objective( asn.elem[i] );
            objects.push_back( pObjective );
        }
        catch( NET_AsnException< ASN1T_EnumOrderErrorCode >& )
        {
             for( IT_ObjectiveVector it = objects.begin(); it != objects.end(); ++it )
                delete *it;
            throw;
        }
    }
    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    diaObjectList.SetValueUserType( objects, DEC_Tools::GetTypeObjectif() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListObjectives
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyObjectiveList( const DIA_Variable_ABC& dia, ASN1T_MissionObjectiveList& asn )
{
    assert( DEC_Tools::CheckTypeListeObjectifs( dia ) );

    const DIA_Variable_ObjectList& diaObjectList = static_cast< const DIA_Variable_ObjectList& >( dia );

    T_ObjectiveVector objects = diaObjectList.ToUserTypeList( objects );

    asn.n = objects.size();
    if( !objects.empty() )
    {
        asn.elem = new ASN1T_MissionObjective[ objects.size() ];
        for( CIT_ObjectiveVector it = objects.begin(); it != objects.end(); ++it )
            (**it).Serialize( asn.elem[ it - objects.begin() ] );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyObjectiveList
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyObjectiveList( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{                       
    assert( DEC_Tools::CheckTypeListeObjectifs( diaFrom ) );
    assert( DEC_Tools::CheckTypeListeObjectifs( diaTo ) );

    const DIA_Variable_ObjectList& diaListFrom = static_cast< const DIA_Variable_ObjectList& >( diaFrom );
          DIA_Variable_ObjectList& diaListTo   = static_cast<       DIA_Variable_ObjectList& >( diaTo );

    T_ObjectiveVector objectsFrom = diaListFrom.ToUserTypeList( objectsFrom );
    T_ObjectiveVector objectsTo;

    objectsTo.reserve( objectsFrom.size() );
    for( CIT_ObjectiveVector it = objectsFrom.begin(); it != objectsFrom.end(); ++it )
    {
        DEC_Objective* pNewObject = new DEC_Objective( **it );
        objectsTo.push_back( pNewObject );
    }
    diaListTo.SetValueUserType( objectsTo, DEC_Tools::GetTypeObjectif() );
}

// =============================================================================
// Mission parameters tools : Localisation/Geometry DIA - SIM - ASN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyLocation
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyLocation( const ASN1T_Location& asn, DIA_Variable_ABC& dia )
{
    TER_Localisation* pLoc = new TER_Localisation();

    if( !NET_ASN_Tools::ReadLocation( asn, *pLoc ) )
    {
        delete pLoc;
        return false;
    }

    dia.SetValue( pLoc, &DEC_Tools::GetTypeLocalisation() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyLocation
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyLocation( const DIA_Variable_ABC& dia, ASN1T_Location& asn )
{
    assert( DEC_Tools::CheckTypeLocalisation( dia ) );
    TER_Localisation* pLocalisation = dia.ToUserPtr( pLocalisation );
    if( !pLocalisation )
    {
        assert( false );
        return false;
    }
    NET_ASN_Tools::WriteLocation( *pLocalisation, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyLocation
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyLocation( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeLocalisation( diaFrom ) );
    assert( DEC_Tools::CheckTypeLocalisation( diaTo ) );
    const TER_Localisation* pSrc = diaFrom.ToUserPtr( pSrc );
    assert( pSrc );

    TER_Localisation* pDest = new TER_Localisation( *pSrc );

    diaTo.SetValue( pDest, &DEC_Tools::GetTypeLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyLocationList
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyLocationList( const ASN1T_LocationList& asn, DIA_Variable_ABC& dia )
{
    T_LocalisationPtrVector localisations;

    if( !NET_ASN_Tools::ReadLocationList( asn, localisations ) )
        return false;

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    diaObjectList.SetValueUserType( localisations, DEC_Tools::GetTypeLocalisation() );
    return true;    
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyLocationList
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyLocationList( const DIA_Variable_ABC& dia, ASN1T_LocationList& asn )
{
    assert( DEC_Tools::CheckTypeListeLocalisations( dia ) );
    const DIA_Variable_ObjectList& diaObjectList = static_cast< const DIA_Variable_ObjectList& >( dia );

    T_LocalisationPtrVector localisations = diaObjectList.ToUserTypeList( localisations );
    NET_ASN_Tools::WriteLocationList( localisations, asn );
    return true;    
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyLocationList
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyLocationList( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeListeLocalisations( diaFrom ) );
    assert( DEC_Tools::CheckTypeListeLocalisations( diaTo ) );

    const DIA_Variable_ObjectList& diaListFrom = static_cast< const DIA_Variable_ObjectList& >( diaFrom );
          DIA_Variable_ObjectList& diaListTo   = static_cast<       DIA_Variable_ObjectList& >( diaTo );

    T_LocalisationPtrVector localisationsFrom = diaListFrom.ToUserTypeList( localisationsFrom );
    T_LocalisationPtrVector localisationsTo;
    localisationsTo.reserve( localisationsFrom.size() );
    for( CIT_LocalisationPtrVector itLoc = localisationsFrom.begin(); itLoc != localisationsFrom.end(); ++itLoc )
    {
        TER_Localisation* pNewLoc = new TER_Localisation(); //$$$$ RAM
        pNewLoc->Reset( **itLoc );
        localisationsTo.push_back( pNewLoc );
    }
    diaListTo.SetValueUserType( localisationsTo, DEC_Tools::GetTypeLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPolygon
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPolygon( const ASN1T_Polygon& asn, DIA_Variable_ABC& dia )
{
    TER_Localisation* pLoc = new TER_Localisation();
    if( !NET_ASN_Tools::ReadPolygon( asn, *pLoc ) )
    {
        delete pLoc;
        return false;
    }
    
    dia.SetValue( pLoc, &DEC_Tools::GetTypeLocalisation() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPolygon
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPolygon( const DIA_Variable_ABC& dia, ASN1T_Polygon& asn )
{
    assert( DEC_Tools::CheckTypeLocalisation( dia ) );

    TER_Localisation* pPolygon = dia.ToUserPtr( pPolygon );
    if( !pPolygon )
    {
        assert( false );
        return false;
    }
    NET_ASN_Tools::WritePolygon( *pPolygon, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPolygon
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyPolygon( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeLocalisation( diaFrom ) );
    assert( DEC_Tools::CheckTypeLocalisation( diaTo ) );

    const TER_Localisation* pSrc = diaFrom.ToUserPtr( pSrc );
    assert( pSrc );

    TER_Localisation* pDest = new TER_Localisation( *pSrc );
    diaTo.SetValue( pDest, &DEC_Tools::GetTypeLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListPolygon
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPolygonList( const ASN1T_PolygonList& asn, DIA_Variable_ABC& dia )
{
    T_LocalisationPtrVector polygons;

    if( !NET_ASN_Tools::ReadPolygonList( asn, polygons ) )
        return false;

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    diaObjectList.SetValueUserType( polygons, DEC_Tools::GetTypeLocalisation() );
    return true;    
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListPolygon
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPolygonList( const DIA_Variable_ABC& dia, ASN1T_PolygonList& asn )
{
    assert( DEC_Tools::CheckTypeListeLocalisations( dia ) );

    const DIA_Variable_ObjectList& diaObjectList = static_cast< const DIA_Variable_ObjectList& >( dia );

    T_LocalisationPtrVector Polygons = diaObjectList.ToUserTypeList( Polygons );
    NET_ASN_Tools::WritePolygonList( Polygons, asn );
    return true;    
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListPolygon
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyPolygonList( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeListeLocalisations( diaFrom ) );
    assert( DEC_Tools::CheckTypeListeLocalisations( diaTo ) );

    const DIA_Variable_ObjectList& diaListFrom = static_cast< const DIA_Variable_ObjectList& >( diaFrom );
          DIA_Variable_ObjectList& diaListTo   = static_cast<       DIA_Variable_ObjectList& >( diaTo );

    T_LocalisationPtrVector PolygonsFrom = diaListFrom.ToUserTypeList( PolygonsFrom );
    T_LocalisationPtrVector PolygonsTo;
    PolygonsTo.reserve( PolygonsFrom.size() );
    for( CIT_LocalisationPtrVector itLoc = PolygonsFrom.begin(); itLoc != PolygonsFrom.end(); ++itLoc )
    {
        TER_Localisation* pNewLoc = new TER_Localisation(); //$$$$ RAM
        pNewLoc->Reset( **itLoc );
        PolygonsTo.push_back( pNewLoc );
    }
    diaListTo.SetValueUserType( PolygonsTo, DEC_Tools::GetTypeLocalisation() );
}


// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPoint
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPoint( const ASN1T_Point& asn, DIA_Variable_ABC& dia, bool bNullValue /*= false*/ )
{
    if( bNullValue )
    {
        dia.SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return true;
    }
    
    MT_Vector2D* pPt = new MT_Vector2D();
    if( !NET_ASN_Tools::ReadPoint( asn, *pPt ) )
    {
        delete pPt;
        return false;
    }

    dia.SetValue( pPt, &DEC_Tools::GetTypePoint() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPoint
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPoint( const DIA_Variable_ABC& dia, ASN1T_Point& asn )
{
    assert( DEC_Tools::CheckTypePoint( dia ) );

    MT_Vector2D* pPoint = dia.ToUserPtr( pPoint );
    if( !pPoint ) // Optional
        return false;

    NET_ASN_Tools::WritePoint( *pPoint, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPoint
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPoint( const DIA_Variable_ABC& dia, ASN1T_CoordLatLong& asn )
{
    assert( DEC_Tools::CheckTypePoint( dia ) );

    MT_Vector2D* pPoint = dia.ToUserPtr( pPoint );
    if( !pPoint ) // Optional
        return false;

    NET_ASN_Tools::WritePoint( *pPoint, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPoint
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPoint( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypePoint( diaFrom ) );
    assert( DEC_Tools::CheckTypePoint( diaTo ) );
    const MT_Vector2D* pSrc = diaFrom.ToUserPtr( pSrc );
    if( !pSrc )   
    {
        diaTo.SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return false;
    }

    MT_Vector2D* pDest = new MT_Vector2D( *pSrc );
    diaTo.SetValue( pDest, &DEC_Tools::GetTypePoint() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListPoint
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPointList( const ASN1T_PointList& asn, DIA_Variable_ABC& dia )
{
    T_PointVector* pPtVec = new T_PointVector();
    if( !NET_ASN_Tools::ReadPointList( asn, *pPtVec ) )
    {
        dia.SetValue( (void*)0, &DEC_Tools::GetTypeListePoints() );
        delete pPtVec;
        return false;
    }

    dia.SetValue( pPtVec, &DEC_Tools::GetTypeListePoints() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListPoint
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPointList( const DIA_Variable_ABC& dia, ASN1T_PointList& asn )
{
    assert( DEC_Tools::CheckTypeListePoints( dia ) );
    T_PointVector* pPoints = dia.ToUserPtr( pPoints );
    if( !pPoints )
    {
        assert( false );
        return false;
    }
    NET_ASN_Tools::WritePointList( *pPoints, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListPoint
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPointList( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeListePoints( diaFrom ) );
    assert( DEC_Tools::CheckTypeListePoints( diaTo  ) );

    T_PointVector* pSrc = diaFrom.ToUserPtr( pSrc );
    if( !pSrc )    
    {
        assert( false );
        diaTo.SetValue( (void*)0, &DEC_Tools::GetTypeListePoints() );
        return false;
    }

    T_PointVector* pDest = new T_PointVector( *pSrc );
    diaTo.SetValue( pDest, &DEC_Tools::GetTypeListePoints() );
    return true;
}


// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPath
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPath( const ASN1T_Path& asn, DIA_Variable_ABC& dia )
{
    T_PointVector* pPtVec = new T_PointVector();
    if( !NET_ASN_Tools::ReadPath( asn, *pPtVec ) )
    {
        dia.SetValue( (void*)0, &DEC_Tools::GetTypeListePoints() );
        delete pPtVec;
        return false;
    }

    dia.SetValue( pPtVec, &DEC_Tools::GetTypeListePoints() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPath
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPath( const DIA_Variable_ABC& dia, ASN1T_Path& asn )
{
    assert( DEC_Tools::CheckTypeListePoints( dia ) );

    T_PointVector* pPoints = dia.ToUserPtr( pPoints );
    if( !pPoints )
    {
        assert( false );
        return false;
    }
    NET_ASN_Tools::WritePath( *pPoints, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPath
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPath( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeListePoints( diaFrom ) );
    assert( DEC_Tools::CheckTypeListePoints( diaTo ) );

    T_PointVector* pSrc = diaFrom.ToUserPtr( pSrc );
    if( !pSrc )    
    {
        assert( false );
        diaTo.SetValue( (void*)0, &DEC_Tools::GetTypeListePoints() );
        return false;
    }
    
    T_PointVector* pDest = new T_PointVector();
    if( !pSrc->empty() )
        pDest->push_back( pSrc->back() );
    diaTo.SetValue( pDest, &DEC_Tools::GetTypeListePoints() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListItineraire
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPathList( const ASN1T_PathList& asn, DIA_Variable_ABC& dia )
{
    T_ItinerairePtrVector itineraires;

    if( !NET_ASN_Tools::ReadPathList( asn, itineraires ) )
        return false;

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    diaObjectList.SetValueUserType( itineraires, DEC_Tools::GetTypeListePoints() );
    return true;       
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListItineraire
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPathList( const DIA_Variable_ABC& dia, ASN1T_PathList& asn )
{
    assert( DEC_Tools::CheckTypeListeListesPoints( dia ) );

    const DIA_Variable_ObjectList& diaObjectList = static_cast< const DIA_Variable_ObjectList& >( dia );

    T_ItinerairePtrVector itineraires = diaObjectList.ToUserTypeList( itineraires );
    NET_ASN_Tools::WritePathList( itineraires, asn );
    return true;        
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListItineraire
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPathList( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeListeListesPoints( diaFrom ) );
    assert( DEC_Tools::CheckTypeListeListesPoints( diaTo  ) );

    const DIA_Variable_ObjectList& diaListFrom = static_cast< const DIA_Variable_ObjectList& >( diaFrom );
          DIA_Variable_ObjectList& diaListTo   = static_cast<       DIA_Variable_ObjectList& >( diaTo );

    T_ItinerairePtrVector itinerairesFrom = diaListFrom.ToUserTypeList( itinerairesFrom );
    T_ItinerairePtrVector itinerairesTo; itinerairesTo.reserve( itinerairesFrom.size() );
    for( CIT_ItinerairePtrVector itIti = itinerairesFrom.begin(); itIti != itinerairesFrom.end(); ++itIti )
    {
        T_PointVector* pPointVector = new T_PointVector(); //$$$ RAM
        *pPointVector = **itIti;
        itinerairesTo.push_back( pPointVector );
    }
    diaListTo.SetValueUserType( itinerairesTo, DEC_Tools::GetTypeListePoints() );
    return true;
}

   
// =============================================================================
// Mission parameters tools : GDH DIA - SIM - ASN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyGDH
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyGDH( const ASN1T_DateTime& asn, DIA_Variable_ABC& dia )
{
    uint value = 0;
    ReadGDH( asn, value );
    dia.SetValue( float( value ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyGDH
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyGDH( const DIA_Variable_ABC& dia, ASN1T_DateTime& asn )
{
    float rValue = dia.ToFloat();
    if( rValue == 0. )
        return false;
    WriteGDH( rValue, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteGDH
// Created: NLD 2004-01-15
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteGDH( uint nRealTimeSec, ASN1T_DateTime& asnGDH )
{
    bpt::ptime time = bpt::from_time_t( 0 ) +  bpt::seconds( nRealTimeSec );
    asnGDH = bpt::to_iso_string( time ).c_str();
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadTick
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ReadTick( const ASN1T_DateTime& asn, uint& simTick )
{
    uint value = 0;
    ReadGDH( asn, value );
    simTick = MIL_AgentServer::GetWorkspace().RealTimeToTick( value );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteTick
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteTick( uint simTick, ASN1T_DateTime& asnGDH )
{
    WriteGDH( MIL_AgentServer::GetWorkspace().TickToRealTime( simTick ), asnGDH );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyGDH
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyGDH( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    diaTo.SetValue( diaFrom.ToFloat() );    
    return true;
}

// =============================================================================
// Mission parameters tools : basic types
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyDirection
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyDirection( const ASN1T_Heading& asn, DIA_Variable_ABC& dia )
{
    MT_Vector2D* pDir = new MT_Vector2D();
    NET_ASN_Tools::ReadDirection( asn, *pDir );
    dia.SetValue( pDir, &DEC_Tools::GetTypeDirection() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyDirection
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyDirection( const DIA_Variable_ABC& dia, ASN1T_Heading& asn )
{
    assert( DEC_Tools::CheckTypeDirection( dia ) );

    MT_Vector2D* pValue = dia.ToUserPtr( pValue );
    if( !pValue )
    {
        assert( false );
        asn = 0;
        return false;
    }
    NET_ASN_Tools::WriteDirection( *pValue, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyDirection
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyDirection( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeDirection( diaFrom ) );
    assert( DEC_Tools::CheckTypeDirection( diaTo ) );

    MT_Vector2D* pSrc = diaFrom.ToUserPtr( pSrc );
    if( !pSrc )
    {
        assert( false );
        return false;
    }
    assert( !pSrc->IsZero() );
    MT_Vector2D* pDest = new MT_Vector2D( *pSrc );
    diaTo.SetValue( pDest, &DEC_Tools::GetTypeDirection() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyString
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyString( const ASN1VisibleString& asn, DIA_Variable_ABC& dia )
{
    std::string strTmp = asn;
    dia.SetValue( strTmp ); 
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyString
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyString( const DIA_Variable_ABC& dia, ASN1VisibleString& asn )
{
    asn = dia.ToString().c_str();
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyString
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyString( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    diaTo.SetValue( diaFrom.ToString() );    
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyBool
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyBool( const ASN1BOOL& asn, DIA_Variable_ABC& dia )
{
    dia.SetValue( asn != 0 ); 
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyBool
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyBool( const DIA_Variable_ABC& dia, ASN1BOOL& asn )
{
    asn = dia.ToBool();
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyBool
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyBool( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    diaTo.SetValue( diaFrom.ToBool() );    
    return true;
}
    
// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyNumeric
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyNumeric( const ASN1UINT& asn, DIA_Variable_ABC& dia )
{
    dia.SetValue( (float)asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyNumeric
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyNumeric( const ASN1INT& asn, DIA_Variable_ABC& dia )
{
    dia.SetValue( (float)asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyNumeric
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyNumeric( const ASN1REAL& asn, DIA_Variable_ABC& dia )
{
    dia.SetValue( (float)asn );    
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyNumeric
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyNumeric( const DIA_Variable_ABC& dia, ASN1UINT& asn )
{
    asn = (uint)dia.ToFloat();    
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyNumeric
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyNumeric( const DIA_Variable_ABC& dia, ASN1INT& asn )
{
    asn = (int)dia.ToFloat();    
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyNumeric
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyNumeric( const DIA_Variable_ABC& dia, ASN1REAL& asn )
{
    asn = dia.ToFloat();    
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyNumeric
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyNumeric( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    diaTo.SetValue( diaFrom.ToFloat() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyNatureAtlas
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyNatureAtlas( const ASN1T_AtlasNature& asn, DIA_Variable_ABC& dia )
{
    int nNatureAtlas = ( asn.data[0] << 8 ) + asn.data[1];
    dia.SetValue( nNatureAtlas );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyNatureAtlas
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyNatureAtlas( const DIA_Variable_ABC& dia, ASN1T_AtlasNature& asn )
{
    int nCategories = dia.ToId();
    asn.numbits = 11; //$$$ n'importe quoi ...
    asn.data[0] = (char)( nCategories >> 8 );
    asn.data[1] = (char)( nCategories & 0x00FF );    
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyNatureAtlas
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyNatureAtlas( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    diaTo.SetValue( diaFrom.ToId() );
    return true;
}

// -----------------------------------------------------------------------------
// DOTATION
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyDotationType
// Created: NLD 2006-08-04
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyDotationType( const ASN1T_DotationType& asn, DIA_Variable_ABC& dia )
{
    const PHY_DotationCategory* pData = PHY_DotationType::FindDotationCategory( asn );
    if( !pData ) 
        return false;

    dia.SetValue( (void*)pData, &DEC_Tools::GetTypeDotation() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyDotationType
// Created: NLD 2006-08-04
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyDotationType( const DIA_Variable_ABC& dia, ASN1T_DotationType& asn )
{
    assert( DEC_Tools::CheckTypeDotation( dia ) );

    const PHY_DotationCategory* pDotationCategory = dia.ToUserPtr( pDotationCategory );
    assert( pDotationCategory );
    if( !pDotationCategory )
        return false;

    asn = pDotationCategory->GetMosID();
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyDotationType
// Created: NLD 2006-08-04
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyDotationType( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeDotation( diaFrom ) );
    assert( DEC_Tools::CheckTypeDotation( diaTo ) );
    diaTo.SetValue( diaFrom.ToPtr(), &DEC_Tools::GetTypeDotation() );
    return true;
}

// -----------------------------------------------------------------------------
// EQUIPMENT
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyEquipmentType
// Created: NLD 2006-08-04
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyEquipmentType( const ASN1T_EquipmentType& asn, DIA_Variable_ABC& dia )
{
    const PHY_ComposanteTypePion* pData = PHY_ComposanteTypePion::Find( asn );
    if( !pData ) 
        return false;

    dia.SetValue( (void*)pData, &DEC_Tools::GetTypeEquipement() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyEquipmentType
// Created: NLD 2006-08-04
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyEquipmentType( const DIA_Variable_ABC& dia, ASN1T_EquipmentType& asn )
{
    assert( DEC_Tools::CheckTypeEquipement( dia ) );

    const PHY_ComposanteTypePion* pTmp = dia.ToUserPtr( pTmp );
    assert( pTmp );
    if( !pTmp )
        return false;

    asn = pTmp->GetMosID();
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyEquipmentType
// Created: NLD 2006-08-04
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyEquipmentType( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeEquipement( diaFrom ) );
    assert( DEC_Tools::CheckTypeEquipement( diaTo ) );
    diaTo.SetValue( diaFrom.ToPtr(), &DEC_Tools::GetTypeEquipement() );
    return true;
}

// -----------------------------------------------------------------------------
// Indirect fire
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyIndirectFire
// Created: NLD 2006-12-07
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyIndirectFire( const DIA_Variable_ABC& dia, ASN1T_UnitFire& asn )
{
    assert( DEC_Tools::CheckTypeTirIndirect( dia ) );
    asn = (uint)dia.ToPtr();
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyIndirectFire
// Created: NLD 2006-12-07
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyIndirectFire( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeTirIndirect( diaFrom ) );
    assert( DEC_Tools::CheckTypeTirIndirect( diaTo ) );
    diaTo.SetValue( diaFrom.ToPtr(), &DEC_Tools::GetTypeTirIndirect() );
    return true;
}

// -----------------------------------------------------------------------------
// ENUMERATION
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyEnumeration
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyEnumeration( uint asn, DIA_Variable_ABC& dia )
{
    dia.SetValue( (int)asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyEnumeration
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyEnumeration( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    diaTo.SetValue( diaFrom.ToId() );    
    return true;
}
    
// =============================================================================
// MISSION PARAMETERS : KNOWLEDGE POPULATION DIA - SIM - ASN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyKnowledgePopulation
// Created: HME 05-12-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPopulationKnowledge( const ASN1T_PopulationKnowledge& asn, DIA_Variable_ABC& dia, const DEC_KnowledgeResolver_ABC& resolver )
{
    DEC_Knowledge_Population* pKnowledge = NET_ASN_Tools::ReadPopulationKnowledge( asn, resolver );
    if( !pKnowledge ) 
        return false;

    dia.SetValue( (void*)pKnowledge->GetID(), &DEC_Tools::GetTypeConnaissancePopulation() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyKnowledgePopulation
// Created: HME 05-12-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPopulationKnowledge( const DIA_Variable_ABC& dia, ASN1T_PopulationKnowledge& asn, const DEC_KnowledgeResolver_ABC& resolver )
{
    assert( DEC_Tools::CheckTypeConnaissancePopulation( dia ) );

    DEC_Knowledge_Population* pKnowledge = resolver.ResolveKnowledgePopulation( dia );
    if( !pKnowledge )
        return false;
    
    asn = pKnowledge->GetID();
    return true;    
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyKnowledgePopulation
// Created: HME 05-12-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::CopyPopulationKnowledge( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeConnaissancePopulation( diaFrom ) );
    assert( DEC_Tools::CheckTypeConnaissancePopulation( diaTo ) );
    assert( &diaFrom != &diaTo );
    diaTo.SetValue( diaFrom.ToPtr(), &DEC_Tools::GetTypeConnaissancePopulation() );
}
