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

#include "MIL.h"

#include "game_asn/Simulation.h"
#include "Knowledge/DEC_Knowledge_Def.h"

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
    
    static bool CopyNatureAtlas ( int value, ASN1T_AtlasNature& asn );
    static bool CopyNatureAtlas ( const ASN1T_AtlasNature& asn, int& value );
    //@}

    //! @name Decoding tools
    //@{
    static bool ReadLine      ( const ASN1T_Line&         asn, TER_Localisation& localisation );
    static bool ReadLine      ( const ASN1T_Line&         asn, T_PointVector& pointVector );
    static bool ReadPoint     ( const ASN1T_Point&        asn, MT_Vector2D& vPoint );
    static void ReadPoint     ( const ASN1T_CoordLatLong&     asn, MT_Vector2D& vPoint );
    static bool ReadPointList ( const ASN1T_PointList&    asn, T_PointVector& pointVector );
    static bool ReadLocation  ( const ASN1T_Location&     asn, TER_Localisation& localisation );
    static void ReadDirection ( const ASN1T_Heading&      asn, MT_Vector2D& vDir );
    static void ReadGDH       ( const ASN1T_DateTime&     asn, uint& realTimeSec );
    static void ReadTick      ( const ASN1T_DateTime&     asn, uint& simTick );
    //@}

    //! @name Encoding tools
    //@{
    static bool WriteCoordinates( const T_PointVector&    points      , ASN1T_CoordLatLongList& asn );
    static bool WriteCoordinates( std::vector< boost::shared_ptr< MT_Vector2D > > points , ASN1T_CoordLatLongList& asn );
    static bool WriteLine       ( const T_PointVector&    points      , ASN1T_Line&         asn );
    static void WriteLine       ( const TER_Localisation& localisation, ASN1T_Line&         asn );
    static void WriteDirection  ( const MT_Vector2D&      vDir        , ASN1T_Heading&      asn );
    static void WriteLocation   ( const TER_Localisation& localisation, ASN1T_Location&     asn );
    static void WritePoint      ( const MT_Vector2D&      vPoint      , ASN1T_Point&        asn );
    static void WritePoint      ( const MT_Vector2D&      vPoint      , ASN1T_CoordLatLong&     asn );
    static void WritePointList  ( const T_PointList&      points      , ASN1T_PointList&    asn );
    static void WriteEllipse    ( const MT_Ellipse&       ellipse     , ASN1T_Location&     asn );
    static void WritePath       ( const T_PointList&      points      , ASN1T_Path&         asn );
    static void WritePath       ( const T_PointVector&    points      , ASN1T_Path&         asn );
    static void WriteGenObject  ( const DEC_Gen_Object&   object      , ASN1T_PlannedWork&  asn );
    static void WriteGDH        ( uint nRealTimeSec, ASN1T_DateTime& asnGDH );
    static void WriteTick       ( uint simTick, ASN1T_DateTime& asnGDH );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< T_PointVector* >          T_ItinerairePtrVector;
    typedef T_ItinerairePtrVector::iterator        IT_ItinerairePtrVector;
    typedef T_ItinerairePtrVector::const_iterator  CIT_ItinerairePtrVector;
    //@}

public:
    // @name Decoding tools
    //@{
    static bool                         ReadPath               ( const ASN1T_Path&   asn, T_PointVector& pointVector );
    static bool                         ReadPolygon            ( const ASN1T_Polygon&      asn, TER_Localisation& localisation );

    static bool                         ReadLocationList       ( const ASN1T_LocationList& asn, T_LocalisationPtrVector& localisationVector, uint nNbrEltMin = 0 );
    static bool                         ReadPolygonList        ( const ASN1T_PolygonList&      asn, T_LocalisationPtrVector& localisationVector, uint nNbrEltMin = 0 );
    static bool                         ReadPathList           ( const ASN1T_PathList&   asn, T_ItinerairePtrVector& itineraireVector );

    static DEC_Decision_ABC*            ReadAgent              ( const ASN1T_Unit&               asn );
    static bool                         ReadAgentList          ( const ASN1T_UnitList&           asn, T_ObjectVector& unitList );
    static DEC_Decision_ABC*            ReadAutomate           ( const ASN1T_Unit&               asn );
    static bool                         ReadAutomateList       ( const ASN1T_AutomatList&        asn, T_ObjectVector& unitList );
    static DEC_Knowledge_Agent*         ReadAgentKnowledge     ( const ASN1T_UnitKnowledge&      asn, const DEC_KnowledgeResolver_ABC& resolver );
    static DEC_Knowledge_Population*    ReadPopulationKnowledge( const ASN1T_PopulationKnowledge& asn, const DEC_KnowledgeResolver_ABC& resolver );

    static DEC_Knowledge_Object*        ReadObjectKnowledge    ( const ASN1T_ObjectKnowledge&     asn, const DEC_KnowledgeResolver_ABC& resolver );
    static bool                         ReadObjectKnowledgeList( const ASN1T_ObjectKnowledgeList& asn, T_KnowledgeObjectDiaIDVector& knowledgeList, const DEC_KnowledgeResolver_ABC& resolver );
    //@}
    
    // @name Encoding tools
    //@{
    static void WritePolygon            ( const TER_Localisation& localisation, ASN1T_Polygon&      asn );
    static void WriteLocationList       ( const T_LocalisationPtrVector& localisationVector, ASN1T_LocationList& asn );
    static void WritePolygonList        ( const T_LocalisationPtrVector& localisationVector, ASN1T_PolygonList&      asn );
    static void WritePathList           ( const T_ItinerairePtrVector& itineraireVector, ASN1T_PathList&   asn );
    static void WritePointList          ( const T_PointVector& pointVector, ASN1T_PointList& asn );

    static void WriteAgent              ( const DEC_Decision_ABC&             pion      , ASN1T_Unit&     asn );
    static void WriteAutomate           ( const DEC_Decision_ABC&             automate  , ASN1T_Automat&  asn );
    static void WriteAgentList          ( const T_ObjectVector&               unitList  , ASN1T_UnitList& asn );   
    static void WriteAutomateList       ( const T_ObjectVector&               unitList  , ASN1T_AutomatList& asn );   
    static void WriteAgentKnowledge     ( const DEC_Knowledge_Agent&          knowledge , ASN1T_UnitKnowledge&      asnKnowledge );
    static void WriteObjectKnowledge    ( const DEC_Knowledge_Object&         knowledge , ASN1T_ObjectKnowledge&     asnKnowledge );
    static void WriteObjectKnowledgeList( const T_KnowledgeObjectDiaIDVector& knowledges, ASN1T_ObjectKnowledgeList& asnListKnowledge, const DEC_KnowledgeResolver_ABC& resolver );
    //@}
};

#endif // __NET_ASN_Tools_h_
