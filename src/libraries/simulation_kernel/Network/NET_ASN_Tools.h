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

namespace Common
{
    class MsgAtlasNature;
    class MsgAutomat;
    class MsgAutomatList;
    class MsgCoordLatLong;
    class MsgCoordLatLongList;
    class MsgDateTime;
    class MsgHeading;
    class MsgLine;
    class MsgLocation;
    class MsgLocationList;
    class MsgLogMaintenancePriorities;
    class MsgLogMedicalPriorities;
    class MsgMissionObjective;
    class MsgMissionObjectiveList;
    class MsgObjectKnowledge;
    class MsgObjectKnowledgeList;
    class MsgPath;
    class MsgPathList;
    class MsgPlannedWork;
    class MsgPlannedWorkList;
    class MsgPoint;
    class MsgPointList;
    class MsgPolygon;
    class MsgPolygonList;
    class MsgPopulationKnowledge;
    class MsgUnit;
    class MsgUnitKnowledge;
    class MsgUnitKnowledgeList;
    class MsgUnitList;
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

    static bool CopyNatureAtlas ( int value, Common::MsgAtlasNature& asn );
    static bool CopyNatureAtlas ( const Common::MsgAtlasNature& asn, int& value );
    //@}

    // @name Misc tools
    //@{
    static void Delete( Common::MsgMissionObjective& asn );
    static void Delete( Common::MsgMissionObjectiveList& asn );
    static void Delete( Common::MsgPlannedWork& asn );
    static void Delete( Common::MsgPlannedWorkList& asn );
    static void Delete( Common::MsgUnitList& asn );
    static void Delete( Common::MsgAutomatList& asn );
    static void Delete( Common::MsgPolygon& asn );
    static void Delete( Common::MsgPolygonList& asn );
    static void Delete( Common::MsgPointList& asn );
    static void Delete( Common::MsgPathList& asn );
    static void Delete( Common::MsgLocationList& asn );
    static void Delete( Common::MsgUnitKnowledgeList& asn );
    static void Delete( Common::MsgObjectKnowledgeList& asn );
    static void Delete( Common::MsgLogMedicalPriorities& asn );
    static void Delete( Common::MsgLogMaintenancePriorities& asn );
    static void Delete( Common::MsgCoordLatLongList& asn );
    //@}

    //! @name Decoding tools
    //@{
    static bool ReadLine( const Common::MsgLine& asn, TER_Localisation& localisation );
    static bool ReadLine( const Common::MsgLine& asn, T_PointVector& pointVector );
    static bool ReadPoint( const Common::MsgPoint& asn, MT_Vector2D& vPoint );
    static void ReadPoint( const Common::MsgCoordLatLong& asn, MT_Vector2D& vPoint );
    static bool ReadPointList( const Common::MsgPointList& asn, T_PointVector& pointVector );
    static bool ReadLocation( const Common::MsgLocation& asn, TER_Localisation& localisation );
    static void ReadDirection( const Common::MsgHeading& asn, MT_Vector2D& vDir );
    static void ReadGDH( const Common::MsgDateTime& asn, unsigned int& realTimeSec );
    static void ReadTick( const Common::MsgDateTime& asn, unsigned int& simTick );
    //@}

    //! @name Encoding tools
    //@{
    static bool WriteCoordinates( const T_PointVector& points, Common::MsgCoordLatLongList& asn );
    static bool WriteCoordinates( std::vector< boost::shared_ptr< MT_Vector2D > > points, Common::MsgCoordLatLongList& asn );
    static bool WriteLine( const T_PointVector& points, Common::MsgLine& asn );
    static void WriteLine( const TER_Localisation& localisation, Common::MsgLine& asn );
    static void WriteDirection( const MT_Vector2D& vDir, Common::MsgHeading& asn );
    static void WriteLocation( const TER_Localisation& localisation, Common::MsgLocation& asn );
    static void WritePoint( const MT_Vector2D& vPoint, Common::MsgPoint& asn );
    static void WritePoint( const MT_Vector2D& vPoint, Common::MsgCoordLatLong& asn );
    static void WritePointList( const T_PointList& points, Common::MsgPointList& asn );
    static void WriteEllipse( const MT_Ellipse& ellipse, Common::MsgLocation& asn );
    static void WritePath( const T_PointList& points, Common::MsgPath& asn );
    static void WritePath( const T_PointVector& points, Common::MsgPath& asn );
    static void WriteGenObject( const DEC_Gen_Object& object, Common::MsgPlannedWork& asn );
    static void WriteGDH( unsigned int nRealTimeSec, Common::MsgDateTime& asnGDH );
    static void WriteTick( unsigned int simTick, Common::MsgDateTime& asnGDH );
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
    static bool ReadPath( const Common::MsgPath& asn, T_PointVector& pointVector );
    static bool ReadPolygon( const Common::MsgPolygon& asn, TER_Localisation& localisation );

    static bool ReadLocationList( const Common::MsgLocationList& asn, T_LocalisationPtrVector& localisationVector, unsigned int nNbrEltMin = 0 );
    static bool ReadPolygonList( const Common::MsgPolygonList& asn, T_LocalisationPtrVector& localisationVector, unsigned int nNbrEltMin = 0 );
    static bool ReadPathList( const Common::MsgPathList& asn, T_ItinerairePtrVector& itineraireVector );

    static DEC_Decision_ABC* ReadAgent( const Common::MsgUnit& asn );
    static DEC_Decision_ABC* ReadAutomate( const Common::MsgUnit& asn );
    static boost::shared_ptr< DEC_Knowledge_Agent > ReadAgentKnowledge( const Common::MsgUnitKnowledge& asn, const DEC_KnowledgeResolver_ABC& resolver );
    static DEC_Knowledge_Population* ReadPopulationKnowledge( const Common::MsgPopulationKnowledge& asn, const DEC_KnowledgeResolver_ABC& resolver );

    static boost::shared_ptr< DEC_Knowledge_Object > ReadObjectKnowledge( const Common::MsgObjectKnowledge& asn, const DEC_KnowledgeResolver_ABC& resolver );
    static bool ReadObjectKnowledgeList( const Common::MsgObjectKnowledgeList& asn, T_KnowledgeObjectDiaIDVector& knowledgeList, const DEC_KnowledgeResolver_ABC& resolver );
    //@}

    // @name Encoding tools
    //@{
    static void WritePolygon( const TER_Localisation& localisation, Common::MsgPolygon& asn );
    static void WriteLocationList( const T_LocalisationPtrVector& localisationVector, Common::MsgLocationList& asn );
    static void WritePolygonList( const T_LocalisationPtrVector& localisationVector, Common::MsgPolygonList& asn );
    static void WritePathList( const T_ItinerairePtrVector& itineraireVector, Common::MsgPathList& asn );
    static void WritePointList( const T_PointVector& pointVector, Common::MsgPointList& asn );

    static void WriteAgent( const DEC_Decision_ABC& pion, Common::MsgUnit& asn );
    static void WriteAutomate( const DEC_Decision_ABC& automate, Common::MsgAutomat& asn );
    static void WriteAgentKnowledge( const DEC_Knowledge_Agent& knowledge, Common::MsgUnitKnowledge& asnKnowledge );
    static void WriteObjectKnowledge( const DEC_Knowledge_Object& knowledge, Common::MsgObjectKnowledge& asnKnowledge );
    static void WriteObjectKnowledgeList( const T_KnowledgeObjectDiaIDVector& knowledges, Common::MsgObjectKnowledgeList& listResult, const DEC_KnowledgeResolver_ABC& resolver );
    //@}
};

#endif // __NET_ASN_Tools_h_
