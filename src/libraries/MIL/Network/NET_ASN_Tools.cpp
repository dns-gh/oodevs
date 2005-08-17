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

#include "MIL_pch.h"

#include "NET_ASN_Tools.h"

#include "Tools/MIL_Tools.h"
#include "MIL_AgentServer.h"
#include "TER/TER_Localisation.h"
#include "MT_Tools/MT_Ellipse.h"

#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"

#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KS_AgentQuerier.h"
#include "Knowledge/DEC_KS_KnowledgeGroupQuerier.h"

#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Automates/MIL_Automate.h"

#include "Decision/Genie/DEC_Gen_Object.h"
#include "Decision/DEC_Tools.h"

// Time for MOS ...
#define MIL_NBR_DAYS_BETWEEN_1901_AND_1970  (uint)25202
#define MIL_NBR_SEC_BETWEEN_1901_AND_1970  ( MIL_NBR_DAYS_BETWEEN_1901_AND_1970 * 86400 )

//=============================================================================
// PARAMETERS TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadDirection
// Created: AGN 03-02-04
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::ReadDirection( const ASN1T_Direction& asnDir, MT_Vector2D& vDir )
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
bool NET_ASN_Tools::ReadLocation( const ASN1T_Localisation& asnLocalisation, TER_Localisation& localisation )
{
    //$$$$ ACCEDER DIRECTEMENT A LA LOCALISATION (EVITER COPIE)
    T_PointVector pointVector;

    for( uint i = 0; i < asnLocalisation.vecteur_point.n; ++i )
    {
        MT_Vector2D vPos;
        NET_ASN_Tools::ReadPoint( asnLocalisation.vecteur_point.elem[i], vPos );
        pointVector.push_back( vPos );
    }

    return localisation.Reset( (TER_Localisation::E_TypeLocalisation)asnLocalisation.type, pointVector );
}


//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadPoint
// Created: AGN 03-01-20
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::ReadPoint( const ASN1T_CoordUTM& asnPoint, MT_Vector2D& vect )
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
    if( asnLocalisation.type != EnumTypeLocalisation::point )
        return false;
    if( asnLocalisation.vecteur_point.n != 1 )
        return false;

    ReadPoint( asnLocalisation.vecteur_point.elem[0], vect );
    return true;
}


// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadLine
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadLine( const ASN1T_Line& asn, TER_Localisation& localisation )
{
    if( asn.type != EnumTypeLocalisation::line || asn.vecteur_point.n < 2 )
        return false;
    return ReadLocation( asn, localisation );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadPath
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadPath( const ASN1T_Line& asn, T_PointVector& itineraire )
{
    if( asn.type != EnumTypeLocalisation::line || asn.vecteur_point.n < 1 )
        return false;

    itineraire.clear();
    for( uint i = 0; i < asn.vecteur_point.n; ++i )
    {
        MT_Vector2D vPos;
        ReadPoint( asn.vecteur_point.elem[i], vPos );
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
    if( asn.type != EnumTypeLocalisation::polygon || asn.vecteur_point.n < 3 )
        return false;
    return ReadLocation( asn, localisation );
}


//---

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadListLocalisation
// Created: NLD 2003-08-11
// -----------------------------------------------------------------------------
// static
bool NET_ASN_Tools::ReadLocationList( const ASN1T_ListLocalisation& asnListLocalisation, T_LocalisationPtrVector& localisationVector, uint nNbrEltMin )
{
    localisationVector.clear();
    for( uint i = 0; i < asnListLocalisation.n; ++i )
    {
        TER_Localisation* pLocalisation = new TER_Localisation(); //$$$ RAM
        if( !ReadLocation( asnListLocalisation.elem[i], *pLocalisation ) )
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
bool NET_ASN_Tools::ReadPolygonList( const ASN1T_ListPolygon& asn, T_LocalisationPtrVector& localisationVector, uint nNbrEltMin )
{
    for( uint i = 0; i < asn.n; ++i )
    {
        if( asn.elem[i].type != EnumTypeLocalisation::polygon || asn.elem[i].vecteur_point.n < 3 )
            return false;
    }
    
    localisationVector.clear();
    for( i = 0; i < asn.n; ++i )
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
bool NET_ASN_Tools::ReadPointList( const ASN1T_ListPoint& asn, T_PointVector& pointVector )
{
	pointVector.clear();

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
bool NET_ASN_Tools::ReadPathList( const ASN1T_ListItineraire&   asn, T_ItinerairePtrVector& itineraireVector )
{
    itineraireVector.clear();
    for( uint i = 0; i < asn.n; ++i )
    {
        T_PointVector* pItineraire = new T_PointVector(); //$$$ RAM
        if( ReadPath( asn.elem[i], *pItineraire ) )
            itineraireVector.push_back( pItineraire );
        else
        {
            MT_DELETEOWNED( itineraireVector );
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
DEC_RolePion_Decision* NET_ASN_Tools::ReadAgent( const ASN1T_Agent& asnAgent )
{
    MIL_AgentPion* pPion = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( asnAgent );
    if( !pPion )
        return 0;
    return &pPion->GetRole< DEC_RolePion_Decision >();
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadListAgent
// Created: AGN 03-01-20
//-----------------------------------------------------------------------------
// static
bool NET_ASN_Tools::ReadAgentList( const ASN1T_ListAgent& asnListAgent, T_ObjectVector& unitList )
{
    unitList.clear();

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
DEC_AutomateDecision* NET_ASN_Tools::ReadAutomate( const ASN1T_Agent& asnAgent )
{
    MIL_Automate* pAutomate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( asnAgent );
    if( !pAutomate ) 
        return 0;
    return &pAutomate->GetDecision();
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadListAutomate
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadAutomateList( const ASN1T_ListAutomate& asnListAgent, T_ObjectVector& unitList )
{
    unitList.clear();

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
DEC_Knowledge_Agent* NET_ASN_Tools::ReadAgentKnowledge( const ASN1T_KnowledgeAgent& asnAgent, const DEC_KS_KnowledgeGroupQuerier& knowledge )
{
    return knowledge.GetKnowledgeAgentFromMosID( asnAgent );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadListKnowledgeAgent
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadAgentKnowledgeList( const ASN1T_ListKnowledgeAgent& asnListAgent, T_KnowledgeAgentDiaIDVector& knowledges, const DEC_KS_KnowledgeGroupQuerier& knowledge )
{
    knowledges.clear();
    for( uint n = 0; n < asnListAgent.n; ++n )
    {        
        DEC_Knowledge_Agent* pKnowledge = ReadAgentKnowledge( asnListAgent.elem[n], knowledge );
        if( pKnowledge == 0 )
            return false;
        knowledges.push_back( (void*)pKnowledge->GetDiaID() );
    }
    return true;        
}


//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadObjectKnowledge
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
// static
DEC_Knowledge_Object* NET_ASN_Tools::ReadObjectKnowledge( const ASN1T_KnowledgeObject& asnObject, const DEC_KS_KnowledgeGroupQuerier& knowledge )
{
    return knowledge.GetKnowledgeObjectFromMosID( asnObject );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadListKnowledgeObject
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::ReadObjectKnowledgeList( const ASN1T_ListKnowledgeObject& asnListObject, T_KnowledgeObjectDiaIDVector& knowledges, const DEC_KS_KnowledgeGroupQuerier& knowledge )
{
    knowledges.clear();
    for( uint n = 0; n < asnListObject.n; ++n )
    {        
        DEC_Knowledge_Object* pKnowledge = ReadObjectKnowledge( asnListObject.elem[n], knowledge );
        if( !pKnowledge )
            return false;
        knowledges.push_back( (void*)pKnowledge->GetDiaID() );
    }
    return true;        
}
     
// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ReadGDH
// Created: NLD 2004-01-15
// -----------------------------------------------------------------------------
uint NET_ASN_Tools::ReadGDH( const ASN1T_GDH& asnGDH )
{
    return asnGDH.datation - MIL_NBR_SEC_BETWEEN_1901_AND_1970;
}

//=============================================================================
// ENCODING TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteDirection
// Created: NLD 2003-05-13
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WriteDirection( const MT_Vector2D& vDir, ASN1T_Direction& asnDir )
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
void NET_ASN_Tools::WriteLocation( const TER_Localisation& localisation, ASN1T_Localisation& asnLocalisation )
{
    assert( localisation.GetType() != TER_Localisation::eNone );
    asnLocalisation.type            = (ASN1T_EnumTypeLocalisation)localisation.GetType();
    asnLocalisation.vecteur_point.n = localisation.GetPoints().size(); 
    if( localisation.GetPoints().empty() )
        return;

    ASN1T_CoordUTM* pCoord = new ASN1T_CoordUTM[ localisation.GetPoints().size() ]; //$$$ RAM
    asnLocalisation.vecteur_point.elem = pCoord;
    uint i = 0;
    for( CIT_PointVector itPoint = localisation.GetPoints().begin(); itPoint != localisation.GetPoints().end(); ++itPoint )
        WritePoint( *itPoint, pCoord[i++] );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WritePoint
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WritePoint( const MT_Vector2D& vPos, ASN1T_CoordUTM& asnCoordUTM )
{
    static std::string strPos;
    MIL_Tools::ConvertCoordSimToMos( vPos, strPos );
    asnCoordUTM = strPos.c_str();
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WritePoint
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WritePoint( const MT_Vector2D& vPos, ASN1T_Point& asnLocalisation )
{
    ASN1T_CoordUTM* pCoord = new ASN1T_CoordUTM[1]; //$$$ RAM

    WritePoint( vPos, *pCoord );

    asnLocalisation.type = EnumTypeLocalisation::point;
    asnLocalisation.vecteur_point.n     = 1;
    asnLocalisation.vecteur_point.elem  = pCoord;
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
// Name: NET_ASN_Tools::WritePath
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WritePath( const T_PointVector& pointVector, ASN1T_Itineraire& asn )
{
    asn.type            = EnumTypeLocalisation::line;
    asn.vecteur_point.n = pointVector.size(); 
    if( pointVector.empty() )
        return;

    ASN1T_CoordUTM* pCoord = new ASN1T_CoordUTM[ pointVector.size() ]; //$$$ RAM
    asn.vecteur_point.elem = pCoord;
    uint i = 0;
    for( CIT_PointVector itPoint = pointVector.begin(); itPoint != pointVector.end(); ++itPoint )
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
void NET_ASN_Tools::WriteEllipse( const MT_Ellipse& ellipse, ASN1T_Localisation& asnLocalisation )
{
    asnLocalisation.type = EnumTypeLocalisation::ellipse;
    asnLocalisation.vecteur_point.n = 3;
    asnLocalisation.vecteur_point.elem = new ASN1T_CoordUTM[3];
    WritePoint( ellipse.GetCenter(), asnLocalisation.vecteur_point.elem[0] );
    WritePoint( ellipse.GetMajorAxisHighPoint(), asnLocalisation.vecteur_point.elem[1] );
    WritePoint( ellipse.GetMinorAxisHighPoint(), asnLocalisation.vecteur_point.elem[2] );
}


//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListPoint
// Created: AGN 03-01-20
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WritePointList( const T_PointVector& pointVector, ASN1T_ListPoint& asn )
{
    asn.n = pointVector.size();
    if( pointVector.empty() )
        return;

    ASN1T_Localisation* pCoord = new ASN1T_Localisation[ pointVector.size() ]; //$$$ RAM
    asn.elem = pCoord;

    uint i = 0;
    for( CIT_PointVector itPoint = pointVector.begin(); itPoint != pointVector.end(); ++itPoint )
        WritePoint( *itPoint, pCoord[i++] );
}

//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListLocalisation
// Created: AGN 03-08-18
//-----------------------------------------------------------------------------
// static
void NET_ASN_Tools::WriteLocationList( const T_LocalisationPtrVector& locaList, ASN1T_ListLocalisation& asnList )
{
    if( locaList.empty() )
    {
        asnList.n = 0;
        return;
    }

    ASN1T_Localisation* pLcal = new ASN1T_Localisation[ locaList.size() ];
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
void NET_ASN_Tools::WritePolygonList( const T_LocalisationPtrVector& locaList, ASN1T_ListPolygon& asnList )
{
    if( locaList.empty() )
    {
        asnList.n = 0;
        return;
    }

    ASN1T_Localisation* pLcal = new ASN1T_Localisation[ locaList.size() ];
    asnList.n = locaList.size();
    asnList.elem = pLcal;

    for( CIT_LocalisationPtrVector it = locaList.begin(); it != locaList.end(); ++it )
        WriteLocation( **it, pLcal[it - locaList.begin()] );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListItineraire
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WritePathList( const T_ItinerairePtrVector& itineraireVector, ASN1T_ListItineraire&   asn )
{
    if( itineraireVector.empty() )
    {
        asn.n = 0;
        return;
    }

    ASN1T_Localisation* pLcal = new ASN1T_Localisation[ itineraireVector.size() ];
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
void NET_ASN_Tools::WriteAgent( const DEC_RolePion_Decision& pion, ASN1T_Agent& asn )
{
    asn = pion.GetPion().GetID();
}


// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteAutomate
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteAutomate( const DEC_AutomateDecision& automate, ASN1T_Agent& asn )
{
    asn = automate.GetAutomate().GetID();    
}


//-----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListAgent
// Created: AGN 03-01-20
//-----------------------------------------------------------------------------
void NET_ASN_Tools::WriteAgentList( const T_ObjectVector& unitList, ASN1T_ListAgent& asnListAgent )
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
// Name: NET_ASN_Tools::WriteListAutomate
// Created: AGN 03-01-20
//-----------------------------------------------------------------------------
void NET_ASN_Tools::WriteAutomateList( const T_ObjectVector& unitList, ASN1T_ListAutomate& asnListAgent )
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
void NET_ASN_Tools::WriteAgentKnowledge( const DEC_Knowledge_Agent& knowledge, ASN1T_KnowledgeAgent& asnKnowledge )
{
    asnKnowledge = knowledge.GetMosID();
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteAgentKnowledgeList( const T_KnowledgeAgentDiaIDVector& knowledges, ASN1T_ListKnowledgeAgent& asnListKnowledge, const DEC_KS_KnowledgeGroupQuerier& knowledge )
{
    asnListKnowledge.n = knowledges.size();
    if( knowledges.empty() )
        return;
    
    asnListKnowledge.elem = new ASN1T_OID[ knowledges.size() ];
    uint i = 0;
    for( CIT_KnowledgeAgentDiaIDVector itKnowledge = knowledges.begin(); itKnowledge != knowledges.end(); ++itKnowledge )    
    {
        const DEC_Knowledge_Agent* pKnowledge = knowledge.GetKnowledgeAgentFromDiaID( (uint)*itKnowledge );
        if( pKnowledge )
            asnListKnowledge.elem[i] = pKnowledge->GetMosID();
        else
            asnListKnowledge.elem[i] = 0;       
        ++i;
    }
}
 

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteObjectKnowledge
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteObjectKnowledge( const DEC_Knowledge_Object& knowledge, ASN1T_KnowledgeObject& asnKnowledge )
{
    asnKnowledge = knowledge.GetMosID();
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteListKnowledgeObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteObjectKnowledgeList( const T_KnowledgeObjectDiaIDVector& knowledges, ASN1T_ListKnowledgeObject& asnListKnowledge, const DEC_KS_KnowledgeGroupQuerier& knowledge )
{
    asnListKnowledge.n = knowledges.size();
    if( knowledges.empty() )
        return;
    
    asnListKnowledge.elem = new ASN1T_OID[ knowledges.size() ];
    uint i = 0;
    for( CIT_KnowledgeObjectDiaIDVector itKnowledge = knowledges.begin(); itKnowledge != knowledges.end(); ++itKnowledge )    
    {
        const DEC_Knowledge_Object* pKnowledge = knowledge.GetKnowledgeObjectFromDiaID( (uint)*itKnowledge );
        if( pKnowledge )
            asnListKnowledge.elem[i] = pKnowledge->GetMosID();
        else
            asnListKnowledge.elem[i] = 0;       
        ++i;
    }
}
  




// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteGDH
// Created: NLD 2004-01-15
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteGDH( uint nRealTimeSec, ASN1T_GDH& asnGDH )
{
    asnGDH.datation     = nRealTimeSec + MIL_NBR_SEC_BETWEEN_1901_AND_1970;
	asnGDH.qualificatif = EnumGDH_Qualificatif::at;   
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::WriteGDH
// Created: NLD 2004-01-15
// -----------------------------------------------------------------------------
void NET_ASN_Tools::WriteGDH( ASN1T_GDH& asnGDH )
{
    asnGDH.datation     = MIL_Tools::GetRealTime() + MIL_NBR_SEC_BETWEEN_1901_AND_1970;
	asnGDH.qualificatif = EnumGDH_Qualificatif::at;   
}  

//=============================================================================
// MISC TOOLS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_ListAgent& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_ListAutomate& asn )
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
    if( asn.vecteur_point.n > 0 )
        delete [] asn.vecteur_point.elem;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_ListPolygon& asn )
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
void NET_ASN_Tools::Delete( ASN1T_ListPoint& asn )
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
void NET_ASN_Tools::Delete( ASN1T_ListItineraire& asn )
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
void NET_ASN_Tools::Delete( ASN1T_ListLocalisation& asn )
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
void NET_ASN_Tools::Delete( ASN1T_ListKnowledgeAgent& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_ListKnowledgeObject& asn )
{
    if( asn.n > 0 )
        delete [] asn.elem;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::Delete
// Created: NLD 2005-08-17
// -----------------------------------------------------------------------------
void NET_ASN_Tools::Delete( ASN1T_ListMissionGenObject& asn )
{
    if( asn.n > 0 )
    {
        for( uint i = 0; i < asn.n; ++i )
        {
            ASN1T_MissionGenObject* pGenObject = &asn.elem[ i ];
            delete pGenObject;
        }
        delete [] asn.elem;
    }
}

// =============================================================================
// MISSION PARAMETERS : KNOWLEDGE OBJECT DIA - SIM - ASN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyObjectKnowledge
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyObjectKnowledge( const ASN1T_KnowledgeObject& asn, DIA_Variable_ABC& dia, const DEC_KS_KnowledgeGroupQuerier& knowledge )
{
    DEC_Knowledge_Object* pKnowledge = NET_ASN_Tools::ReadObjectKnowledge( asn, knowledge );
    if( !pKnowledge ) 
        return false;

    dia.SetValue( (void*)pKnowledge->GetDiaID(), &DEC_Tools::GetTypeConnaissanceObjet() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyObjectKnowledge
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyObjectKnowledge( const DIA_Variable_ABC& dia, ASN1T_KnowledgeObject& asn, const DEC_KS_KnowledgeGroupQuerier& knowledge )
{
    assert( DEC_Tools::CheckTypeConnaissanceObjet( dia ) );

    uint nDiaID = (uint)dia.ToPtr();
    DEC_Knowledge_Object* pKnowledge = knowledge.GetKnowledgeObjectFromDiaID( nDiaID );
    if( !pKnowledge )
        return false;
    
    NET_ASN_Tools::WriteObjectKnowledge( *pKnowledge, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetObjectKnowledge
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetObjectKnowledge( DIA_Variable_ABC& dia )
{
    dia.SetValue( (void*)0, &DEC_Tools::GetTypeConnaissanceObjet() );
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
bool NET_ASN_Tools::CopyObjectKnowledgeList( const ASN1T_ListKnowledgeObject& asn, DIA_Variable_ABC& dia, const DEC_KS_KnowledgeGroupQuerier& knowledge )
{
    T_KnowledgeObjectDiaIDVector knowledges;
    
    if( !NET_ASN_Tools::ReadObjectKnowledgeList( asn, knowledges, knowledge ) )
        return false;

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceObjet() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListKnowledgeObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyObjectKnowledgeList( const DIA_Variable_ABC& dia, ASN1T_ListKnowledgeObject& asn, const DEC_KS_KnowledgeGroupQuerier& knowledge )
{
    assert( DEC_Tools::CheckTypeListeConnaissancesObjet( dia ) );

    const DIA_Variable_ObjectList& diaObjectList = static_cast< const DIA_Variable_ObjectList& >( dia );

    T_KnowledgeObjectDiaIDVector knowledges = diaObjectList.ToUserTypeList( knowledges );
    NET_ASN_Tools::WriteObjectKnowledgeList( knowledges, asn, knowledge );
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

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetListKnowledgeObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetObjectKnowledgeList( DIA_Variable_ABC& dia )
{
    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    T_KnowledgeObjectDiaIDVector knowledges;
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceObjet() );
}


// =============================================================================
// MISSION PARAMETERS : KNOWLEDGE AGENT DIA - SIM - ASN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgentKnowledge( const ASN1T_KnowledgeAgent& asn, DIA_Variable_ABC& dia, const DEC_KS_KnowledgeGroupQuerier& knowledge )
{
    DEC_Knowledge_Agent* pKnowledge = NET_ASN_Tools::ReadAgentKnowledge( asn, knowledge );
    if( !pKnowledge ) 
        return false;

    dia.SetValue( (void*)pKnowledge->GetDiaID(), &DEC_Tools::GetTypeConnaissanceAgent() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgentKnowledge( const DIA_Variable_ABC& dia, ASN1T_KnowledgeAgent& asn, const DEC_KS_KnowledgeGroupQuerier& knowledge )
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( dia ) );

    uint nDiaID = (uint)dia.ToPtr();
    DEC_Knowledge_Agent* pKnowledge = knowledge.GetKnowledgeAgentFromDiaID( nDiaID );
    if( !pKnowledge )
        return false;
    
    asn = pKnowledge->GetMosID();
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
// Name: NET_ASN_Tools::ResetDiaKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetAgentKnowledge( DIA_Variable_ABC& dia )
{
    dia.SetValue( (void*)0, &DEC_Tools::GetTypeConnaissanceAgent() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgentKnowledgeList( const ASN1T_ListKnowledgeAgent& asn, DIA_Variable_ABC& dia, const DEC_KS_KnowledgeGroupQuerier& knowledge )
{
    T_KnowledgeAgentDiaIDVector knowledges;
    
    if( !NET_ASN_Tools::ReadAgentKnowledgeList( asn, knowledges, knowledge ) )
        return false;

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgentKnowledgeList( const DIA_Variable_ABC& dia, ASN1T_ListKnowledgeAgent& asn, const DEC_KS_KnowledgeGroupQuerier& knowledge )
{
    assert( DEC_Tools::CheckTypeListeConnaissancesAgent( dia ) );

    const DIA_Variable_ObjectList& diaObjectList = static_cast< const DIA_Variable_ObjectList& >( dia );

    T_KnowledgeAgentDiaIDVector knowledges = diaObjectList.ToUserTypeList( knowledges );
    NET_ASN_Tools::WriteAgentKnowledgeList( knowledges, asn, knowledge );
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

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetListKnowledgeAgent
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetAgentKnowledgeList( DIA_Variable_ABC& dia )
{
    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    T_KnowledgeAgentDiaIDVector knowledges;
    diaObjectList.SetValueUserType( knowledges, DEC_Tools::GetTypeConnaissanceAgent() );
}

// =============================================================================
// MISSION PARAMETERS TOOLS : AGENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyAgent
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgent( const ASN1T_Agent& asn, DIA_Variable_ABC& dia, bool bValueIfOptional /*= true*/ )
{
    if( !bValueIfOptional )
    {
        dia.SetValue( *(DIA_TypedObject*)0 );
        return true;
    }

    DEC_RolePion_Decision* pAgent = NET_ASN_Tools::ReadAgent( asn );
    dia.SetValue( *pAgent );
    return pAgent != 0;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyAgent
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgent( const DIA_Variable_ABC& dia, ASN1T_Agent& asn )
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
// Name: NET_ASN_Tools::ResetAgent
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetAgent( DIA_Variable_ABC& dia )
{
    dia.SetValue( *(DIA_TypedObject*)0 );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyAutomate
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAutomate( const ASN1T_Agent& asn, DIA_Variable_ABC& dia, bool bValueIfOptional /*= true*/ )
{
    if( !bValueIfOptional )
    {
        dia.SetValue( *(DIA_TypedObject*)0 );
        return true;
    }

    DEC_AutomateDecision* pAgent = NET_ASN_Tools::ReadAutomate( asn );
    dia.SetValue( *pAgent );
    return pAgent != 0;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyAutomate
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAutomate( const DIA_Variable_ABC& dia, ASN1T_Agent& asn )
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
// Name: NET_ASN_Tools::ResetAutomate
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetAutomate( DIA_Variable_ABC& dia )
{
    dia.SetValue( *(DIA_TypedObject*)0 );
}
   
// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListAgent
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgentList( const ASN1T_ListAgent& asn, DIA_Variable_ABC& dia )
{
    T_ObjectVector agents;
    if( !NET_ASN_Tools::ReadAgentList( asn, agents ) )
        return false;
    dia.SetValue( agents );
    return !agents.empty();
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListAgent
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAgentList( const DIA_Variable_ABC& dia, ASN1T_ListAgent& asn )
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
// Name: NET_ASN_Tools::ResetListAgent
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetAgentList( DIA_Variable_ABC& dia )
{
     T_ObjectVector emptyList;
     dia.SetValue( emptyList );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListAutomate
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAutomateList( const ASN1T_ListAutomate& asn, DIA_Variable_ABC& dia )
{
    T_ObjectVector automates;
    if( !NET_ASN_Tools::ReadAutomateList( asn, automates ) )
        return false;
    dia.SetValue( automates );
    return !automates.empty();
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListAutomate
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAutomateList( const DIA_Variable_ABC& dia, ASN1T_ListAutomate& asn )
{
    assert( DEC_Tools::CheckTypeListeAutomates( dia ) );
    NET_ASN_Tools::WriteAutomateList( dia.ToSelection(), asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListAutomate
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyAutomateList( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeListeAutomates( diaFrom ) );
    assert( DEC_Tools::CheckTypeListeAutomates( diaTo ) );
    diaTo.SetValue( diaFrom.ToSelection() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetListAutomate
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetAutomateList( DIA_Variable_ABC& dia )
{
     T_ObjectVector emptyList;
     dia.SetValue( emptyList );
}

// =============================================================================
// Mission parameters tools : LOG specific
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyMaintenancePriorities
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyMaintenancePriorities( const ASN1T_MaintenancePriorites& asn, T_MaintenancePriorityVector& data, DIA_Variable_ABC& dia )
{
    data.clear();
    for( uint i = 0; i < asn.n; ++i )
    {
        uint nCompTypeMosID = asn.elem[i];
        const PHY_ComposanteTypePion* pType = PHY_ComposanteTypePion::FindComposanteType( nCompTypeMosID );
        if( !pType )
        {
            dia.SetValue( 0, &DEC_Tools::GetTypeMaintenancePriorites() );
            data.clear();
            return false;
        }
        data.push_back( pType );
    }
    dia.SetValue( &data, &DEC_Tools::GetTypeMaintenancePriorites() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyMaintenancePriorities
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyMaintenancePriorities( const DIA_Variable_ABC& diaFrom, T_MaintenancePriorityVector& container, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeMaintenancePriorites( diaFrom ) );
    const T_MaintenancePriorityVector* pData = diaFrom.ToUserPtr( pData );
    assert( pData );
    container = *pData;
    diaTo.SetValue( &container, &DEC_Tools::GetTypeMaintenancePriorites() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyMedicalPriorities
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyMedicalPriorities( const ASN1T_SantePriorites& asn, T_MedicalPriorityVector& data, DIA_Variable_ABC& dia )
{
    data.clear();
    for( uint i = 0; i < asn.n; ++i )
    {
        ASN1T_EnumHumanWound nWoundID = asn.elem[i];
        const PHY_HumanWound* pWound = PHY_HumanWound::Find( nWoundID );
        if( !pWound )
        {
            dia.SetValue( 0, &DEC_Tools::GetTypeMaintenancePriorites() );
            data.clear();
            return false;
        }
        data.push_back( pWound );
    }
    dia.SetValue( &data, &DEC_Tools::GetTypeSantePriorites() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyMedicalPriorities
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyMedicalPriorities( const DIA_Variable_ABC& diaFrom, T_MedicalPriorityVector& container, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeSantePriorites( diaFrom ) );
    const T_MedicalPriorityVector* pData = diaFrom.ToUserPtr( pData );
    assert( pData );
    container = *pData;
    diaTo.SetValue( &container, &DEC_Tools::GetTypeSantePriorites() );
    return true;
}

// =============================================================================
// Mission parameters tools : GEN specific
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyGenObject
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyGenObject( const ASN1T_MissionGenObject& asn, DIA_Variable_ABC& dia )
{
    DEC_Gen_Object* pGenObject = new DEC_Gen_Object();
    if( pGenObject->Initialize( asn ) != EnumOrderErrorCode::no_error )
    {
        delete pGenObject;
        pGenObject = 0;
    }
    dia.SetValue( *pGenObject );
    return pGenObject != 0;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyGenObject
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyGenObject( const DIA_Variable_ABC& /*dia*/, ASN1T_MissionGenObject& /*asn*/ )
{
    assert( false ); //$$$ TODO
    return false;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetGenObject
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetGenObject( DIA_Variable_ABC& dia )
{
    assert( DEC_Tools::CheckTypeGenObjet( dia ) );
    DEC_Gen_Object* pGenObject = dia.ToUserObject( pGenObject );
    delete pGenObject;
    dia.SetValue( *(DEC_Gen_Object*)0 );
}


// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListGenObjects
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyGenObjectList( const ASN1T_ListMissionGenObject& asn, DIA_Variable_ABC& dia )
{
    T_ObjectVector genObjects;
    for( uint i = 0; i < asn.n; ++i )
    {
        DEC_Gen_Object* pGenObject = new DEC_Gen_Object();
        if( pGenObject->Initialize( asn.elem[i] ) != EnumOrderErrorCode::no_error )
        {
            MT_DELETEOWNED( genObjects );
            genObjects.clear();
            dia.SetValue( genObjects );
            return false;
        }
        genObjects.push_back( pGenObject );
    }

    dia.SetValue( genObjects );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListGenObjects
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyGenObjectList( const DIA_Variable_ABC& /*dia*/, ASN1T_ListMissionGenObject& /*asn*/ )
{
    assert( false );
    return false;    
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetListGenObjects
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetGenObjectList( DIA_Variable_ABC& dia )
{
    T_ObjectVector genObjects = dia.ToSelection();
    for( CIT_ObjectVector itObj = genObjects.begin(); itObj != genObjects.end(); ++itObj )
        delete *itObj;
    genObjects.clear();
    dia.SetValue( genObjects );
}

// =============================================================================
// Mission parameters tools : Localisation/Geometry DIA - SIM - ASN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyLocation
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyLocation( const ASN1T_Localisation& asn, TER_Localisation& container, DIA_Variable_ABC& dia )
{
    if( !NET_ASN_Tools::ReadLocation( asn, container ) )
        return false;
    dia.SetValue( &container, &DEC_Tools::GetTypeLocalisation() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyLocation
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyLocation( const DIA_Variable_ABC& dia, ASN1T_Localisation& asn )
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
void NET_ASN_Tools::CopyLocation( const DIA_Variable_ABC& diaFrom, TER_Localisation& container, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeLocalisation( diaFrom ) );
    assert( DEC_Tools::CheckTypeLocalisation( diaTo ) );
    TER_Localisation* pLocalisation = diaFrom.ToUserPtr( pLocalisation );
    container.Reset( *pLocalisation );
    diaTo.SetValue( &container, &DEC_Tools::GetTypeLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetLocation
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetLocation( TER_Localisation& container, DIA_Variable_ABC& dia )
{
    container.Reset();
    dia.SetValue( &container, &DEC_Tools::GetTypeLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListLocalisation
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyLocationList( const ASN1T_ListLocalisation& asn, DIA_Variable_ABC& dia )
{
    T_LocalisationPtrVector localisations;

    if( !NET_ASN_Tools::ReadLocationList( asn, localisations, 1 ) )
        return false;

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    diaObjectList.SetValueUserType( localisations, DEC_Tools::GetTypeLocalisation() );
    return true;    
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListLocalisation
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyLocationList( const DIA_Variable_ABC& dia, ASN1T_ListLocalisation& asn )
{
    assert( DEC_Tools::CheckTypeListeLocalisations( dia ) );
    const DIA_Variable_ObjectList& diaObjectList = static_cast< const DIA_Variable_ObjectList& >( dia );

    T_LocalisationPtrVector localisations = diaObjectList.ToUserTypeList( localisations );
    NET_ASN_Tools::WriteLocationList( localisations, asn );
    return true;    
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListLocalisation
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
    for( CIT_LocalisationPtrVector itLoc = localisationsFrom.begin(); itLoc != localisationsFrom.end(); ++itLoc )
    {
        TER_Localisation* pNewLoc = new TER_Localisation(); //$$$$ RAM
        pNewLoc->Reset( **itLoc );
        localisationsTo.push_back( pNewLoc );
    }
    diaListTo.SetValueUserType( localisationsTo, DEC_Tools::GetTypeLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetListLocalisation
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetLocationList( DIA_Variable_ABC& dia )
{
    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    T_LocalisationPtrVector localisations = diaObjectList.ToUserTypeList( localisations );
    for( IT_LocalisationPtrVector itLoc = localisations.begin(); itLoc != localisations.end(); ++itLoc )
        delete *itLoc;
    localisations.clear();
    diaObjectList.SetValueUserType( localisations, DEC_Tools::GetTypeLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPolygon
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPolygon( const ASN1T_Polygon& asn, TER_Localisation& container, DIA_Variable_ABC& dia )
{
    if( !NET_ASN_Tools::ReadPolygon( asn, container ) )
        return false;
    dia.SetValue( &container, &DEC_Tools::GetTypeLocalisation() );
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
void NET_ASN_Tools::CopyPolygon( const DIA_Variable_ABC& diaFrom, TER_Localisation& container, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeLocalisation( diaFrom ) );
    assert( DEC_Tools::CheckTypeLocalisation( diaTo ) );

    TER_Localisation* pPolygon = diaFrom.ToUserPtr( pPolygon );
    container.Reset( *pPolygon );
    diaTo.SetValue( &container, &DEC_Tools::GetTypeLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetPolygon
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetPolygon( TER_Localisation& container, DIA_Variable_ABC& dia )
{
    container.Reset();
    dia.SetValue( &container, &DEC_Tools::GetTypeLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListPolygon
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPolygonList( const ASN1T_ListPolygon& asn, DIA_Variable_ABC& dia )
{
    T_LocalisationPtrVector Polygons;

    if( !NET_ASN_Tools::ReadPolygonList( asn, Polygons, 1 ) )
        return false;

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    diaObjectList.SetValueUserType( Polygons, DEC_Tools::GetTypeLocalisation() );
    return true;    
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListPolygon
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPolygonList( const DIA_Variable_ABC& dia, ASN1T_ListPolygon& asn )
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
    for( CIT_LocalisationPtrVector itLoc = PolygonsFrom.begin(); itLoc != PolygonsFrom.end(); ++itLoc )
    {
        TER_Localisation* pNewLoc = new TER_Localisation(); //$$$$ RAM
        pNewLoc->Reset( **itLoc );
        PolygonsTo.push_back( pNewLoc );
    }
    diaListTo.SetValueUserType( PolygonsTo, DEC_Tools::GetTypeLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetListPolygon
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetPolygonList( DIA_Variable_ABC& dia )
{
    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    T_LocalisationPtrVector Polygons = diaObjectList.ToUserTypeList( Polygons );
    for( CIT_LocalisationPtrVector itLoc = Polygons.begin(); itLoc != Polygons.end(); ++itLoc )
        delete *itLoc;
    Polygons.clear();
    diaObjectList.SetValueUserType( Polygons, DEC_Tools::GetTypeLocalisation() );
}


// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPoint
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPoint( const ASN1T_Point& asn, MT_Vector2D& container, DIA_Variable_ABC& dia, bool bValueIfOptional /*= true*/ )
{
    if( !bValueIfOptional )
    {
        dia.SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return true;
    }
    
    if( !NET_ASN_Tools::ReadPoint( asn, container ) )
        return false;

    dia.SetValue( &container, &DEC_Tools::GetTypePoint() );
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
    if( !pPoint )
    {
        assert( false );
        return false;
    }
    NET_ASN_Tools::WritePoint( *pPoint, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPoint
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPoint( const DIA_Variable_ABC& dia, ASN1T_CoordUTM& asn )
{
    assert( DEC_Tools::CheckTypePoint( dia ) );

    MT_Vector2D* pPoint = dia.ToUserPtr( pPoint );
    if( !pPoint )
    {
        assert( false );
        return false;
    }
    NET_ASN_Tools::WritePoint( *pPoint, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPoint
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPoint( const DIA_Variable_ABC& diaFrom, MT_Vector2D& container, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypePoint( diaFrom ) );
    assert( DEC_Tools::CheckTypePoint( diaTo ) );
    MT_Vector2D* pPoint = diaFrom.ToUserPtr( pPoint );
    if( !pPoint )   
    {
        assert( false );
        diaTo.SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return false;
    }
    container = *pPoint;
    diaTo.SetValue( &container, &DEC_Tools::GetTypePoint() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetPoint
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetPoint( MT_Vector2D& container, DIA_Variable_ABC& dia )
{
    container.Reset();
    dia.SetValue( &container, &DEC_Tools::GetTypePoint() );
}
    
    
// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListPoint
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPointList( const ASN1T_ListPoint& asn, T_PointVector& container, DIA_Variable_ABC& dia )
{
    container.clear();
    if( !NET_ASN_Tools::ReadPointList( asn, container ) )
    {
        dia.SetValue( (void*)0, &DEC_Tools::GetTypeListePoints() );
        return false;
    }

    dia.SetValue( &container, &DEC_Tools::GetTypeListePoints() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListPoint
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPointList( const DIA_Variable_ABC& dia, ASN1T_ListPoint& asn )
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
bool NET_ASN_Tools::CopyPointList( const DIA_Variable_ABC& diaFrom, T_PointVector& container, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeListePoints( diaFrom ) );
    assert( DEC_Tools::CheckTypeListePoints( diaTo  ) );

    T_PointVector* pPoints = diaFrom.ToUserPtr( pPoints );
    if( !pPoints )    
    {
        assert( false );
        diaTo.SetValue( (void*)0, &DEC_Tools::GetTypeListePoints() );
        return false;
    }
    container = *pPoints;
    diaTo.SetValue( &container, &DEC_Tools::GetTypeListePoints() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetListPoint
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetPointList( T_PointVector& container, DIA_Variable_ABC& dia )
{
    container.clear();
    dia.SetValue( &container, &DEC_Tools::GetTypeListePoints() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPath
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPath( const ASN1T_Itineraire& asn, T_PointVector& container, DIA_Variable_ABC& dia )
{
    container.clear();
    if( !NET_ASN_Tools::ReadPath( asn, container ) )
    {
        dia.SetValue( (void*)0, &DEC_Tools::GetTypeListePoints() );
        return false;
    }

    dia.SetValue( &container, &DEC_Tools::GetTypeListePoints() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyPath
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPath( const DIA_Variable_ABC& dia, ASN1T_Itineraire& asn )
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
bool NET_ASN_Tools::CopyPath( const DIA_Variable_ABC& diaFrom, T_PointVector& container, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeListePoints( diaFrom ) );
    assert( DEC_Tools::CheckTypeListePoints( diaTo ) );

    T_PointVector* pPoints = diaFrom.ToUserPtr( pPoints );
    if( !pPoints )    
    {
        assert( false );
        diaTo.SetValue( (void*)0, &DEC_Tools::GetTypeListePoints() );
        return false;
    }
    container.clear();
    if( !pPoints->empty() )
        container.push_back( pPoints->back() );
    diaTo.SetValue( &container, &DEC_Tools::GetTypeListePoints() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetPath
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetPath( T_PointVector& container, DIA_Variable_ABC& dia )
{
    container.clear();
    dia.SetValue( &container, &DEC_Tools::GetTypeListePoints() );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyListItineraire
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyPathList( const ASN1T_ListItineraire& asn, DIA_Variable_ABC& dia )
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
bool NET_ASN_Tools::CopyPathList( const DIA_Variable_ABC& dia, ASN1T_ListItineraire& asn )
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
    T_ItinerairePtrVector itinerairesTo;
    for( CIT_ItinerairePtrVector itIti = itinerairesFrom.begin(); itIti != itinerairesFrom.end(); ++itIti )
    {
        T_PointVector* pPointVector = new T_PointVector(); //$$$ RAM
        *pPointVector = **itIti;
        itinerairesTo.push_back( pPointVector );
    }
    diaListTo.SetValueUserType( itinerairesTo, DEC_Tools::GetTypeListePoints() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetListItineraire
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetPathList( DIA_Variable_ABC& dia )
{
    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( dia );
    T_ItinerairePtrVector itineraires = diaObjectList.ToUserTypeList( itineraires );
    for( CIT_ItinerairePtrVector itIti = itineraires.begin(); itIti != itineraires.end(); ++itIti )
        delete *itIti; //$$$
    itineraires.clear();
    diaObjectList.SetValueUserType( itineraires, DEC_Tools::GetTypeListePoints() );
}
 
   
// =============================================================================
// Mission parameters tools : GDH DIA - SIM - ASN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyGDH
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyGDH( const ASN1T_GDH& asn, DIA_Variable_ABC& dia, bool bValueIfOptional /*= true*/ )
{
    if( !bValueIfOptional )    
    {
        dia.SetValue( (float)0. );
        return true;
    }

    dia.SetValue( MIL_Tools::ConvertRealTimeSimToDia( NET_ASN_Tools::ReadGDH( asn ) ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyGDH
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyGDH( const DIA_Variable_ABC& dia, ASN1T_GDH& asn )
{
    float rValue = dia.ToFloat();
    if( rValue == 0. )
        return false;
    NET_ASN_Tools::WriteGDH( MIL_Tools::ConvertRealTimeDiaToSim( rValue ), asn );
    return true;
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

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetGDH
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetGDH( DIA_Variable_ABC& dia )
{
    dia.SetValue( (float)0. );
}

// =============================================================================
// Mission parameters tools : basic types
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyDirection
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyDirection( const ASN1T_Direction& asn, MT_Vector2D& container, DIA_Variable_ABC& dia )
{
    NET_ASN_Tools::ReadDirection( asn, container );
    dia.SetValue( &container, &DEC_Tools::GetTypeDirection() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyDirection
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyDirection( const DIA_Variable_ABC& dia, ASN1T_Direction& asn )
{
    assert( DEC_Tools::CheckTypeDirection( dia ) );

    MT_Vector2D* pValue = dia.ToUserPtr( pValue );
    if( !pValue )
    {
        assert( false );
        return false;
    }
    NET_ASN_Tools::WriteDirection( *pValue, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyDirection
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyDirection( const DIA_Variable_ABC& diaFrom, MT_Vector2D& container, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeDirection( diaFrom ) );
    assert( DEC_Tools::CheckTypeDirection( diaTo ) );

    MT_Vector2D* pValue = diaFrom.ToUserPtr( pValue );
    if( !pValue )
    {
        assert( false );
        return false;
    }
    assert( !pValue->IsZero() );
    container = *pValue;
    diaTo.SetValue( &container, &DEC_Tools::GetTypeDirection() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetDirection
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetDirection( MT_Vector2D& container, DIA_Variable_ABC& dia )
{
//    container.Reset();
    container = MT_Vector2D( 0., 1. ); //$$$
    dia.SetValue( &container, &DEC_Tools::GetTypeDirection() );
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
// Name: NET_ASN_Tools::ResetBool
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetBool( DIA_Variable_ABC& dia )
{
    dia.SetValue( false );   
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
// Name: NET_ASN_Tools::ResetNumeric
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetNumeric( DIA_Variable_ABC& dia )
{
    dia.SetValue( (float)0. );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetNumeric
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyID ( const ASN1T_OID& asn, DIA_Variable_ABC& dia )
{
    dia.SetValue( (void*)asn, &DEC_Tools::GetTypeID() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetNumeric
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyID ( const DIA_Variable_ABC& dia, ASN1T_OID& asn )
{
    assert( DEC_Tools::CheckTypeID( dia ) );
    asn = (int)dia.ToPtr();
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetNumeric
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyID ( const DIA_Variable_ABC& diaFrom, DIA_Variable_ABC& diaTo )
{
    assert( DEC_Tools::CheckTypeID( diaFrom ) );
    assert( DEC_Tools::CheckTypeID( diaTo ) );
    diaTo.SetValue( diaFrom.ToPtr(), &DEC_Tools::GetTypeID() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetNumeric
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetID( DIA_Variable_ABC& dia )
{
    dia.SetValue( (void*)0, &DEC_Tools::GetTypeID() );
}


// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyNatureAtlas
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyNatureAtlas( const ASN1T_NatureAtlas& asn, DIA_Variable_ABC& dia )
{
    int nNatureAtlas = ( asn.data[0] << 8 ) + asn.data[1];
    dia.SetValue( nNatureAtlas );
    return true;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::CopyNatureAtlas
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
bool NET_ASN_Tools::CopyNatureAtlas( const DIA_Variable_ABC& dia, ASN1T_NatureAtlas& asn )
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
// Name: NET_ASN_Tools::ResetNatureAtlas
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetNatureAtlas( DIA_Variable_ABC& dia )
{
    dia.SetValue( (int)0 );
}

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
    
// -----------------------------------------------------------------------------
// Name: NET_ASN_Tools::ResetEnumeration
// Created: NLD 2004-04-22
// -----------------------------------------------------------------------------
void NET_ASN_Tools::ResetEnumeration( DIA_Variable_ABC& dia )
{
    dia.SetValue( 0 );
}






