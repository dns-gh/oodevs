//*****************************************************************************
//
// $Created: NLD 2003-02-28 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_ASN_Tools.h $
// $Author: Nld $
// $Modtime: 27/04/05 18:07 $
// $Revision: 8 $
// $Workfile: NET_ASN_Tools.h $
//
//*****************************************************************************

#ifndef __NET_ASN_Tools_h_
#define __NET_ASN_Tools_h_

#include "Knowledge/DEC_Knowledge_Def.h"
#include "MT_Tools/MT_Vector2DTypes.h"

namespace sword
{
    class AutomatId;
    class AutomatIdList;
    class CoordLatLong;
    class CoordLatLongList;
    class DateTime;
    class Heading;
    class Line;
    class Location;
    class LocationList;
    class LogMaintenancePriorities;
    class LogMedicalPriorities;
    class MissionObjective;
    class MissionObjectiveList;
    class ObjectKnowledgeId;
    class ObjectKnowledgeIdList;
    class Path;
    class PathList;
    class PlannedWork;
    class PlannedWorkList;
    class Point;
    class PointList;
    class Polygon;
    class PolygonList;
    class CrowdKnowledgeId;
    class UnitId;
    class UnitKnowledgeId;
    class UnitKnowledgeIdList;
    class UnitIdList;
}

class TER_Localisation;
class DEC_Gen_Object;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_Population;
class MT_Ellipse;
class DEC_Decision_ABC;
class DEC_KnowledgeResolver_ABC;
class MIL_EntityManager_ABC;

//=============================================================================
// Created: NLD 2003-02-28
// Last modified: JVT 04-03-25
//=============================================================================
class NET_ASN_Tools
{
public:
    // @name Misc tools
    //@{
    static void Delete( sword::MissionObjective& asn );
    static void Delete( sword::MissionObjectiveList& asn );
    static void Delete( sword::PlannedWork& asn );
    static void Delete( sword::PlannedWorkList& asn );
    static void Delete( sword::UnitIdList& asn );
    static void Delete( sword::AutomatIdList& asn );
    static void Delete( sword::Polygon& asn );
    static void Delete( sword::PolygonList& asn );
    static void Delete( sword::PointList& asn );
    static void Delete( sword::PathList& asn );
    static void Delete( sword::LocationList& asn );
    static void Delete( sword::UnitKnowledgeIdList& asn );
    static void Delete( sword::ObjectKnowledgeIdList& asn );
    static void Delete( sword::LogMedicalPriorities& asn );
    static void Delete( sword::LogMaintenancePriorities& asn );
    static void Delete( sword::CoordLatLongList& asn );
    //@}

    //! @name Decoding tools
    //@{
    static bool ReadLine( const sword::Line& asn, TER_Localisation& localisation );
    static bool ReadLine( const sword::Line& asn, T_PointVector& pointVector );
    static bool ReadPoint( const sword::Point& asn, MT_Vector2D& vPoint );
    static void ReadPoint( const sword::CoordLatLong& asn, MT_Vector2D& vPoint );
    static bool ReadPointList( const sword::PointList& asn, T_PointVector& pointVector );
    static bool ReadLocation( const sword::Location& asn, TER_Localisation& localisation, double rPointSize = 250. );
    static void ReadDirection( const sword::Heading& asn, MT_Vector2D& vDir );
    static void ReadGDH( const sword::DateTime& asn, unsigned int& realTimeSec );
    static void ReadTick( const sword::DateTime& asn, unsigned int& simTick );
    //@}

    //! @name Encoding tools
    //@{
    static bool WriteCoordinates( const T_PointVector& points, sword::CoordLatLongList& asn );
    static bool WriteCoordinates( std::vector< boost::shared_ptr< MT_Vector2D > > points, sword::CoordLatLongList& asn );
    static bool WriteLine( const T_PointVector& points, sword::Line& asn );
    static void WriteLine( const TER_Localisation& localisation, sword::Line& asn );
    static void WriteDirection( const MT_Vector2D& vDir, sword::Heading& asn );
    static void WriteLocation( const TER_Localisation& localisation, sword::Location& asn );
    static void WritePoint( const MT_Vector2D& vPoint, sword::Point& asn );
    static void WritePoint( const MT_Vector2D& vPoint, sword::CoordLatLong& asn );
    static void WritePointList( const T_PointList& points, sword::PointList& asn );
    static void WriteEllipse( const MT_Ellipse& ellipse, sword::Location& asn );
    static void WritePath( const T_PointList& points, sword::Path& asn );
    static void WritePath( const T_PointVector& points, sword::Path& asn );
    static void WriteGenObject( const DEC_Gen_Object& object, sword::PlannedWork& asn );
    static void WriteGDH( unsigned int nRealTimeSec, sword::DateTime& asnGDH );
    static void WriteTick( unsigned int simTick, sword::DateTime& asnGDH );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< T_PointVector* >           T_ItinerairePtrVector;
    typedef T_ItinerairePtrVector::iterator        IT_ItinerairePtrVector;
    typedef T_ItinerairePtrVector::const_iterator CIT_ItinerairePtrVector;

    typedef std::vector< boost::shared_ptr< TER_Localisation > > T_LocalisationPtrVector;
    typedef T_LocalisationPtrVector::const_iterator            CIT_LocalisationPtrVector;
    //@}

public:
    // @name Decoding tools
    //@{
    static bool ReadPath( const sword::Path& asn, T_PointVector& pointVector );
    static bool ReadPolygon( const sword::Polygon& asn, TER_Localisation& localisation );

    static bool ReadLocationList( const sword::LocationList& asn, T_LocalisationPtrVector& localisationVector, unsigned int nNbrEltMin = 0 );
    static bool ReadPolygonList( const sword::PolygonList& asn, T_LocalisationPtrVector& localisationVector, unsigned int nNbrEltMin = 0 );
    static bool ReadPathList( const sword::PathList& asn, T_ItinerairePtrVector& itineraireVector );

    static DEC_Decision_ABC*            ReadAgent              ( const sword::UnitId&               asn );
    static DEC_Decision_ABC*            ReadAutomate           ( const sword::UnitId&               asn );
    static boost::shared_ptr< DEC_Knowledge_Agent > ReadAgentKnowledge     ( const sword::UnitKnowledgeId&      asn, const DEC_KnowledgeResolver_ABC& resolver );
    static DEC_Knowledge_Population*    ReadPopulationKnowledge( const sword::CrowdKnowledgeId& asn, const DEC_KnowledgeResolver_ABC& resolver );

    static boost::shared_ptr< DEC_Knowledge_Object > ReadObjectKnowledge    ( const sword::ObjectKnowledgeId&     asn, const DEC_KnowledgeResolver_ABC& resolver );
    static bool                         ReadObjectKnowledgeList( const sword::ObjectKnowledgeIdList& asn, T_KnowledgeObjectDiaIDVector& knowledgeList, const DEC_KnowledgeResolver_ABC& resolver );
    //@}

    // @name Encoding tools
    //@{
    static void WritePolygon( const TER_Localisation& localisation, sword::Polygon& asn );
    static void WriteLocationList( const T_LocalisationPtrVector& localisationVector, sword::LocationList& asn );
    static void WritePolygonList( const T_LocalisationPtrVector& localisationVector, sword::PolygonList& asn );
    static void WritePathList( const T_ItinerairePtrVector& itineraireVector, sword::PathList& asn );
    static void WritePointList( const T_PointVector& pointVector, sword::PointList& asn );

    static void WriteAgent              ( const DEC_Decision_ABC&             pion      , sword::UnitId&     asn );
    static void WriteAutomate           ( const DEC_Decision_ABC&             automate  , sword::AutomatId&  asn );
    static void WriteAgentKnowledge     ( const DEC_Knowledge_Agent&          knowledge , sword::UnitKnowledgeId&      asnKnowledge );
    static void WriteObjectKnowledge    ( const DEC_Knowledge_Object&         knowledge , sword::ObjectKnowledgeId&     asnKnowledge );
    static void WriteObjectKnowledgeList( const T_KnowledgeObjectDiaIDVector& knowledges, sword::ObjectKnowledgeIdList& listResult, const DEC_KnowledgeResolver_ABC& resolver );
    //@}
};

#endif // __NET_ASN_Tools_h_
