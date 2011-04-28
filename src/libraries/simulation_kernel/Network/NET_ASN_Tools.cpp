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
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "MT_Tools/MT_Ellipse.h"
#include "protocol/Protocol.h"
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

using namespace sword;
namespace bpt = boost::posix_time;

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadLocation
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
// static
bool NET_ASN_Tools::ReadLocation( const Location& asnLocalisation, TER_Localisation& localisation )
{
    //$$$$ ACCEDER DIRECTEMENT A LA LOCALISATION (EVITER COPIE)
    T_PointVector pointVector;
    pointVector.reserve( asnLocalisation.coordinates().elem_size() );
    for( int i = 0; i < asnLocalisation.coordinates().elem_size(); ++i )
    {
        MT_Vector2D vPos;
        NET_ASN_Tools::ReadPoint( asnLocalisation.coordinates().elem(i), vPos );
        pointVector.push_back( vPos );
    }
    return localisation.Reset( (TER_Localisation::E_LocationType)asnLocalisation.type(), pointVector );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadPoint
// Created: AGN 03-01-20
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::ReadPoint( const CoordLatLong& asnPoint, MT_Vector2D& vect )
{
    MIL_Tools::ConvertCoordMosToSim( asnPoint, vect );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadPoint
// Created: NLD 2003-03-26
//-----------------------------------------------------------------------------
// static
bool NET_ASN_Tools::ReadPoint( const Point& asnLocalisation, MT_Vector2D& vect )
{
    if( asnLocalisation.location().type() != Location_Geometry_point )
        return false;
    if( asnLocalisation.location().coordinates().elem_size() != 1 )
        return false;
    ReadPoint( asnLocalisation.location().coordinates().elem(0), vect );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadLine
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadLine( const Line& asn, T_PointVector& points )
{
    if( asn.location().type() != Location_Geometry_line || asn.location().coordinates().elem_size()< 2 )
        return false;
    points.clear();
    points.reserve( asn.location().coordinates().elem_size());
    for( int i = 0; i < asn.location().coordinates().elem_size(); ++i )
    {
        MT_Vector2D vPosTmp;
        MIL_Tools::ConvertCoordMosToSim( asn.location().coordinates().elem(i), vPosTmp );
        points.push_back( vPosTmp );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadLine
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadLine( const Line& asn, TER_Localisation& localisation )
{
    if( asn.location().type() != Location_Geometry_line || asn.location().coordinates().elem_size()< 2 )
        return false;
    return ReadLocation( asn.location(), localisation );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadPath
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadPath( const Path& asn, T_PointVector& itineraire )
{
    if( asn.location().type() != Location_Geometry_line || asn.location().coordinates().elem_size()< 1 )
        return false;
    itineraire.clear(); itineraire.reserve( asn.location().coordinates().elem_size());
    for( int i = 0; i < asn.location().coordinates().elem_size(); ++i )
    {
        MT_Vector2D vPos;
        ReadPoint( asn.location().coordinates().elem(i), vPos );
        itineraire.push_back( vPos );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadPolygon
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadPolygon( const Polygon&      asn, TER_Localisation& localisation )
{
    if( asn.location().type() != Location_Geometry_polygon || asn.location().coordinates().elem_size()< 3 )
        return false;
    return ReadLocation( asn.location(), localisation );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadLocationList
// Created: NLD 2003-08-11
// -----------------------------------------------------------------------------
// static
bool NET_ASN_Tools::ReadLocationList( const LocationList& asnLocationList, T_LocalisationPtrVector& localisationVector, unsigned int nNbrEltMin )
{
    localisationVector.clear(); localisationVector.reserve( asnLocationList.elem_size() );
    for( int i = 0; i < asnLocationList.elem_size(); ++i )
    {
        boost::shared_ptr< TER_Localisation > pLocalisation( new TER_Localisation() ); //$$$ RAM
        if( !ReadLocation( asnLocationList.elem( i ), *pLocalisation ) )
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
bool NET_ASN_Tools::ReadPolygonList( const PolygonList& asn, T_LocalisationPtrVector& localisationVector, unsigned int nNbrEltMin )
{
    for( int i = 0; i < asn.elem_size(); ++i )
    {
        if( asn.elem(i).location().type() != Location_Geometry_polygon || asn.elem(i).location().coordinates().elem_size() < 3 )
            return false;
    }
    localisationVector.clear(); localisationVector.reserve( asn.elem_size() );
    for( int i = 0; i < asn.elem_size(); ++i )
    {
        boost::shared_ptr< TER_Localisation > pLocalisation( new TER_Localisation() ); //$$$ RAM
        if( !ReadLocation( asn.elem(i).location(), *pLocalisation ) )
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
bool NET_ASN_Tools::ReadPointList( const PointList& asn, T_PointVector& pointVector )
{
    pointVector.clear(); pointVector.reserve( asn.elem_size() );
    for( int i = 0; i < asn.elem_size(); ++i )
    {
        MT_Vector2D vPos;
        ReadPoint( asn.elem(i).location().coordinates().elem(0), vPos );
        pointVector.push_back( vPos );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadListItineraire
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadPathList( const PathList&   asn, T_ItinerairePtrVector& itineraireVector )
{
    itineraireVector.clear(); itineraireVector.reserve( asn.elem_size() );
    for( int i = 0; i < asn.elem_size(); ++i )
    {
        T_PointVector* pItineraire = new T_PointVector(); //$$$ RAM
        if( ReadPath( asn.elem(i), *pItineraire ) )
            itineraireVector.push_back( pItineraire );
        else
        {
            for( IT_ItinerairePtrVector it = itineraireVector.begin(); it != itineraireVector.end(); ++it )
                delete *it;
            itineraireVector.clear();
            return false;
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadAgent
// Created: NLD 2003-03-24
//-----------------------------------------------------------------------------
// static
DEC_Decision_ABC* NET_ASN_Tools::ReadAgent( const UnitId& asnAgent )
{
    MIL_AgentPion* pPion = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( asnAgent.id() );
    if( !pPion )
        return 0;
    return &pPion->GetDecision();
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadAutomate
// Created: NLD 2003-03-24
//-----------------------------------------------------------------------------
// static
DEC_Decision_ABC* NET_ASN_Tools::ReadAutomate( const UnitId& asnAgent )
{
    MIL_Automate* pAutomate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( asnAgent.id() );
    if( !pAutomate )
        return 0;
    return &pAutomate->GetDecision();
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadKnowledgeAgent
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
// static $$NLDJVT$$ Interet de cette fonction
boost::shared_ptr< DEC_Knowledge_Agent > NET_ASN_Tools::ReadAgentKnowledge( const UnitKnowledgeId& asnAgent, const DEC_KnowledgeResolver_ABC& resolver )
{
    return resolver.ResolveKnowledgeAgent( asnAgent );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadObjectKnowledge
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
// static
boost::shared_ptr< DEC_Knowledge_Object > NET_ASN_Tools::ReadObjectKnowledge( const ObjectKnowledgeId& asnObject, const DEC_KnowledgeResolver_ABC& resolver )
{
    return resolver.ResolveKnowledgeObject( asnObject );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadListKnowledgeObject
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadObjectKnowledgeList( const ObjectKnowledgeIdList& asnListObject, T_KnowledgeObjectDiaIDVector& knowledges, const DEC_KnowledgeResolver_ABC& resolver )
{
    knowledges.clear(); knowledges.reserve( asnListObject.elem_size() );
    for( int n = 0; n < asnListObject.elem_size(); ++n )
    {
        boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = ReadObjectKnowledge( asnListObject.elem( n ), resolver );
        if( !pKnowledge )
            return false;
        knowledges.push_back( pKnowledge );
    }
    return true;
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadPopulationKnowledge
// Created: HME 05-12-22
//-----------------------------------------------------------------------------
DEC_Knowledge_Population* NET_ASN_Tools::ReadPopulationKnowledge( const CrowdKnowledgeId& asnPopulation, const DEC_KnowledgeResolver_ABC& resolver )
{
    return resolver.ResolveKnowledgePopulation( asnPopulation );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadGDH
// Created: NLD 2004-01-15
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ReadGDH( const DateTime& asn, unsigned int& realTimeSec )
{
    bpt::ptime time = bpt::from_iso_string( asn.data() );
    realTimeSec = ( time - bpt::from_time_t( 0 ) ).total_seconds();
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteDirection
// Created: NLD 2003-05-13
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WriteDirection( const MT_Vector2D& vDir, Heading& asnDir )
{
    static const MT_Vector2D vNorth( 0, 1 );
    assert( !vDir.IsZero() );
    double rAngle = Angle( vNorth, vDir ); // Trigo ...[-PI, PI]
    if( rAngle < 0 )
        rAngle += (2 * MT_PI); // Trigo ...[0, 2PI]
    rAngle = (2 * MT_PI) - rAngle; // Clockwise [0, 2PI]
    asnDir.set_heading( static_cast< unsigned int >( rAngle * 180. / MT_PI + 0.5f) );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteLocation
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
void NET_ASN_Tools::WriteLocation( const TER_Localisation& localisation, Location& asnLocalisation )
{
    assert( localisation.GetType() != TER_Localisation::eNone );
    asnLocalisation.set_type( (Location_Geometry)localisation.GetType() );
    if( localisation.GetPoints().empty() )
        return;
    for( CIT_PointVector itPoint = localisation.GetPoints().begin(); itPoint != localisation.GetPoints().end(); ++itPoint )
        WritePoint( *itPoint, *asnLocalisation.mutable_coordinates()->add_elem() );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WritePoint
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WritePoint( const MT_Vector2D& vPos, CoordLatLong& asnCoordUTM )
{
    MIL_Tools::ConvertCoordSimToMos( vPos, asnCoordUTM );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WritePoint
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WritePoint( const MT_Vector2D& vPos, Point& asnLocalisation )
{
    asnLocalisation.mutable_location()->set_type( Location_Geometry_point );
    WritePoint( vPos, *asnLocalisation.mutable_location()->mutable_coordinates()->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteLine
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteLine( const TER_Localisation& localisation, Line& asn )
{
    WriteLocation( localisation, *asn.mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteCoordinates
// Created: AGE 2007-07-06
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::WriteCoordinates( const T_PointVector& points , CoordLatLongList& asn )
{
    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
        WritePoint( *it, *asn.add_elem() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteCoordinates
// Created: AHC 2009-07-30
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::WriteCoordinates( std::vector< boost::shared_ptr< MT_Vector2D > > points , CoordLatLongList& asn )
{
    for( std::vector< boost::shared_ptr< MT_Vector2D > >::iterator it = points.begin(); it != points.end(); ++it )
        WritePoint( **it, *asn.add_elem() );
    return true;
}

// -----------------------------------------------------------------------------
// Name NET_ASN_Tools::WriteLine
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::WriteLine( const T_PointVector& points, Line& asn )
{
    asn.mutable_location()->set_type( sword::Location_Geometry_line );
    if( points.size() < 2 )
        return false;
    return WriteCoordinates( points, *asn.mutable_location()->mutable_coordinates() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WritePath
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WritePath( const T_PointVector& pointVector, Path& asn )
{
    asn.mutable_location()->set_type( Location_Geometry_line );
    if( pointVector.empty() )
        return;
    WriteCoordinates( pointVector, *asn.mutable_location()->mutable_coordinates() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WritePath
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WritePath( const T_PointList& points, Path& asn )
{
    asn.mutable_location()->set_type( Location_Geometry_line );
    for( CIT_PointList itPoint = points.begin(); itPoint != points.end(); ++itPoint )
        WritePoint( *itPoint, *asn.mutable_location()->mutable_coordinates()->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WritePolygon
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WritePolygon( const TER_Localisation& localisation, Polygon& asn )
{
    WriteLocation( localisation, *asn.mutable_location() );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteEllipse
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void NET_ASN_Tools::WriteEllipse( const MT_Ellipse& ellipse, Location& asnLocalisation )
{
    asnLocalisation.set_type( Location_Geometry_ellipse );
    WritePoint( ellipse.GetCenter(), *asnLocalisation.mutable_coordinates()->add_elem() );
    WritePoint( ellipse.GetMajorAxisHighPoint(), *asnLocalisation.mutable_coordinates()->add_elem() );
    WritePoint( ellipse.GetMinorAxisHighPoint(), *asnLocalisation.mutable_coordinates()->add_elem() );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListPoint
// Created: AGN 03-01-20
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WritePointList( const T_PointVector& pointVector, PointList& asn )
{
    if( pointVector.empty() )
        return;
    unsigned int i = 0;
    for( CIT_PointVector itPoint = pointVector.begin(); itPoint != pointVector.end(); ++itPoint )
        WritePoint( *itPoint, *asn.mutable_elem( i++ )->mutable_location()->mutable_coordinates()->add_elem() ); // $$$$ FHD 2009-10-07: point or coordlatlong ?
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListPoint
// Created: AGN 03-01-20
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WritePointList( const T_PointList& points, PointList& asn )
{
    if( points.empty() )
        return;
    unsigned int i = 0;
    for( CIT_PointList itPoint = points.begin(); itPoint != points.end(); ++itPoint )
        WritePoint( *itPoint, *asn.mutable_elem( i++ )->mutable_location()->mutable_coordinates()->add_elem() );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteLocationList
// Created: AGN 03-08-18
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WriteLocationList( const T_LocalisationPtrVector& locaList, LocationList& asnList )
{
    if( locaList.empty() )
        return;
    for (unsigned int i = 0; i < locaList.size(); ++i )
        asnList.add_elem();
    for( CIT_LocalisationPtrVector it = locaList.begin(); it != locaList.end(); ++it )
        WriteLocation( **it, *asnList.mutable_elem( it - locaList.begin() ) );
}


//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListPolygon
// Created: AGN 03-08-18
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WritePolygonList( const T_LocalisationPtrVector& locaList, PolygonList& asnList )
{
    if( locaList.empty() )
        return;
    for( CIT_LocalisationPtrVector it = locaList.begin(); it != locaList.end(); ++it )
        WriteLocation( **it, *asnList.mutable_elem(it - locaList.begin())->mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListItineraire
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WritePathList( const T_ItinerairePtrVector& itineraireVector, PathList&   asn )
{
    if( itineraireVector.empty() )
        return;
    for( CIT_ItinerairePtrVector it = itineraireVector.begin(); it != itineraireVector.end(); ++it )
        WritePath( **it, *asn.mutable_elem(it - itineraireVector.begin()) );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteAgent
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteAgent( const DEC_Decision_ABC& pion, UnitId& asn )
{
    asn.set_id( pion.GetPion().GetID() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteAutomate
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteAutomate( const DEC_Decision_ABC& automate, AutomatId& asn )
{
    asn.set_id( automate.GetAutomate().GetID() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteAgentKnowledge( const DEC_Knowledge_Agent& knowledge, UnitKnowledgeId& asnKnowledge )
{
    asnKnowledge.set_id( knowledge.GetID() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteObjectKnowledge
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteObjectKnowledge( const DEC_Knowledge_Object& knowledge, ObjectKnowledgeId& asnKnowledge )
{
    asnKnowledge.set_id( knowledge.GetID() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListKnowledgeObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteObjectKnowledgeList( const T_KnowledgeObjectDiaIDVector& knowledges, sword::ObjectKnowledgeIdList& listResult, const DEC_KnowledgeResolver_ABC& /*resolver*/ )
{
    listResult.clear_elem();
    for( CIT_KnowledgeObjectDiaIDVector itKnowledge = knowledges.begin(); itKnowledge != knowledges.end(); ++itKnowledge )
    {
        boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = *itKnowledge;
        listResult.add_elem()->set_id( pKnowledge && pKnowledge->IsValid() ? pKnowledge->GetID() : 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteGDH
// Created: NLD 2004-01-15
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteGDH( unsigned int nRealTimeSec, DateTime& asnGDH )
{
    bpt::ptime time = bpt::from_time_t( 0 ) +  bpt::seconds( nRealTimeSec );
    asnGDH.set_data( bpt::to_iso_string( time ) );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadTick
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ReadTick( const DateTime& asn, unsigned int& simTick )
{
    unsigned int value = 0;
    ReadGDH( asn, value );
    simTick = MIL_AgentServer::GetWorkspace().RealTimeToTick( value );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteTick
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteTick( unsigned int simTick, DateTime& asnGDH )
{
    WriteGDH( MIL_AgentServer::GetWorkspace().TickToRealTime( simTick ), asnGDH );
}
