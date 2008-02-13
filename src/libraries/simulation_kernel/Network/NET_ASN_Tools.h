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

#include "game_asn/Asn.h"
#include "Knowledge/DEC_Knowledge_Def.h"

class TER_Localisation;
class DEC_Gen_Object;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_Population;
class MT_Ellipse;
class DEC_AutomateDecision;
class DEC_RolePion_Decision;
class DEC_KnowledgeResolver_ABC;

//=============================================================================
// Created: NLD 2003-02-28
// Last modified: JVT 04-03-25
//=============================================================================
class NET_ASN_Tools
{
public:
    //! @name Mission parameters tools : Knowledge object DIA - SIM - ASN
    //@{
    static bool CopyObjectKnowledge ( const ASN1T_ObjectKnowledge& asn, DIA_Variable_ABC& dia, const DEC_KnowledgeResolver_ABC& resolver  );
    static bool CopyObjectKnowledge ( const DIA_Variable_ABC& dia, ASN1T_ObjectKnowledge& asn, const DEC_KnowledgeResolver_ABC& resolver  );
    static void CopyObjectKnowledge ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyObjectKnowledgeList ( const ASN1T_ObjectKnowledgeList& asn, DIA_Variable_ABC& dia, const DEC_KnowledgeResolver_ABC& resolver  );
    static bool CopyObjectKnowledgeList ( const DIA_Variable_ABC& dia, ASN1T_ObjectKnowledgeList& asn, const DEC_KnowledgeResolver_ABC& resolver  );
    static void CopyObjectKnowledgeList ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    //@}

    //! @name Mission parameters tools : Knowledge agent DIA - SIM - ASN
    //@{
    static bool CopyAgentKnowledge ( const ASN1T_UnitKnowledge& asn, DIA_Variable_ABC& dia, const DEC_KnowledgeResolver_ABC& knowledge  );
    static bool CopyAgentKnowledge ( const DIA_Variable_ABC& dia, ASN1T_UnitKnowledge& asn, const DEC_KnowledgeResolver_ABC& knowledge  );
    static void CopyAgentKnowledge ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyAgentKnowledgeList ( const ASN1T_UnitKnowledgeList& asn, DIA_Variable_ABC& dia, const DEC_KnowledgeResolver_ABC& knowledge  );
    static bool CopyAgentKnowledgeList ( const DIA_Variable_ABC& dia, ASN1T_UnitKnowledgeList& asn, const DEC_KnowledgeResolver_ABC& knowledge  );
    static void CopyAgentKnowledgeList ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    //@}

    //! @name Mission parameters tools : Knowledge Population DIA - SIM - ASN
    //@{
    static bool CopyPopulationKnowledge ( const ASN1T_PopulationKnowledge& asn, DIA_Variable_ABC& dia, const DEC_KnowledgeResolver_ABC& resolver  );
    static bool CopyPopulationKnowledge ( const DIA_Variable_ABC& dia, ASN1T_PopulationKnowledge& asn, const DEC_KnowledgeResolver_ABC& resolver  );
    static void CopyPopulationKnowledge ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    //@}

    //! @name Mission parameters tools : Population object knowldege DIA - SIM - ASN
    //@{
    /*    
    static bool CopyPopulationObjectKnowledge ( const ASN1T_Objet& asn, DIA_Variable_ABC& dia );
    static bool CopyPopulationObjectKnowledge ( const DIA_Variable_ABC& dia, ASN1T_Objet& asn );
    static void CopyPopulationObjectKnowledge ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    */
    //@}

    //! @name Mission parameters tools : Agent / Automate / Pion
    //@{
    static bool CopyAutomate ( const ASN1T_Unit& asn, DIA_Variable_ABC& dia );
    static bool CopyAutomate ( const DIA_Variable_ABC& dia, ASN1T_Unit& asn );
    static bool CopyAutomate ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyAgent ( const ASN1T_Unit& asn, DIA_Variable_ABC& dia );
    static bool CopyAgent ( const DIA_Variable_ABC& dia, ASN1T_Unit& asn );   
    static bool CopyAgent ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyAgentList ( const ASN1T_UnitList& asn, DIA_Variable_ABC& dia );
    static bool CopyAgentList ( const DIA_Variable_ABC& dia, ASN1T_UnitList& asn );
    static bool CopyAgentList ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyAutomateList ( const ASN1T_AutomatList& asn, DIA_Variable_ABC& dia );
    static bool CopyAutomateList ( const DIA_Variable_ABC& dia, ASN1T_AutomatList& asn );
    static bool CopyAutomateList ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    //@}

    //! @name Mission parameters tools : LOG specific
    //@{
    static bool CopyMaintenancePriorities( const ASN1T_LogMaintenancePriorities& asn, DIA_Variable_ABC& dia );
    static bool CopyMaintenancePriorities( const DIA_Variable_ABC& dia, ASN1T_LogMaintenancePriorities& asn );    
    static bool CopyMaintenancePriorities( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyMedicalPriorities( const ASN1T_LogMedicalPriorities& asn, DIA_Variable_ABC& dia );    
    static bool CopyMedicalPriorities( const DIA_Variable_ABC& dia, ASN1T_LogMedicalPriorities& asn );    
    static bool CopyMedicalPriorities( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    //@}

    //! @name Mission parameters tools : GEN specific
    //@{
    static void CopyGenObjectList( const ASN1T_PlannedWorkList& asn, DIA_Variable_ABC& dia );
    static bool CopyGenObjectList( const DIA_Variable_ABC& dia, ASN1T_PlannedWorkList& asn );
    static void CopyGenObjectList( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static void CopyGenObject( const ASN1T_PlannedWork& asn, DIA_Variable_ABC& dia );
    static bool CopyGenObject( const DIA_Variable_ABC& dia, ASN1T_PlannedWork& asn );
    static void CopyGenObject( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    //@}

    //! @name Mission parameters tools : Objectives
    //@{
    static void CopyObjectiveList( const ASN1T_MissionObjectiveList& asn, DIA_Variable_ABC& dia );
    static bool CopyObjectiveList( const DIA_Variable_ABC& dia, ASN1T_MissionObjectiveList& asn );
    static void CopyObjectiveList( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static void CopyObjective( const ASN1T_MissionObjective& asn, DIA_Variable_ABC& dia );
    static bool CopyObjective( const DIA_Variable_ABC& dia, ASN1T_MissionObjective& asn );
    static void CopyObjective( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    //@}

    //! @name Mission parameters tools : Localisation/Geometry DIA - SIM - ASN
    //@{
    static bool CopyLocation ( const ASN1T_Location& asn, DIA_Variable_ABC& dia );  
    static bool CopyLocation ( const DIA_Variable_ABC& dia, ASN1T_Location& asn );
    static void CopyLocation ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyLocationList  ( const ASN1T_LocationList& asn, DIA_Variable_ABC& dia );
    static bool CopyLocationList  ( const DIA_Variable_ABC& dia, ASN1T_LocationList& asn );
    static void CopyLocationList  ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyPolygon ( const ASN1T_Polygon& asn, DIA_Variable_ABC& dia );  
    static bool CopyPolygon ( const DIA_Variable_ABC& dia, ASN1T_Polygon& asn );
    static void CopyPolygon ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyPolygonList  ( const ASN1T_PolygonList& asn, DIA_Variable_ABC& dia );
    static bool CopyPolygonList  ( const DIA_Variable_ABC& dia, ASN1T_PolygonList& asn );
    static void CopyPolygonList  ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyPoint ( const ASN1T_Point& asn, DIA_Variable_ABC& dia, bool bNullValue = false );
    static bool CopyPoint ( const DIA_Variable_ABC& dia, ASN1T_CoordUTM& asn );
    static bool CopyPoint ( const DIA_Variable_ABC& dia, ASN1T_Point& asn );
    static bool CopyPoint ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );    
    
    static bool CopyPointList ( const ASN1T_PointList& asn, DIA_Variable_ABC& dia );
    static bool CopyPointList ( const DIA_Variable_ABC& dia, ASN1T_PointList& asn );
    static bool CopyPointList ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyPath ( const ASN1T_Path& asn, DIA_Variable_ABC& dia );
    static bool CopyPath ( const DIA_Variable_ABC& dia, ASN1T_Path& asn );
    static bool CopyPath ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyPathList ( const ASN1T_PathList& asn, DIA_Variable_ABC& dia );
    static bool CopyPathList ( const DIA_Variable_ABC& dia, ASN1T_PathList& asn );
    static bool CopyPathList ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    //@}

    //! @name Mission parameters tools : GDH DIA - SIM - ASN
    //@{
    static bool CopyGDH ( const ASN1T_DateTime&        asn    , DIA_Variable_ABC& dia );
    static bool CopyGDH ( const DIA_Variable_ABC& dia    , ASN1T_DateTime& asn          );
    static bool CopyGDH ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    //@}

    //! @name Mission parameters tools : basic types
    //@{
    static bool CopyDirection ( const ASN1T_Heading& asn, DIA_Variable_ABC& dia );
    static bool CopyDirection ( const DIA_Variable_ABC& dia, ASN1T_Heading& asn );
    static bool CopyDirection ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyBool ( const ASN1BOOL& asn, DIA_Variable_ABC& dia );
    static bool CopyBool ( const DIA_Variable_ABC& dia, ASN1BOOL& asn );
    static bool CopyBool ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyString ( const ASN1VisibleString& asn, DIA_Variable_ABC& dia );
    static bool CopyString ( const DIA_Variable_ABC& dia, ASN1VisibleString& asn );
    static bool CopyString ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    
    static bool CopyNumeric ( const ASN1UINT& asn, DIA_Variable_ABC& dia );
    static bool CopyNumeric ( const ASN1INT& asn , DIA_Variable_ABC& dia );
    static bool CopyNumeric ( const ASN1REAL& asn, DIA_Variable_ABC& dia );
    static bool CopyNumeric ( const DIA_Variable_ABC& dia, ASN1UINT& asn );
    static bool CopyNumeric ( const DIA_Variable_ABC& dia, ASN1INT& asn );
    static bool CopyNumeric ( const DIA_Variable_ABC& dia, ASN1REAL& asn );    
    static bool CopyNumeric ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyNatureAtlas ( const ASN1T_AtlasNature& asn, DIA_Variable_ABC& dia );
    static bool CopyNatureAtlas ( const DIA_Variable_ABC& dia, ASN1T_AtlasNature& asn );
    static bool CopyNatureAtlas ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyDotationType( const ASN1T_DotationType& asn, DIA_Variable_ABC& dia );
    static bool CopyDotationType( const DIA_Variable_ABC& dia, ASN1T_DotationType& asn );
    static bool CopyDotationType( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyEquipmentType( const ASN1T_EquipmentType& asn, DIA_Variable_ABC& dia );
    static bool CopyEquipmentType( const DIA_Variable_ABC& dia, ASN1T_EquipmentType& asn );
    static bool CopyEquipmentType( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyIndirectFire( const DIA_Variable_ABC& dia, ASN1T_UnitFire& asn );
    static bool CopyIndirectFire( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );

    static bool CopyEnumeration ( uint asn, DIA_Variable_ABC& dia );
    template< typename T > static bool CopyEnumeration ( const DIA_Variable_ABC& dia, T& asn )
    {
        int nValue = dia.ToId();
        asn = (T)nValue;
        return true;
    }
    static bool CopyEnumeration ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    //@}

    // @name Misc tools
    //@{
    static void Delete( ASN1T_MissionObjective&         asn );
    static void Delete( ASN1T_MissionObjectiveList&     asn );
    static void Delete( ASN1T_PlannedWork&              asn );
    static void Delete( ASN1T_PlannedWorkList&          asn );
    static void Delete( ASN1T_UnitList&                 asn );
    static void Delete( ASN1T_AutomatList&              asn );
    static void Delete( ASN1T_Polygon&                  asn );
    static void Delete( ASN1T_PolygonList&              asn );
    static void Delete( ASN1T_PointList&                asn );
    static void Delete( ASN1T_PathList&                 asn );
    static void Delete( ASN1T_LocationList&             asn );
    static void Delete( ASN1T_UnitKnowledgeList&        asn );
    static void Delete( ASN1T_ObjectKnowledgeList&      asn );
    static void Delete( ASN1T_LogMedicalPriorities&     asn );
    static void Delete( ASN1T_LogMaintenancePriorities& asn );
    static void Delete( ASN1T_CoordUTMList& asn );
    //@}

    //! @name Decoding tools
    //@{
    static bool ReadLine      ( const ASN1T_Line&         asn, TER_Localisation& localisation );
    static bool ReadLine      ( const ASN1T_Line&         asn, T_PointVector& pointVector );
    static bool ReadPoint     ( const ASN1T_Point&        asn, MT_Vector2D& vPoint );
    static void ReadPoint     ( const ASN1T_CoordUTM&     asn, MT_Vector2D& vPoint );
    static bool ReadPointList ( const ASN1T_PointList&    asn, T_PointVector& pointVector );
    static bool ReadLocation  ( const ASN1T_Location&     asn, TER_Localisation& localisation );
    static void ReadDirection ( const ASN1T_Heading&      asn, MT_Vector2D& vDir );
    static void ReadTick      ( const ASN1T_DateTime&     asn, uint& simTick );
    //@}

    //! @name Encoding tools
    //@{
    static bool WriteCoordinates( const T_PointVector&    points      , ASN1T_CoordUTMList& asn );
    static bool WriteLine       ( const T_PointVector&    points      , ASN1T_Line&         asn );
    static void WriteLine       ( const TER_Localisation& localisation, ASN1T_Line&         asn );
    static void WriteDirection  ( const MT_Vector2D&      vDir        , ASN1T_Heading&      asn );
    static void WriteLocation   ( const TER_Localisation& localisation, ASN1T_Location&     asn );
    static void WritePoint      ( const MT_Vector2D&      vPoint      , ASN1T_Point&        asn );
    static void WritePoint      ( const MT_Vector2D&      vPoint      , ASN1T_CoordUTM&     asn );
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

private:
    // @name Decoding tools
    //@{
    static bool                         ReadPath               ( const ASN1T_Path&   asn, T_PointVector& pointVector );
    static bool                         ReadPolygon            ( const ASN1T_Polygon&      asn, TER_Localisation& localisation );

    static bool                         ReadLocationList       ( const ASN1T_LocationList& asn, T_LocalisationPtrVector& localisationVector, uint nNbrEltMin = 0 );
    static bool                         ReadPolygonList        ( const ASN1T_PolygonList&      asn, T_LocalisationPtrVector& localisationVector, uint nNbrEltMin = 0 );
    static bool                         ReadPathList           ( const ASN1T_PathList&   asn, T_ItinerairePtrVector& itineraireVector );

    static DEC_RolePion_Decision*       ReadAgent              ( const ASN1T_Unit&               asn );
    static bool                         ReadAgentList          ( const ASN1T_UnitList&           asn, T_ObjectVector& unitList );
    static DEC_AutomateDecision*        ReadAutomate           ( const ASN1T_Unit&               asn );
    static bool                         ReadAutomateList       ( const ASN1T_AutomatList&        asn, T_ObjectVector& unitList );
    static DEC_Knowledge_Agent*         ReadAgentKnowledge     ( const ASN1T_UnitKnowledge&      asn, const DEC_KnowledgeResolver_ABC& resolver );
    static bool                         ReadAgentKnowledgeList ( const ASN1T_UnitKnowledgeList&  asn, T_KnowledgeAgentDiaIDVector& knowledgeList, const DEC_KnowledgeResolver_ABC& resolver );
    static DEC_Knowledge_Population*    ReadPopulationKnowledge( const ASN1T_PopulationKnowledge& asn, const DEC_KnowledgeResolver_ABC& resolver );

    static DEC_Knowledge_Object*        ReadObjectKnowledge    ( const ASN1T_ObjectKnowledge&     asn, const DEC_KnowledgeResolver_ABC& resolver );
    static bool                         ReadObjectKnowledgeList( const ASN1T_ObjectKnowledgeList& asn, T_KnowledgeObjectDiaIDVector& knowledgeList, const DEC_KnowledgeResolver_ABC& resolver );
    static uint                         ReadGDH                ( const ASN1T_DateTime&                 asn );
    //@}
    
    // @name Encoding tools
    //@{
    static void WritePolygon            ( const TER_Localisation& localisation, ASN1T_Polygon&      asn );
    static void WriteLocationList       ( const T_LocalisationPtrVector& localisationVector, ASN1T_LocationList& asn );
    static void WritePolygonList        ( const T_LocalisationPtrVector& localisationVector, ASN1T_PolygonList&      asn );
    static void WritePathList           ( const T_ItinerairePtrVector& itineraireVector, ASN1T_PathList&   asn );
    static void WritePointList          ( const T_PointVector& pointVector, ASN1T_PointList& asn );

    static void WriteAgent              ( const DEC_RolePion_Decision&        pion      , ASN1T_Unit&     asn );
    static void WriteAutomate           ( const DEC_AutomateDecision&         automate  , ASN1T_Unit&     asn );
    static void WriteAgentList          ( const T_ObjectVector&               unitList  , ASN1T_UnitList& asn );   
    static void WriteAutomateList       ( const T_ObjectVector&               unitList  , ASN1T_AutomatList& asn );   
    static void WriteAgentKnowledge     ( const DEC_Knowledge_Agent&          knowledge , ASN1T_UnitKnowledge&      asnKnowledge );
    static void WriteObjectKnowledge    ( const DEC_Knowledge_Object&         knowledge , ASN1T_ObjectKnowledge&     asnKnowledge );
    static void WriteAgentKnowledgeList ( const T_KnowledgeAgentDiaIDVector&  knowledges, ASN1T_UnitKnowledgeList&  asnListKnowledge, const DEC_KnowledgeResolver_ABC& resolver );
    static void WriteObjectKnowledgeList( const T_KnowledgeObjectDiaIDVector& knowledges, ASN1T_ObjectKnowledgeList& asnListKnowledge, const DEC_KnowledgeResolver_ABC& resolver );
    //@}
};

#endif // __NET_ASN_Tools_h_
