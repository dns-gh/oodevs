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
    class MsgAtlasNature;
    class AutomatId;
    class AutomatIdList;
    class MsgCoordLatLong;
    class MsgCoordLatLongList;
    class MsgDateTime;
    class MsgHeading;
    class MsgLine;
    class MsgLocation;
    class MsgLocationList;
    class LogMaintenancePriorities;
    class LogMedicalPriorities;
    class MsgMissionObjective;
    class MsgMissionObjectiveList;
    class ObjectKnowledgeId;
    class ObjectKnowledgeIdList;
    class MsgPath;
    class MsgPathList;
    class MsgPlannedWork;
    class MsgPlannedWorkList;
    class MsgPoint;
    class MsgPointList;
    class MsgPolygon;
    class MsgPolygonList;
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

    static bool CopyNatureAtlas ( int value, sword::MsgAtlasNature& asn );
    static bool CopyNatureAtlas ( const sword::MsgAtlasNature& asn, int& value );
    //@}

    // @name Misc tools
    //@{
    static void Delete( sword::MsgMissionObjective& asn );
    static void Delete( sword::MsgMissionObjectiveList& asn );
    static void Delete( sword::MsgPlannedWork& asn );
    static void Delete( sword::MsgPlannedWorkList& asn );
    static void Delete( sword::UnitIdList& asn );
    static void Delete( sword::AutomatIdList& asn );
    static void Delete( sword::MsgPolygon& asn );
    static void Delete( sword::MsgPolygonList& asn );
    static void Delete( sword::MsgPointList& asn );
    static void Delete( sword::MsgPathList& asn );
    static void Delete( sword::MsgLocationList& asn );
    static void Delete( sword::UnitKnowledgeIdList& asn );
    static void Delete( sword::ObjectKnowledgeIdList& asn );
    static void Delete( sword::LogMedicalPriorities& asn );
    static void Delete( sword::LogMaintenancePriorities& asn );
    static void Delete( sword::MsgCoordLatLongList& asn );
    //@}

    //! @name Decoding tools
    //@{
    static bool ReadLine( const sword::MsgLine& asn, TER_Localisation& localisation );
    static bool ReadLine( const sword::MsgLine& asn, T_PointVector& pointVector );
    static bool ReadPoint( const sword::MsgPoint& asn, MT_Vector2D& vPoint );
    static void ReadPoint( const sword::MsgCoordLatLong& asn, MT_Vector2D& vPoint );
    static bool ReadPointList( const sword::MsgPointList& asn, T_PointVector& pointVector );
    static bool ReadLocation( const sword::MsgLocation& asn, TER_Localisation& localisation );
    static void ReadDirection( const sword::MsgHeading& asn, MT_Vector2D& vDir );
    static void ReadGDH( const sword::MsgDateTime& asn, unsigned int& realTimeSec );
    static void ReadTick( const sword::MsgDateTime& asn, unsigned int& simTick );
    //@}

    //! @name Encoding tools
    //@{
    static bool WriteCoordinates( const T_PointVector& points, sword::MsgCoordLatLongList& asn );
    static bool WriteCoordinates( std::vector< boost::shared_ptr< MT_Vector2D > > points, sword::MsgCoordLatLongList& asn );
    static bool WriteLine( const T_PointVector& points, sword::MsgLine& asn );
    static void WriteLine( const TER_Localisation& localisation, sword::MsgLine& asn );
    static void WriteDirection( const MT_Vector2D& vDir, sword::MsgHeading& asn );
    static void WriteLocation( const TER_Localisation& localisation, sword::MsgLocation& asn );
    static void WritePoint( const MT_Vector2D& vPoint, sword::MsgPoint& asn );
    static void WritePoint( const MT_Vector2D& vPoint, sword::MsgCoordLatLong& asn );
    static void WritePointList( const T_PointList& points, sword::MsgPointList& asn );
    static void WriteEllipse( const MT_Ellipse& ellipse, sword::MsgLocation& asn );
    static void WritePath( const T_PointList& points, sword::MsgPath& asn );
    static void WritePath( const T_PointVector& points, sword::MsgPath& asn );
    static void WriteGenObject( const DEC_Gen_Object& object, sword::MsgPlannedWork& asn );
    static void WriteGDH( unsigned int nRealTimeSec, sword::MsgDateTime& asnGDH );
    static void WriteTick( unsigned int simTick, sword::MsgDateTime& asnGDH );
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
    static bool ReadPath( const sword::MsgPath& asn, T_PointVector& pointVector );
    static bool ReadPolygon( const sword::MsgPolygon& asn, TER_Localisation& localisation );

    static bool ReadLocationList( const sword::MsgLocationList& asn, T_LocalisationPtrVector& localisationVector, unsigned int nNbrEltMin = 0 );
    static bool ReadPolygonList( const sword::MsgPolygonList& asn, T_LocalisationPtrVector& localisationVector, unsigned int nNbrEltMin = 0 );
    static bool ReadPathList( const sword::MsgPathList& asn, T_ItinerairePtrVector& itineraireVector );

    static DEC_Decision_ABC*            ReadAgent              ( const sword::UnitId&               asn );
    static DEC_Decision_ABC*            ReadAutomate           ( const sword::UnitId&               asn );
    static boost::shared_ptr< DEC_Knowledge_Agent > ReadAgentKnowledge     ( const sword::UnitKnowledgeId&      asn, const DEC_KnowledgeResolver_ABC& resolver );
    static DEC_Knowledge_Population*    ReadPopulationKnowledge( const sword::CrowdKnowledgeId& asn, const DEC_KnowledgeResolver_ABC& resolver );

    static boost::shared_ptr< DEC_Knowledge_Object > ReadObjectKnowledge    ( const sword::ObjectKnowledgeId&     asn, const DEC_KnowledgeResolver_ABC& resolver );
    static bool                         ReadObjectKnowledgeList( const sword::ObjectKnowledgeIdList& asn, T_KnowledgeObjectDiaIDVector& knowledgeList, const DEC_KnowledgeResolver_ABC& resolver );
    //@}

    // @name Encoding tools
    //@{
    static void WritePolygon( const TER_Localisation& localisation, sword::MsgPolygon& asn );
    static void WriteLocationList( const T_LocalisationPtrVector& localisationVector, sword::MsgLocationList& asn );
    static void WritePolygonList( const T_LocalisationPtrVector& localisationVector, sword::MsgPolygonList& asn );
    static void WritePathList( const T_ItinerairePtrVector& itineraireVector, sword::MsgPathList& asn );
    static void WritePointList( const T_PointVector& pointVector, sword::MsgPointList& asn );

    static void WriteAgent              ( const DEC_Decision_ABC&             pion      , sword::UnitId&     asn );
    static void WriteAutomate           ( const DEC_Decision_ABC&             automate  , sword::AutomatId&  asn );
    static void WriteAgentKnowledge     ( const DEC_Knowledge_Agent&          knowledge , sword::UnitKnowledgeId&      asnKnowledge );
    static void WriteObjectKnowledge    ( const DEC_Knowledge_Object&         knowledge , sword::ObjectKnowledgeId&     asnKnowledge );
    static void WriteObjectKnowledgeList( const T_KnowledgeObjectDiaIDVector& knowledges, sword::ObjectKnowledgeIdList& listResult, const DEC_KnowledgeResolver_ABC& resolver );
    //@}
};

#endif // __NET_ASN_Tools_h_
