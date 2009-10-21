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
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/MIL_Army.h"

#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/DEC_KnowledgeResolver_ABC.h"

#include "Decision/DEC_Decision_ABC.h"
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
        boost::shared_ptr< TER_Localisation > pLocalisation( new TER_Localisation() ); //$$$ RAM
        if( !ReadLocation( asnLocationList.elem[i], *pLocalisation ) )
        {
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
        boost::shared_ptr< TER_Localisation > pLocalisation( new TER_Localisation() ); //$$$ RAM
        if( !ReadLocation( asn.elem[i], *pLocalisation ) )
        {
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
DEC_Decision_ABC* NET_ASN_Tools::ReadAgent( const ASN1T_Unit& asnAgent )
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
        DEC_Decision_ABC* pAgent = ReadAgent( asnListAgent.elem[n] );
        if( !pAgent )
        {
            unitList.clear();
            return false;
        }
        unitList.push_back( reinterpret_cast< DIA_TypedObject* >( pAgent ) );// $$$$ LDC: Remove DIA_TypedObjects
    }
    return true;
}


//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadAutomate
// Created: NLD 2003-03-24
//-----------------------------------------------------------------------------
// static
DEC_Decision_ABC* NET_ASN_Tools::ReadAutomate( const ASN1T_Unit& asnAgent )
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
        DEC_Decision_ABC* pAgent = ReadAutomate( asnListAgent.elem[n] );
        if( !pAgent)
        {
            unitList.clear();
            return false;
        }
        unitList.push_back( reinterpret_cast< DIA_TypedObject* >( pAgent ) );// $$$$ LDC: Remove DIA_TypedObjects
    }
    return true;
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadKnowledgeAgent
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
// static $$NLDJVT$$ Interet de cette fonction
boost::shared_ptr< DEC_Knowledge_Agent > NET_ASN_Tools::ReadAgentKnowledge( const ASN1T_UnitKnowledge& asnAgent, const DEC_KnowledgeResolver_ABC& resolver )
{
    return resolver.ResolveKnowledgeAgent( asnAgent );
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
        knowledges.push_back( pKnowledge->GetID() );
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
// Name: NET_ASN_Tools::WriteCoordinates
// Created: AHC 2009-07-30
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::WriteCoordinates( std::vector< boost::shared_ptr< MT_Vector2D > > points , ASN1T_CoordLatLongList& asn )
{
    asn.n    = points.size();
    asn.elem = asn.n ? new ASN1T_CoordLatLong[ asn.n ] : 0;
    uint i = 0;
    for( std::vector< boost::shared_ptr< MT_Vector2D > >::iterator it = points.begin(); it != points.end(); ++it )
        WritePoint( **it, asn.elem[i++] );
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
void NET_ASN_Tools::WriteAgent( const DEC_Decision_ABC& pion, ASN1T_Unit& asn )
{
    asn = pion.GetPion().GetID();
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteAutomate
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteAutomate( const DEC_Decision_ABC& automate, ASN1T_Automat& asn )
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
        pOID[i++] = reinterpret_cast< DEC_Decision_ABC* >( *itUnit )->GetPion().GetID();// $$$$ LDC: Remove DIA_TypedObjects
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
        pOID[i++] = reinterpret_cast< DEC_Decision_ABC* >( *itUnit )->GetAutomate().GetID();// $$$$ LDC: Remove DIA_TypedObjects
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
        const DEC_Knowledge_Object* pKnowledge = resolver.ResolveKnowledgeObject( *itKnowledge );
        if( pKnowledge )
            asnListKnowledge.elem[i] = pKnowledge->GetID();
        else
            asnListKnowledge.elem[i] = 0;       
        ++i;
    }
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

// =============================================================================
// Mission parameters tools : basic types
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyNatureAtlas
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyNatureAtlas( const ASN1T_AtlasNature& asn, int& value )
{
    value = ( asn.data[0] << 8 ) + asn.data[1];
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyNatureAtlas
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyNatureAtlas( int value, ASN1T_AtlasNature& asn )
{
    asn.numbits = 11; //$$$ n'importe quoi ...
    asn.data[0] = (char)( value >> 8 );
    asn.data[1] = (char)( value & 0x00FF );    
    return true;
}
