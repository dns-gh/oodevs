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

#include "Network/NET_ASN_Types.h"
#include "Knowledge/DEC_Knowledge_Def.h"

class TER_Localisation;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class MT_Ellipse;
class DEC_AutomateDecision;
class DEC_RolePion_Decision;
class DEC_KS_KnowledgeGroupQuerier;

//=============================================================================
// Created: NLD 2003-02-28
// Last modified: JVT 04-03-25
//=============================================================================
class NET_ASN_Tools
{
public:
    //! @name Mission parameters tools : Knowledge object DIA - SIM - ASN
    //@{
    static bool CopyObjectKnowledge ( const ASN1T_KnowledgeObject& asn, DIA_Variable_ABC& dia, const DEC_KS_KnowledgeGroupQuerier& knowledge  );
    static bool CopyObjectKnowledge ( const DIA_Variable_ABC& dia, ASN1T_KnowledgeObject& asn, const DEC_KS_KnowledgeGroupQuerier& knowledge  );
    static void CopyObjectKnowledge ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetObjectKnowledge( DIA_Variable_ABC& dia );

    static bool CopyObjectKnowledgeList ( const ASN1T_ListKnowledgeObject& asn, DIA_Variable_ABC& dia, const DEC_KS_KnowledgeGroupQuerier& knowledge  );
    static bool CopyObjectKnowledgeList ( const DIA_Variable_ABC& dia, ASN1T_ListKnowledgeObject& asn, const DEC_KS_KnowledgeGroupQuerier& knowledge  );
    static void CopyObjectKnowledgeList ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetObjectKnowledgeList( DIA_Variable_ABC& dia );
    //@}

    //! @name Mission parameters tools : Knowledge agent DIA - SIM - ASN
    //@{
    static bool CopyAgentKnowledge ( const ASN1T_KnowledgeAgent& asn, DIA_Variable_ABC& dia, const DEC_KS_KnowledgeGroupQuerier& knowledge  );
    static bool CopyAgentKnowledge ( const DIA_Variable_ABC& dia, ASN1T_KnowledgeAgent& asn, const DEC_KS_KnowledgeGroupQuerier& knowledge  );
    static void CopyAgentKnowledge ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetAgentKnowledge( DIA_Variable_ABC& dia );

    static bool CopyAgentKnowledgeList ( const ASN1T_ListKnowledgeAgent& asn, DIA_Variable_ABC& dia, const DEC_KS_KnowledgeGroupQuerier& knowledge  );
    static bool CopyAgentKnowledgeList ( const DIA_Variable_ABC& dia, ASN1T_ListKnowledgeAgent& asn, const DEC_KS_KnowledgeGroupQuerier& knowledge  );
    static void CopyAgentKnowledgeList ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetAgentKnowledgeList( DIA_Variable_ABC& dia );
    //@}

    //! @name Mission parameters tools : Agent / Automate / Pion
    //@{
    static bool CopyAutomate ( const ASN1T_Agent& asn, DIA_Variable_ABC& dia, bool bValueIfOptional = true );
    static bool CopyAutomate ( const DIA_Variable_ABC& dia, ASN1T_Agent& asn );
    static bool CopyAutomate ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetAutomate( DIA_Variable_ABC& dia );

    static bool CopyAgent ( const ASN1T_Agent& asn, DIA_Variable_ABC& dia, bool bValueIfOptional = true );
    static bool CopyAgent ( const DIA_Variable_ABC& dia, ASN1T_Agent& asn );   
    static bool CopyAgent ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetAgent( DIA_Variable_ABC& dia );

    static bool CopyAgentList ( const ASN1T_ListAgent& asn, DIA_Variable_ABC& dia );
    static bool CopyAgentList ( const DIA_Variable_ABC& dia, ASN1T_ListAgent& asn );
    static bool CopyAgentList ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetAgentList( DIA_Variable_ABC& dia );

    static bool CopyAutomateList ( const ASN1T_ListAutomate& asn, DIA_Variable_ABC& dia );
    static bool CopyAutomateList ( const DIA_Variable_ABC& dia, ASN1T_ListAutomate& asn );
    static bool CopyAutomateList ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetAutomateList( DIA_Variable_ABC& dia );
    //@}

    //! @name Mission parameters tools : LOG specific
    //@{
    static bool CopyMaintenancePriorities( const ASN1T_MaintenancePriorites& asn, T_MaintenancePriorityVector& data, DIA_Variable_ABC& dia );    
    static bool CopyMaintenancePriorities( const DIA_Variable_ABC& diaFrom, T_MaintenancePriorityVector& container, DIA_Variable_ABC& diaTo );

    static bool CopyMedicalPriorities( const ASN1T_SantePriorites& asn, T_MedicalPriorityVector& data, DIA_Variable_ABC& dia );    
    static bool CopyMedicalPriorities( const DIA_Variable_ABC& diaFrom, T_MedicalPriorityVector& container, DIA_Variable_ABC& diaTo );
    //@}

    //! @name Mission parameters tools : GEN specific
    //@{
    static bool CopyGenObjectList ( const ASN1T_ListMissionGenObject& asn, DIA_Variable_ABC& dia );
    static bool CopyGenObjectList ( const DIA_Variable_ABC& dia, ASN1T_ListMissionGenObject& asn );
    static void ResetGenObjectList( DIA_Variable_ABC& dia );
    static bool CopyGenObject ( const ASN1T_MissionGenObject& asn, DIA_Variable_ABC& dia );
    static bool CopyGenObject ( const DIA_Variable_ABC& dia, ASN1T_MissionGenObject& asn );
    static void ResetGenObject( DIA_Variable_ABC& dia );
    //@}

    //! @name Mission parameters tools : Localisation/Geometry DIA - SIM - ASN
    //@{
    static bool CopyLocation ( const ASN1T_Localisation& asn, TER_Localisation& container, DIA_Variable_ABC& dia );  
    static bool CopyLocation ( const DIA_Variable_ABC& dia, ASN1T_Localisation& asn );
    static void CopyLocation ( const DIA_Variable_ABC& diaFrom, TER_Localisation& container, DIA_Variable_ABC& diaTo );
    static void ResetLocation( TER_Localisation& container, DIA_Variable_ABC& dia );

    static bool CopyLocationList  ( const ASN1T_ListLocalisation& asn, DIA_Variable_ABC& dia );
    static bool CopyLocationList  ( const DIA_Variable_ABC& dia, ASN1T_ListLocalisation& asn );
    static void CopyLocationList  ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetLocationList ( DIA_Variable_ABC& dia );

    static bool CopyPolygon ( const ASN1T_Polygon& asn, TER_Localisation& container, DIA_Variable_ABC& dia );  
    static bool CopyPolygon ( const DIA_Variable_ABC& dia, ASN1T_Polygon& asn );
    static void CopyPolygon ( const DIA_Variable_ABC& diaFrom, TER_Localisation& container, DIA_Variable_ABC& diaTo );
    static void ResetPolygon( TER_Localisation& container, DIA_Variable_ABC& dia );

    static bool CopyPolygonList  ( const ASN1T_ListPolygon& asn, DIA_Variable_ABC& dia );
    static bool CopyPolygonList  ( const DIA_Variable_ABC& dia, ASN1T_ListPolygon& asn );
    static void CopyPolygonList  ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetPolygonList ( DIA_Variable_ABC& dia );

    static bool CopyPoint ( const ASN1T_Point& asn, MT_Vector2D& container, DIA_Variable_ABC& dia, bool bValueIfOptional = true );
    static bool CopyPoint ( const DIA_Variable_ABC& dia, ASN1T_CoordUTM& asn );
    static bool CopyPoint ( const DIA_Variable_ABC& dia, ASN1T_Point& asn );
    static bool CopyPoint ( const DIA_Variable_ABC& diaFrom, MT_Vector2D& container, DIA_Variable_ABC& diaTo );
    static void ResetPoint( MT_Vector2D& container, DIA_Variable_ABC& dia );
    
    static bool CopyPointList ( const ASN1T_ListPoint& asn, T_PointVector& container, DIA_Variable_ABC& dia );
    static bool CopyPointList ( const DIA_Variable_ABC& dia, ASN1T_ListPoint& asn );
    static bool CopyPointList ( const DIA_Variable_ABC& diaFrom, T_PointVector& container, DIA_Variable_ABC& diaTo );
    static void ResetPointList( T_PointVector& container, DIA_Variable_ABC& dia );

    static bool CopyPath ( const ASN1T_Itineraire& asn, T_PointVector& container, DIA_Variable_ABC& dia );
    static bool CopyPath ( const DIA_Variable_ABC& dia, ASN1T_Itineraire& asn );
    static bool CopyPath ( const DIA_Variable_ABC& diaFrom, T_PointVector& container, DIA_Variable_ABC& diaTo );
    static void ResetPath( T_PointVector& container, DIA_Variable_ABC& dia );

    static bool CopyPathList ( const ASN1T_ListItineraire& asn, DIA_Variable_ABC& dia );
    static bool CopyPathList ( const DIA_Variable_ABC& dia, ASN1T_ListItineraire& asn );
    static bool CopyPathList ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetPathList( DIA_Variable_ABC& dia );
    //@}

    //! @name Mission parameters tools : GDH DIA - SIM - ASN
    //@{
    static bool CopyGDH ( const ASN1T_GDH&        asn    , DIA_Variable_ABC& dia, bool bValueIfOptional = true );
    static bool CopyGDH ( const DIA_Variable_ABC& dia    , ASN1T_GDH& asn          );
    static bool CopyGDH ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetGDH( DIA_Variable_ABC& dia );
    //@}

    //! @name Mission parameters tools : basic types
    //@{
    static bool CopyDirection ( const ASN1T_Direction& asn, MT_Vector2D& container, DIA_Variable_ABC& dia );
    static bool CopyDirection ( const DIA_Variable_ABC& dia, ASN1T_Direction& asn );
    static bool CopyDirection ( const DIA_Variable_ABC& diaFrom, MT_Vector2D& container, DIA_Variable_ABC& diaTo );
    static void ResetDirection( MT_Vector2D& container, DIA_Variable_ABC& dia );

    static bool CopyBool ( const ASN1BOOL& asn, DIA_Variable_ABC& dia );
    static bool CopyBool ( const DIA_Variable_ABC& dia, ASN1BOOL& asn );
    static bool CopyBool ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetBool( DIA_Variable_ABC& dia );
    
    static bool CopyNumeric ( const ASN1UINT& asn, DIA_Variable_ABC& dia );
    static bool CopyNumeric ( const ASN1INT& asn , DIA_Variable_ABC& dia );
    static bool CopyNumeric ( const ASN1REAL& asn, DIA_Variable_ABC& dia );
    static bool CopyNumeric ( const DIA_Variable_ABC& dia, ASN1UINT& asn );
    static bool CopyNumeric ( const DIA_Variable_ABC& dia, ASN1INT& asn );
    static bool CopyNumeric ( const DIA_Variable_ABC& dia, ASN1REAL& asn );    
    static bool CopyNumeric ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetNumeric( DIA_Variable_ABC& dia );

    static bool CopyID ( const ASN1T_OID& asn, DIA_Variable_ABC& dia );
    static bool CopyID ( const DIA_Variable_ABC& dia, ASN1T_OID& asn );
    static bool CopyID ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetID( DIA_Variable_ABC& dia );

    static bool CopyNatureAtlas ( const ASN1T_NatureAtlas& asn, DIA_Variable_ABC& dia );
    static bool CopyNatureAtlas ( const DIA_Variable_ABC& dia, ASN1T_NatureAtlas& asn );
    static bool CopyNatureAtlas ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetNatureAtlas( DIA_Variable_ABC& dia );

    static bool CopyEnumeration ( uint asn, DIA_Variable_ABC& dia );
    template< typename T > static bool CopyEnumeration ( const DIA_Variable_ABC& dia, T& asn )
    {
        int nValue = dia.ToId();
        asn = (T)nValue;
        return true;
    }
    static bool CopyEnumeration ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo );
    static void ResetEnumeration( DIA_Variable_ABC& dia );
    //@}

    // @name Misc tools
    //@{
    static void Delete( ASN1T_ListAgent&            asn );
    static void Delete( ASN1T_ListAutomate&         asn );
    static void Delete( ASN1T_Polygon&              asn );
    static void Delete( ASN1T_ListPolygon&          asn );
    static void Delete( ASN1T_ListPoint&            asn );
    static void Delete( ASN1T_ListItineraire&       asn );
    static void Delete( ASN1T_ListLocalisation&     asn );
    static void Delete( ASN1T_ListKnowledgeAgent&   asn );
    static void Delete( ASN1T_ListKnowledgeObject&  asn );
    static void Delete( ASN1T_ListMissionGenObject& asn );
    static void Delete( ASN1T_MissionGenObject&     asn );
    //@}

    //! @name Decoding tools
    //@{
    static bool ReadPoint     ( const ASN1T_Point&        asn, MT_Vector2D& vPoint );
    static void ReadPoint     ( const ASN1T_CoordUTM& asn   , MT_Vector2D&      vPoint );
    static bool ReadLocation  ( const ASN1T_Localisation& asn, TER_Localisation& localisation );
    static void ReadDirection ( const ASN1T_Direction&    asn, MT_Vector2D& vDir );
    //@}

    //! @name Encoding tools
    //@{
    static void WriteDirection( const MT_Vector2D&      vDir        , ASN1T_Direction&    asn );
    static void WriteLocation ( const TER_Localisation& localisation, ASN1T_Localisation& asn );
    static void WritePoint    ( const MT_Vector2D&      vPoint      , ASN1T_Point&        asn );
    static void WritePoint    ( const MT_Vector2D&      vPoint      , ASN1T_CoordUTM&     asn );
    static void WritePointList( const T_PointList& points, ASN1T_ListPoint& asn );
    static void WriteEllipse  ( const MT_Ellipse&       ellipse     , ASN1T_Localisation& asn );
    static void WritePath     ( const T_PointList&      points      , ASN1T_Itineraire&   asn );
    static void WritePath     ( const T_PointVector&    points      , ASN1T_Itineraire&   asn );
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
    static bool                  ReadLine      ( const ASN1T_Line&         asn, TER_Localisation& localisation );
    static bool                  ReadPath      ( const ASN1T_Itineraire&   asn, T_PointVector& pointVector );
    static bool                  ReadPolygon   ( const ASN1T_Polygon&      asn, TER_Localisation& localisation );

    static bool                  ReadLocationList  ( const ASN1T_ListLocalisation& asn, T_LocalisationPtrVector& localisationVector, uint nNbrEltMin = 0 );
    static bool                  ReadPolygonList   ( const ASN1T_ListPolygon&      asn, T_LocalisationPtrVector& localisationVector, uint nNbrEltMin = 0 );
    static bool                  ReadPointList     ( const ASN1T_ListPoint&        asn, T_PointVector& pointVector );
    static bool                  ReadPathList      ( const ASN1T_ListItineraire&   asn, T_ItinerairePtrVector& itineraireVector );

    static DEC_RolePion_Decision*  ReadAgent              ( const ASN1T_Agent&               asn );
    static bool                    ReadAgentList          ( const ASN1T_ListAgent&           asn, T_ObjectVector& unitList );
    static DEC_AutomateDecision*   ReadAutomate           ( const ASN1T_Agent&               asn );
    static bool                    ReadAutomateList       ( const ASN1T_ListAutomate&        asn, T_ObjectVector& unitList );
    static DEC_Knowledge_Agent*    ReadAgentKnowledge     ( const ASN1T_KnowledgeAgent&      asn, const DEC_KS_KnowledgeGroupQuerier& knowledge );
    static bool                    ReadAgentKnowledgeList ( const ASN1T_ListKnowledgeAgent&  asn, T_KnowledgeAgentDiaIDVector& knowledgeList, const DEC_KS_KnowledgeGroupQuerier& knowledge );
    static DEC_Knowledge_Object*   ReadObjectKnowledge    ( const ASN1T_KnowledgeObject&     asn, const DEC_KS_KnowledgeGroupQuerier& knowledge );
    static bool                    ReadObjectKnowledgeList( const ASN1T_ListKnowledgeObject& asn, T_KnowledgeObjectDiaIDVector& knowledgeList, const DEC_KS_KnowledgeGroupQuerier& knowledge );
    static uint                    ReadGDH                ( const ASN1T_GDH&                 asn );
    //@}
    
    // @name Encoding tools
    //@{
    static void WriteLine               ( const TER_Localisation& localisation, ASN1T_Line&         asn );
    static void WritePolygon            ( const TER_Localisation& localisation, ASN1T_Polygon&      asn );

    static void WriteLocationList       ( const T_LocalisationPtrVector& localisationVector, ASN1T_ListLocalisation& asn );
    static void WritePolygonList        ( const T_LocalisationPtrVector& localisationVector, ASN1T_ListPolygon&      asn );
    static void WritePathList           ( const T_ItinerairePtrVector& itineraireVector, ASN1T_ListItineraire&   asn );
    static void WritePointList          ( const T_PointVector& pointVector, ASN1T_ListPoint& asn );

    static void WriteAgent              ( const DEC_RolePion_Decision&        pion      , ASN1T_Agent&     asn );
    static void WriteAutomate           ( const DEC_AutomateDecision&         automate  , ASN1T_Agent&     asn );
    static void WriteAgentList          ( const T_ObjectVector&               unitList  , ASN1T_ListAgent& asn );   
    static void WriteAutomateList       ( const T_ObjectVector&               unitList  , ASN1T_ListAutomate& asn );   
    static void WriteAgentKnowledge     ( const DEC_Knowledge_Agent&          knowledge , ASN1T_KnowledgeAgent&      asnKnowledge );
    static void WriteObjectKnowledge    ( const DEC_Knowledge_Object&         knowledge , ASN1T_KnowledgeObject&     asnKnowledge );
    static void WriteAgentKnowledgeList ( const T_KnowledgeAgentDiaIDVector&  knowledges, ASN1T_ListKnowledgeAgent&  asnListKnowledge, const DEC_KS_KnowledgeGroupQuerier& knowledge );
    static void WriteObjectKnowledgeList( const T_KnowledgeObjectDiaIDVector& knowledges, ASN1T_ListKnowledgeObject& asnListKnowledge, const DEC_KS_KnowledgeGroupQuerier& knowledge );
    
    static void WriteGDH                ( ASN1T_GDH& asnGDH );
    static void WriteGDH                ( uint nRealTimeSec, ASN1T_GDH& asnGDH );
    //@}
};

#include "NET_ASN_Tools.inl"

#endif // __NET_ASN_Tools_h_
