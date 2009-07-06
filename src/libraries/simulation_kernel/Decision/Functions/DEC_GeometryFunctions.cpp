// *****************************************************************************
//
// $Created: NLD 2003-08-20 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_GeometryFunctions.cpp $
// $Author: Nld $
// $Modtime: 7/06/05 9:59 $
// $Revision: 46 $
// $Workfile: DEC_GeometryFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "DEC_GeometryFunctions.h"
#include "DEC_FunctionsTools.h"
#include "DEC_FrontAndBackLinesComputer.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Path/DEC_PathPoint.h"
#include "Decision/Path/Agent/DEC_Agent_Path.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "Tools/MIL_Tools.h"
#include "simulation_terrain/TER_Localisation.h"
#include "MT_Tools/MT_Random.h"

#define PRECISION 0.0000001

// =============================================================================
// TOUT POURRI
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeAgentsBarycenter
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeAgentsBarycenter( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeListePions( call.GetParameter( 0 ) ) );
    const T_ObjectVector selection = call.GetParameter( 0 ).ToSelection();
    MT_Vector2D vResult;
    for( CIT_ObjectVector it = selection.begin(); it != selection.end(); ++it )
    {
        DEC_RolePion_Decision* pKnow = static_cast< DEC_RolePion_Decision* >( *it );
        assert( pKnow );

        vResult = vResult + pKnow->GetPion().GetRole< PHY_RolePion_Location >().GetPosition();
    }

    if( selection.size() != 0 )
        vResult = (vResult / (MT_Float)selection.size());

    MT_Vector2D* pNew = new MT_Vector2D( vResult );
    call.GetResult().SetValue( (void*)pNew, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDestPointForPion
// Created: JVT 2004-12-06
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeDestPointForPion( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );

    assert( pPion );
    assert( std::find( callerAutomate.GetPions().begin(), callerAutomate.GetPions().end(), &pPion->GetPion() ) != callerAutomate.GetPions().end() );

    ComputeDestPoint( call, pPion->GetPion() );
}

// =============================================================================
// CREATION / DESTRUCTION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreateLocalisation
// Created: JVT 2005-05-11
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::CreateLocalisation( DIA_Call_ABC& call )
{
    TER_Localisation* pLoc = new TER_Localisation();
    call.GetResult().SetValue( (void*)pLoc, &DEC_Tools::GetTypeLocalisation() );
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreateListPoint
// Created: NLD 2003-09-15
//-----------------------------------------------------------------------------
void DEC_GeometryFunctions::CreateListPoint( DIA_Call_ABC& call )
{
    T_PointVector* pResult = new T_PointVector();
    call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypeListePoints() );
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreatePoint
// Created: AGN 03-03-14
//-----------------------------------------------------------------------------
// static
void DEC_GeometryFunctions::CreatePoint( DIA_Call_ABC& call )
{
    MT_Vector2D* pVect = new MT_Vector2D();
    call.GetResult().SetValue( (void*)pVect, &DEC_Tools::GetTypePoint() );
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreateDirection
// Created: NLD 2003-05-13
//-----------------------------------------------------------------------------
void DEC_GeometryFunctions::CreateDirection( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 1 ) ) );

    MT_Vector2D* pPosSource = call.GetParameter( 0 ).ToUserPtr( pPosSource );
    MT_Vector2D* pPosDest   = call.GetParameter( 1 ).ToUserPtr( pPosDest   );

    assert( pPosSource );
    assert( pPosDest   );

    if( *pPosSource == *pPosDest )
    {
        MT_Vector2D* pVect = new MT_Vector2D( 0., 1. );
        call.GetResult().SetValue( (void*)pVect, &DEC_Tools::GetTypeDirection() );
    }
    else
    {
        MT_Vector2D* pVect = new MT_Vector2D( (*pPosDest) - (*pPosSource) );
        pVect->Normalize();
        call.GetResult().SetValue( (void*)pVect, &DEC_Tools::GetTypeDirection() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreateOrthoDirection
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::CreateOrthoDirection( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 0 ) ) );

    MT_Vector2D* pDir = call.GetParameter( 0 ).ToUserPtr( pDir );
    assert( pDir );
    assert( MT_IsZero( pDir->SquareMagnitude() - 1. ) );

    pDir = new MT_Vector2D( *pDir );

    if( call.GetParameters().GetParameters().size() >= 2 && call.GetParameter( 1 ).ToBool() == true )
        pDir->Rotate90();
    else
        pDir->Rotate90ClockWise();

    call.GetResult().SetValue( (void*)pDir, &DEC_Tools::GetTypeDirection() );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ReverseDirection
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ReverseDirection( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 0 ) ) );

    MT_Vector2D* pDir = call.GetParameter( 0 ).ToUserPtr( pDir );
    assert( pDir );
    *pDir *= -1.;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CopyAndReverseDirection
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::CopyAndReverseDirection( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 0 ) ) );

    const MT_Vector2D* pDir = call.GetParameter( 0 ).ToUserPtr( pDir );
    assert( pDir );

    MT_Vector2D* pNewDir = new MT_Vector2D( *pDir );
    *pNewDir *= -1.;
    call.GetResult().SetValue( pNewDir, &DEC_Tools::GetTypeDirection() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::RotateDirection
// Created: NLD 2005-03-18
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::RotateDirection( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 0 ) ) );

    MT_Vector2D* pDir = call.GetParameter( 0 ).ToUserPtr( pDir );
    assert( pDir );

    const MT_Float rAngle = - ( call.GetParameter( 1 ).ToFloat() * MT_PI / 180. );
    pDir->Rotate( rAngle );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CopyAndRotateDirection
// Created: NLD 2005-03-18
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::CopyAndRotateDirection( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 0 ) ) );

    MT_Vector2D* pDir = call.GetParameter( 0 ).ToUserPtr( pDir );
    assert( pDir );

    MT_Vector2D* pNewDir = new MT_Vector2D( *pDir );
    const MT_Float rAngle = - ( call.GetParameter( 1 ).ToFloat() * MT_PI / 180. );
    pNewDir->Rotate( rAngle );
    call.GetResult().SetValue( pNewDir, &DEC_Tools::GetTypeDirection() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CompareLocalisations
// Created: NLD 2003-09-01
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::CompareLocalisations( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 1 ) ) );

    TER_Localisation* pLocalisation1 = call.GetParameter( 0 ).ToUserPtr( pLocalisation1 );
    TER_Localisation* pLocalisation2 = call.GetParameter( 1 ).ToUserPtr( pLocalisation2 );

    assert( pLocalisation1 );
    assert( pLocalisation2 );

    call.GetResult().SetValue( *pLocalisation1 == *pLocalisation2 );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeLocalisationPointsForPionsInFuseau
/**
*   Calcule les points dans une localisation pour un nombre de pions donné, répartis
*   par rapport au barycentre de l'objet clippé dans le fuseau, et sur la droite
*   perpendiculaire à la direction donnée en paramètre, en respectant un distance
*   maximale entre les points.
*/
// Created: NLD 2003-08-20
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeLocalisationPointsForPionsInFuseau( DIA_Call_ABC& diaCall, const MIL_Automate& callerAutomate )
{
    DIA_Parameters& diaParams = diaCall.GetParameters();

    assert( DEC_Tools::CheckTypeListePions  ( diaCall.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeLocalisation( diaCall.GetParameter( 1 ) ) );
    assert( DEC_Tools::CheckTypeDirection   ( diaCall.GetParameter( 2 ) ) );

    T_ObjectVector      pions               = diaParams[ 0 ].ToSelection();
    TER_Localisation*   pLocalisation       = diaParams[ 1 ].ToUserPtr( pLocalisation );
    MT_Vector2D*        pDirDanger          = diaParams[ 2 ].ToUserPtr( pDirDanger );
    MT_Float            rDistMaxBtwPoints   = MIL_Tools::ConvertMeterToSim( diaParams[ 3 ].ToFloat() );

    assert( !pions.empty() );

    // 1. Vérifie si les pions sont bien dans le même fuseau
    const MIL_Fuseau& fuseau = static_cast< DEC_RolePion_Decision& >( **pions.begin() ).GetPion().GetOrderManager().GetFuseau();
    IT_ObjectVector itPion = pions.begin();
    for( ++itPion; itPion != pions.end(); ++itPion )
    {
        if( !( static_cast< DEC_RolePion_Decision& >( **itPion ).GetPion().GetOrderManager().GetFuseau() == fuseau ) ) //$$$ beark
        {
            diaCall.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeListePoints() );
            return;
        }
    }

    // 2. Clippe le polygone dans le fuseau
    T_PointVector clippedPointVector;
    pLocalisation->GetPointsClippedByPolygon( fuseau, clippedPointVector );
    if( clippedPointVector.empty() )
    {
        diaCall.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeListePoints() );
        return;
    }

    // 3. Barycentre polygone clippé
    MT_Vector2D vBarycenter = MT_ComputeBarycenter( clippedPointVector );
    assert( fuseau.IsInside( vBarycenter ) );
    assert( TER_World::GetWorld().IsValidPosition( vBarycenter ) );

    // 4. Droite perpendiculaire dir dangereuse
    MT_Vector2D vDirTmp( *pDirDanger );
    vDirTmp.Normalize();
    MT_Float rTmp = vDirTmp.rX_;
    vDirTmp.rX_   = vDirTmp.rY_;
    vDirTmp.rY_   = -rTmp;
    MT_Droite lineForPositions( vBarycenter, vDirTmp + vBarycenter );

    // 5. Extrémités dans fuseau
    MT_Vector2D vLeftPoint;
    MT_Vector2D vRightPoint;
    fuseau.ComputeExtremities( lineForPositions, vLeftPoint, vRightPoint );

    // 6. Extrémités réélles dans objet clippé - fait que si l'objet est un polygone
    if( clippedPointVector.size() > 2 )
    {
        MT_Line segmentLeftSide ( vBarycenter, vLeftPoint  );
        MT_Line segmentRightSide( vBarycenter, vRightPoint );

        TER_DistanceLess cmp( vBarycenter );
        T_PointSet collisionSet( cmp );
        collisionSet.clear();
        segmentLeftSide.Intersect2D( clippedPointVector, collisionSet, PRECISION );
        if( !collisionSet.empty() )
            vLeftPoint = *(--collisionSet.end());
        collisionSet.clear();
        segmentRightSide.Intersect2D( clippedPointVector, collisionSet, PRECISION );
        if( !collisionSet.empty() )
            vRightPoint = *(--collisionSet.end());
    }

    // 7. Répartition des points
    T_PointVector* pOutPoints = new T_PointVector();

    pOutPoints->reserve( pions.size() + 1 );

    // Nombre impaire => un pion est au barycentre
    if( pions.size() % 2 )
        pOutPoints->push_back( vBarycenter );

    uint nNbrPointsPerSide = pions.size() / 2;

    // Left side
    MT_Vector2D vDir( vLeftPoint - vBarycenter );
    MT_Float rIncr = std::min( vDir.Magnitude() / ( nNbrPointsPerSide + 1 ), rDistMaxBtwPoints );
    vDir.Normalize();
    for( uint i = 1; i <= nNbrPointsPerSide; ++i )
        pOutPoints->push_back( vBarycenter + vDir * ( rIncr * i ));

    // Right side
    vDir = MT_Vector2D( vRightPoint - vBarycenter );
    rIncr = std::min( vDir.Magnitude() / ( nNbrPointsPerSide + 1 ), rDistMaxBtwPoints );
    vDir.Normalize();
    for( uint j = 1; j <= nNbrPointsPerSide; ++j )
        pOutPoints->push_back( vBarycenter + vDir * ( rIncr * j ));

    // Envoi du résulat à DIA
    diaCall.GetResult().SetValue( (void*)pOutPoints, &DEC_Tools::GetTypeListePoints() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsPointInsideLocalisation
// Created: NLD 2004-05-17
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::IsPointInsideLocalisation( DIA_Call_ABC& call  )
{
    assert( DEC_Tools::CheckTypePoint       ( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 1 ) ) );

    MT_Vector2D*      pPoint        = call.GetParameter( 0 ).ToUserPtr( pPoint        );
    TER_Localisation* pLocalisation = call.GetParameter( 1 ).ToUserPtr( pLocalisation );

    call.GetResult().SetValue( pLocalisation->IsInside( *pPoint ) );
}


// =============================================================================
// GEOMETRY - POINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ConvertPointToLocalisation
// Created: NLD 2003-08-19
// -----------------------------------------------------------------------------
// static
void DEC_GeometryFunctions::ConvertPointToLocalisation( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );

    MT_Vector2D* pPos = call.GetParameter( 0 ).ToUserPtr( pPos );
    assert( pPos );

    TER_Localisation* pLoc = new TER_Localisation();
    pLoc->Reset( *pPos );

    call.GetResult().SetValue( (void*)pLoc, &DEC_Tools::GetTypeLocalisation() );
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::Distance
// Created: NLD 2002-12-24
//-----------------------------------------------------------------------------
void DEC_GeometryFunctions::Distance( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 1 ) ) );

    MT_Vector2D* p1 = call.GetParameter( 0 ).ToUserPtr( p1 );
    MT_Vector2D* p2 = call.GetParameter( 1 ).ToUserPtr( p2 );

    assert( p1 != 0 && p2 != 0 );
    call.GetResult().SetValue( MIL_Tools::ConvertSimToMeter( p1->Distance( *p2 ) ) );
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::TranslatePosition
// Created: JDY 03-01-23
//-----------------------------------------------------------------------------
void DEC_GeometryFunctions::TranslatePosition( DIA_Call_ABC& call)
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 1 ) ) );

    MT_Vector2D* p1 = call.GetParameter( 0 ).ToUserPtr( p1 );
    MT_Vector2D* p2 = call.GetParameter( 1 ).ToUserPtr( p2 );
    MT_Float     d  = MIL_Tools::ConvertMeterToSim( call.GetParameter( 2 ).ToFloat() );

    assert( p1 && p2 );

    MT_Vector2D* res = new MT_Vector2D();

    if( (*p1) == (*p2) )
        *res = *p1;
    else
        *res=*p1+ d*(*p2-*p1).Normalized();

    call.GetResult().SetValue( (void*)res, &DEC_Tools::GetTypePoint() );
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::TranslatePositionInDirection
// Created: AGN 03-04-11
//-----------------------------------------------------------------------------
void DEC_GeometryFunctions::TranslatePositionInDirection( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypePoint    ( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 1 ) ) );

    MT_Vector2D* p1 = call.GetParameter( 0 ).ToUserPtr( p1 );
    MT_Vector2D* p2 = call.GetParameter( 1 ).ToUserPtr( p2 );
    MT_Float     d  = MIL_Tools::ConvertMeterToSim( call.GetParameter( 2 ).ToFloat() );

    assert( p1 && p2 );
    assert( MT_IsZero( p2->SquareMagnitude() - 1. ) );

    MT_Vector2D* res = new MT_Vector2D();
    *res = *p1 + d * (*p2);

    call.GetResult().SetValue((void*)res, &DEC_Tools::GetTypePoint() );
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComparePositions
// Created: AGN 03-01-02
//-----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComparePositions( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 1 ) ) );

    MT_Vector2D* p1 = call.GetParameter( 0 ).ToUserPtr( p1 );
    MT_Vector2D* p2 = call.GetParameter( 1 ).ToUserPtr( p2 );
    assert( p1 && p2 );

    // $$$ JVT : Débile : si il y a besoin d'une "weldvalue" elle doit être prise en compte
    //           directement dans l'objet vecteur et dans ses operateurs ( en l'occurence == )
    static const MT_Float rWeldValue = TER_World::GetWorld().GetWeldValue();

    call.GetResult().SetValue( p1->Distance( *p2 ) <= rWeldValue );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CompareDirection
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::CompareDirection( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 1 ) ) );

    MT_Vector2D* pD1 = call.GetParameter( 0 ).ToUserPtr( pD1 );
    MT_Vector2D* pD2 = call.GetParameter( 1 ).ToUserPtr( pD2 );
    assert( pD1 && pD2 );

    call.GetResult().SetValue( *pD1 == *pD2 );
}

// =============================================================================
// CALCULS DE POSITIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeSupportPosition
// Created: NLD 2003-10-15
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeSupportPosition( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pAgentToSupport = call.GetParameter( 0 ).ToUserObject( pAgentToSupport );
    MT_Float               rDist           = MIL_Tools::ConvertMeterToSim( call.GetParameter( 1 ).ToFloat() );

    assert( pAgentToSupport != 0 );

    const MT_Vector2D& vUnitToSupportPos = pAgentToSupport->GetPion().GetRole< PHY_RolePion_Location >().GetPosition ();
    const MIL_Fuseau& fuseau             = callerAgent.GetOrderManager().GetFuseau();

    MT_Vector2D  vDirLooked;
    pAgentToSupport->GetPion().GetRole< PHY_RolePion_Perceiver >().GetMainPerceptionDirection( vDirLooked );

    vDirLooked.Rotate90ClockWise();

    MT_Vector2D* pResult = new MT_Vector2D();
    call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypePoint() );

    // $$$$ Position dans le fuseau : devrait être générique
    MT_Vector2D vSupportPos1( vUnitToSupportPos + vDirLooked * rDist );
    if( fuseau.IsInside( vSupportPos1 ) )
    {
        *pResult = vSupportPos1;
        return;
    }

    MT_Vector2D vSupportPos2( vUnitToSupportPos + vDirLooked * -rDist );
    if( fuseau.IsInside( vSupportPos2 ) )
    {
        *pResult = vSupportPos2;
        return;
    }

    MT_Line line1( vUnitToSupportPos, vSupportPos1 );
    MT_Line line2( vUnitToSupportPos, vSupportPos2 );

    TER_DistanceLess cmpLeft( vUnitToSupportPos );
    T_PointSet collisions( cmpLeft );

    fuseau.IntersectWithBorder( line1, collisions );
    fuseau.IntersectWithBorder( line2, collisions );
    if( collisions.empty() )
    {
        *pResult = vSupportPos1; //$$$ POURRI
        return;
    }

    *pResult = *collisions.begin();
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeAmbushPosition
// Created: NLD 2003-11-05
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeAmbushPosition( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 1 ) ) );

    MT_Vector2D* pAmbushPosition  = call.GetParameter( 0 ).ToUserPtr( pAmbushPosition  );
    MT_Vector2D* pRetreatPosition = call.GetParameter( 1 ).ToUserPtr( pRetreatPosition );
    MT_Float     rDist            = MIL_Tools::ConvertMeterToSim( call.GetParameter( 2 ).ToFloat() );

    assert( pAmbushPosition  );
    assert( pRetreatPosition );

    MT_Vector2D* pResult = new MT_Vector2D();
    call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypePoint() );

    MT_Vector2D vDirAmbushPos = callerAgent.GetOrderManager().GetDirDanger();
    vDirAmbushPos.Rotate90ClockWise();
    vDirAmbushPos.Normalize();

    MT_Vector2D vAmbushPos1( *pAmbushPosition + vDirAmbushPos *  rDist );
    MT_Vector2D vAmbushPos2( *pAmbushPosition + vDirAmbushPos * -rDist );

    if( pRetreatPosition->SquareDistance( vAmbushPos1 ) < pRetreatPosition->SquareDistance( vAmbushPos2 ) )
        *pResult = vAmbushPos1;
    else
        *pResult = vAmbushPos2;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeSafetyPosition
// Created: NLD 2004-04-28 (Copied from shit's JDY code)
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeSafetyPosition( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( call.GetParameter( 0 ) ) );

    DEC_Knowledge_Agent* pKnowledgeEnnemy = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledgeEnnemy )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return;
    }

    MT_Float rMinDistance = MIL_Tools::ConvertMeterToSim( call.GetParameter( 1 ).ToFloat() );

    // Position de l'ennemi
    const MT_Vector2D& vEnnemiPos  = pKnowledgeEnnemy->GetPosition();

    MT_Vector2D vDirEniToAmi = ( callerAgent.GetRole< PHY_RolePion_Location >().GetPosition() - vEnnemiPos).Normalize();
    if( vDirEniToAmi.IsZero() )
        vDirEniToAmi = -callerAgent.GetOrderManager().GetDirDanger();

    MT_Vector2D vSafetyPos = vEnnemiPos + vDirEniToAmi * rMinDistance;

    TER_World::GetWorld().ClipPointInsideWorld( vSafetyPos );

    MT_Vector2D* pResult = new MT_Vector2D( vSafetyPos );
    call.GetResult().SetValue( pResult, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeSafetyPositionWithPopulation
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeSafetyPositionWithPopulation( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeConnaissancePopulation( call.GetParameter( 0 ) ) );

    DEC_Knowledge_Population* pKnowledge = DEC_FunctionsTools::GetKnowledgePopulationFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return;
    }

    MT_Float rMinDistance = MIL_Tools::ConvertMeterToSim( call.GetParameter( 1 ).ToFloat() );

    MT_Vector2D* pResult = new MT_Vector2D( pKnowledge->GetSafetyPosition( callerAgent, rMinDistance ) );
    call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeSafetyPositionWithObjective
// Created: NLD 2004-04-28
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeSafetyPositionWithObjective( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint            ( call.GetParameter( 2 ) ) );

    DEC_Knowledge_Agent* pKnowledgeEnnemy = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledgeEnnemy )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return;
    }

    MT_Float     rMinDistance = MIL_Tools::ConvertMeterToSim( call.GetParameter( 1 ).ToFloat() );
    MT_Vector2D* pObjective   = call.GetParameter( 2 ).ToUserPtr( pObjective );
    assert( pObjective );

    const MT_Vector2D& vEnnemiPos  = pKnowledgeEnnemy->GetPosition();

    MT_Vector2D vDirEniToAmi       = ( callerAgent.GetRole< PHY_RolePion_Location >().GetPosition() - vEnnemiPos).Normalize();
    MT_Vector2D vDirEniToObjective = ( *pObjective - vEnnemiPos ).Normalize();

    MT_Vector2D vSafetyPos;
    if( vDirEniToAmi.IsZero() )
    {
        vSafetyPos = vEnnemiPos + vDirEniToObjective * rMinDistance;
    }
    else
    {
        const MT_Float rAngle = Angle( vDirEniToAmi, vDirEniToObjective );
        vSafetyPos = vEnnemiPos + vDirEniToAmi.Rotate( rAngle  * -0.5 ) * rMinDistance;
    }

    TER_World::GetWorld().ClipPointInsideWorld( vSafetyPos );

    MT_Vector2D* pResult = new MT_Vector2D( vSafetyPos );
    call.GetResult().SetValue( pResult, &DEC_Tools::GetTypePoint() );
}


//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeNearestFuseauEntryPoint
// Created: NLD 2003-04-24
//-----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeNearestFuseauEntryPoint( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    MT_Vector2D* pResult = new MT_Vector2D();
    callerAgent.GetOrderManager().GetFuseau().ComputeEntryPoint( callerAgent.GetRole< PHY_RolePion_Location >().GetPosition(), *pResult );
    call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypePoint() );
}


// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeChantierPosition
// Created: NLD 2004-05-17
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeChantierPosition( DIA_Call_ABC& /*call*/, const MIL_AgentPion& /*callerAgent*/ )
{
    assert( false ); 
}

// =============================================================================
// ASA
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePosDeploiementASAOmni
// Created: JVT 2005-02-15
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputePosDeploiementASAOmni( DIA_Call_ABC& call, const MIL_Automate& automate )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 1 ) ) );

          int          nNbrPos = (int )call.GetParameter( 0 ).ToFloat  ();
          MT_Vector2D* pCenter =       call.GetParameter( 1 ).ToUserPtr( pCenter );
    const MT_Float     rRadius = MIL_Tools::ConvertMeterToSim( call.GetParameter( 2 ).ToFloat() );

    if( nNbrPos <= 0 )
        return;

    T_PointVector* pResult = new T_PointVector();
    pResult->reserve( nNbrPos );
    call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypeListePoints() );

    const MT_Float    rAngle = 2. * MT_PI / nNbrPos;
          MT_Vector2D vDir = automate.GetOrderManager().GetDirDanger() * rRadius;

    while ( nNbrPos-- )
    {
        pResult->push_back( *pCenter + vDir );
        vDir.Rotate( rAngle );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePosDeploiementASANasse
// Created: JVT 2005-02-15
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputePosDeploiementASANasse( DIA_Call_ABC& call, const MIL_Automate& )
{
    assert( DEC_Tools::CheckTypePoint    ( call.GetParameter( 1 ) ) );
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 5 ) ) );

          int          nNbrPos        = (int )call.GetParameter( 0 ).ToFloat  ();
    const MT_Float     rSemiAngle     =       call.GetParameter( 2 ).ToFloat() * MT_PI / 360.; // ( / 360. = * 0.5 / 180., car demi-angle );
    const MT_Float     rIniDist       = MIL_Tools::ConvertMeterToSim( call.GetParameter( 3 ).ToFloat() );
    const MT_Float     rBetweenDist   = MIL_Tools::ConvertMeterToSim( call.GetParameter( 4 ).ToFloat() );
          MT_Vector2D* pMainDirection =       call.GetParameter( 5 ).ToUserPtr( pMainDirection );
          MT_Vector2D  vCenter        =      *call.GetParameter( 1 ).ToUserPtr( pMainDirection );

    assert( MT_IsZero( pMainDirection->SquareMagnitude() - 1. ) );

    if( nNbrPos <= 0 )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeListePoints() );
        return;
    }

    vCenter += rIniDist * *pMainDirection;

    const MT_Vector2D vSupport1( pMainDirection->Rotated(  rSemiAngle ) * rBetweenDist );
    const MT_Vector2D vSupport2( pMainDirection->Rotated( -rSemiAngle ) * rBetweenDist );

    T_PointVector* pResult = new T_PointVector();
    pResult->reserve( nNbrPos );
    call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypeListePoints() );

    if( nNbrPos % 2 )
    {    // cas impair : on a en plus un point au centre
        pResult->push_back( vCenter );

        if( !--nNbrPos )
            return;
    }

    assert( nNbrPos >= 2 && !( nNbrPos % 2 ) );

    pResult->push_back( vCenter + vSupport1 );
    pResult->push_back( vCenter + vSupport2 );

    for( nNbrPos -= 2; nNbrPos; nNbrPos -= 2 )
    {
        pResult->push_back( *( pResult->rbegin() + 1 ) + vSupport1 );
        pResult->push_back( *( pResult->rbegin() + 1 ) + vSupport2 );
    }
 }

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePosDeploiementASADoubleRideau
// Created: JVT 2005-02-15
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputePosDeploiementASADoubleRideau( DIA_Call_ABC& call, const MIL_Automate& )
{
    assert( DEC_Tools::CheckTypePoint    ( call.GetParameter( 1 ) ) );
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 5 ) ) );

          int          nNbrPos            = (int)call.GetParameter( 0 ).ToFloat();
          MT_Vector2D* pCenter            =      call.GetParameter( 1 ).ToUserPtr( pCenter );
    const MT_Float     rIniDist           = MIL_Tools::ConvertMeterToSim( call.GetParameter( 2 ).ToFloat() );
    const MT_Float     rBetweenLinesDist  = MIL_Tools::ConvertMeterToSim( call.GetParameter( 3 ).ToFloat() );
    const MT_Float     rBetweenPointsDist = MIL_Tools::ConvertMeterToSim( call.GetParameter( 4 ).ToFloat() );
          MT_Vector2D  vDirection         =     *call.GetParameter( 5 ).ToUserPtr( pCenter );

    assert( MT_IsZero( vDirection.SquareMagnitude() - 1. ) );

    MT_Vector2D vSupport1( vDirection.Rotated90() );
    MT_Vector2D vSupport2( -vSupport1 );

    if( nNbrPos <= 0 )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeListePoints() );
        return;
    }

    T_PointVector* pResult = new T_PointVector();
    pResult->reserve( nNbrPos );
    call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypeListePoints() );

    pResult->push_back( *pCenter + rIniDist * vDirection + ( rBetweenPointsDist * .5 ) * vSupport1 );
    if( !--nNbrPos ) return;
    pResult->push_back( *pCenter + rIniDist * vDirection + ( rBetweenPointsDist * .5 ) * vSupport2 );

    vDirection *= rBetweenLinesDist;
    vSupport1  *= rBetweenPointsDist;
    vSupport2  *= rBetweenPointsDist;

    while ( --nNbrPos )
    {
        pResult->push_back( *( pResult->rbegin() + 1 ) + vDirection );
        if( !--nNbrPos ) return;
        pResult->push_back( *( pResult->rbegin() + 1 ) + vDirection );
        if( !--nNbrPos ) return;
        pResult->push_back( *( pResult->rbegin() + 3 ) + vSupport1 );
        if( !--nNbrPos ) return;
        pResult->push_back( *( pResult->rbegin() + 3 ) + vSupport2 );
    }
}


// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePointsBeforeLima
// Created: NLD 2004-05-25
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputePointsBeforeLima( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypeLima( call.GetParameter( 0 ) ) );

    MIL_LimaOrder*    pLima           = callerAutomate.GetOrderManager().FindLima( (uint)call.GetParameter( 0 ).ToPtr() );
    MT_Float          rDistBeforeLima = MIL_Tools::ConvertMeterToSim( call.GetParameter( 1 ).ToFloat() );
    MT_Float          rNbPoints       = call.GetParameter( 2 ).ToFloat();

    if( !pLima )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeListePoints() );
        return;
    }

    T_PointVector* pResult = new T_PointVector();

    bool bResult = callerAutomate.GetOrderManager().GetFuseau().ComputePointsBeforeLima( *pLima, rDistBeforeLima, (uint)rNbPoints, *pResult );
    if( bResult )
        call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypeListePoints() );
    else
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeListePoints() );
        delete pResult;
    }
}

// =============================================================================
// LIGNES AVANT / ARRIERE /MIDDLE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDistanceFromMiddleLine
// Created: NLD 2003-10-01
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeDistanceFromMiddleLine( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeListePions( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePion      ( call.GetParameter( 1 ) ) );

    T_ObjectVector selPions = call.GetParameter( 0 ).ToSelection();
    if( selPions.empty() )
    {
        call.GetResult().SetValue( (float)0. );
        return;
    }

    DEC_RolePion_Decision* pReferencePion = call.GetParameter( 1 ).ToUserObject( pReferencePion );

    // Barycenter of the pions given
    MT_Vector2D vBarycenter;
    for( CIT_ObjectVector itPion = selPions.begin(); itPion != selPions.end(); ++itPion )
        vBarycenter += static_cast< DEC_RolePion_Decision& >(**itPion).GetPion().GetRole< PHY_RolePion_Location >().GetPosition();
    vBarycenter /= selPions.size();

    const MT_Vector2D& vReferencePionPosition = pReferencePion->GetPion().GetRole< PHY_RolePion_Location >().GetPosition();

    //
    const MIL_Fuseau& fuseau = pReferencePion->GetPion().GetOrderManager().GetFuseau();
    MT_Float rDist;
    if( fuseau.IsNull() )
        rDist = vReferencePionPosition.Distance( vBarycenter );
    else
    {
        // Project it on the fuseau global direction
        MT_Vector2D vProjectedPionsBarycenter_ = fuseau.GetGlobalDirection().ClosestPointOnLine( vBarycenter );
        rDist = vReferencePionPosition.Distance( vProjectedPionsBarycenter_ );
    }
    call.GetResult().SetValue( MIL_Tools::ConvertSimToMeter( rDist ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::StartComputingFrontAndBackLines
// Created: NLD 2004-10-19
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::StartComputingFrontAndBackLines( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    T_ObjectVector sel = call.GetParameter( 0 ).ToSelection();
    std::vector< MIL_AgentPion*> pions;
    pions.reserve( sel.size() );
    for( IT_ObjectVector it = sel.begin(); it != sel.end(); ++it )
        pions.push_back( &static_cast< DEC_RolePion_Decision& >( **it ).GetPion() );
    DEC_FrontAndBackLinesComputer* pComputer = new DEC_FrontAndBackLinesComputer( callerAutomate, pions );
    call.GetResult().SetValue( (void*)pComputer, &DEC_Tools::GetTypeCalculLignesAvantArriere() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::StartComputingAutomatFrontAndBackLines
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::StartComputingAutomatFrontAndBackLines( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    std::vector< MIL_Automate*> automats;
    T_ObjectVector sel = call.GetParameter( 0 ).ToSelection();
    automats.reserve( sel.size() );
    for( IT_ObjectVector it = sel.begin(); it != sel.end(); ++it )
        automats.push_back( &static_cast< DEC_AutomateDecision& >( **it ).GetAutomate() );
    DEC_FrontAndBackLinesComputer* pComputer = new DEC_FrontAndBackLinesComputer( callerAutomate, automats );
    call.GetResult().SetValue( (void*)pComputer, &DEC_Tools::GetTypeCalculLignesAvantArriere() );
}                           

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::StopComputingFrontAndBackLines
// Created: NLD 2004-10-19
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::StopComputingFrontAndBackLines( DIA_Call_ABC& call, const MIL_Automate& /*callerAutomate*/ )
{
    assert( DEC_Tools::CheckTypeCalculLignesAvantArriere( call.GetParameter( 0 ) ) );
//    DEC_FrontAndBackLinesComputer* pComputer = call.GetParameter( 0 ).ToUserPtr( pComputer );
//    delete pComputer;
//    call.GetParameter( 0 ).SetValue( (void*)0, &DEC_Tools::GetTypeCalculLignesAvantArriere() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDistanceFromFrontLine
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeDistanceFromFrontLine( DIA_Call_ABC& call, const MIL_Automate& /*callerAgent*/ )
{
    assert( DEC_Tools::CheckTypeCalculLignesAvantArriere( call.GetParameter( 0 ) ) );

    DEC_FrontAndBackLinesComputer* pComputer = call.GetParameter( 0 ).ToUserPtr( pComputer );
    assert( pComputer );

    MT_Float rDist = 0;
    if( DEC_Tools::CheckTypePion( call.GetParameter( 1 ) ) )
    {
        DEC_RolePion_Decision* pPion = call.GetParameter( 1 ).ToUserObject( pPion );
        assert( pPion );
        rDist = pComputer->ComputeDistanceFromFrontLine( pPion->GetPion().GetRole< PHY_RolePion_Location >().GetPosition() );
    }
    else if( DEC_Tools::CheckTypeAutomate( call.GetParameter( 1 ) ) )
    {
        DEC_AutomateDecision* pAutomate = call.GetParameter( 1 ).ToUserObject( pAutomate );
        assert( pAutomate );
        MT_Vector2D barycenter;
        if( pAutomate->GetAutomate().GetAlivePionsBarycenter( barycenter ) )
            rDist = pComputer->ComputeDistanceFromFrontLine( barycenter );
    }
    else
        assert( false );

    call.GetResult().SetValue( MIL_Tools::ConvertSimToMeter( rDist ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDistancePointFromFrontLine
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeDistancePointFromFrontLine( DIA_Call_ABC& call, const MIL_Automate& /*callerAgent*/ )
{
    assert( DEC_Tools::CheckTypeCalculLignesAvantArriere( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 1 ) ) );

    DEC_FrontAndBackLinesComputer* pComputer = call.GetParameter( 0 ).ToUserPtr( pComputer );
    assert( pComputer );

    const MT_Vector2D* pPoint = call.GetParameter( 1 ).ToUserPtr( pPoint );
    assert( pPoint );

    MT_Float rDist = pComputer->ComputeDistanceFromFrontLine( *pPoint );
    call.GetResult().SetValue( MIL_Tools::ConvertSimToMeter( rDist ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDistanceFromBackLine
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeDistanceFromBackLine( DIA_Call_ABC& call, const MIL_Automate& /*callerAgent*/ )
{
    assert( DEC_Tools::CheckTypeCalculLignesAvantArriere( call.GetParameter( 0 ) ) );

    DEC_FrontAndBackLinesComputer* pComputer = call.GetParameter( 0 ).ToUserPtr( pComputer );
    assert( pComputer );

    MT_Float rDist = 0;
    if( DEC_Tools::CheckTypePion( call.GetParameter( 1 ) ) )
    {
        DEC_RolePion_Decision* pPion = call.GetParameter( 1 ).ToUserObject( pPion );
        assert( pPion );
        rDist = pComputer->ComputeDistanceFromBackLine( pPion->GetPion().GetRole< PHY_RolePion_Location >().GetPosition() );
    }
    else if( DEC_Tools::CheckTypeAutomate( call.GetParameter( 1 ) ) )
    {
        DEC_AutomateDecision* pAutomate = call.GetParameter( 1 ).ToUserObject( pAutomate );
        assert( pAutomate );
        MT_Vector2D barycenter;
        if( pAutomate->GetAutomate().GetAlivePionsBarycenter( barycenter ) )
            rDist = pComputer->ComputeDistanceFromBackLine( barycenter );
    }
    else
        assert( false );

    call.GetResult().SetValue( MIL_Tools::ConvertSimToMeter( rDist ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDistancePointFromBackLine
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeDistancePointFromBackLine( DIA_Call_ABC& call, const MIL_Automate& /*callerAgent*/ )
{
    assert( DEC_Tools::CheckTypeCalculLignesAvantArriere( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 1 ) ) );

    DEC_FrontAndBackLinesComputer* pComputer = call.GetParameter( 0 ).ToUserPtr( pComputer );
    assert( pComputer );

    const MT_Vector2D* pPoint = call.GetParameter( 1 ).ToUserPtr( pPoint );
    assert( pPoint );

    MT_Float rDist = pComputer->ComputeDistanceFromBackLine( *pPoint );
    call.GetResult().SetValue( MIL_Tools::ConvertSimToMeter( rDist ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ClipLocalisationInFuseau
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::ClipLocalisationInFuseau( const TER_Localisation& localisation, const MIL_Fuseau& fuseau, TER_Localisation& clippedLocalisation )
{
    // Clippe le polygone dans le fuseau
    T_PointVector clippedPointVector;
    localisation.GetPointsClippedByPolygon( fuseau, clippedPointVector );

    if( clippedPointVector.empty() )
        return false;

    clippedLocalisation.Reset( clippedPointVector ); //$$$ NAZE
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitLocalisation
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::SplitLocalisation( const TER_Localisation& localisation, uint nNbrParts, DIA_Variable_ABC& diaReturnCode, DIA_Variable_ObjectList& result )
{
    assert( nNbrParts > 0 );

    T_LocalisationPtrVector splitLocVector;
    localisation.Split( nNbrParts, splitLocVector );

    if( splitLocVector.size() != nNbrParts )
        diaReturnCode.SetValue( eWarning_DecoupageIncomplet );
    else
        diaReturnCode.SetValue( eNoError );

    result.SetValueUserType( splitLocVector, DEC_Tools::GetTypeLocalisation() );
}


// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitLocalisation
// Created: JVT 2004-11-04
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::SplitLocalisation( const TER_Localisation& localisation, MT_Vector2D vOrigin, const MT_Vector2D& vDirection, MT_Float rSectionLength, DIA_Variable_ObjectList& diaObjectList )
{
    T_LocalisationPtrVector splitLocVector;

    const MT_Vector2D vTranslation   = rSectionLength * vDirection;
    const MT_Vector2D vLineDirection = vDirection.Rotated90();

    MT_Droite backBound  ( vOrigin, vOrigin + vLineDirection );
    vOrigin += vTranslation;
    MT_Droite frontBound ( vOrigin, vOrigin + vLineDirection );

    while( true )
    {
        T_PointVector points;
        localisation.GetPointsClippedBetweenTwoLines( backBound, frontBound, points );

        if( points.empty() )
            break;

        splitLocVector.push_back( new TER_Localisation( TER_Localisation::ePolygon, points ) );

        vOrigin += vTranslation;
        backBound  = frontBound;
        frontBound.MT_Droite::MT_Droite( vOrigin, vOrigin + vLineDirection );
    };

    diaObjectList.SetValueUserType( splitLocVector, DEC_Tools::GetTypeLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeLocalisationBarycenter
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeLocalisationBarycenter( DIA_Call_ABC& diaCall )
{
    assert( DEC_Tools::CheckTypeLocalisation( diaCall.GetParameter( 0 ) ) );
    TER_Localisation* pLocalisation = diaCall.GetParameter( 0 ).ToUserPtr( pLocalisation );
    assert( pLocalisation );

    MT_Vector2D* pBarycenter = new MT_Vector2D( MT_ComputeBarycenter( pLocalisation->GetPoints() ) );
    assert( TER_World::GetWorld().IsValidPosition( *pBarycenter ) );
    diaCall.GetResult().SetValue( (void*)pBarycenter, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeMeanDirection
// Created: JVT 2005-01-25
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeMeanDirection( DIA_Call_ABC& diaCall )
{
    assert( DEC_Tools::CheckTypeListeDirection( diaCall.GetParameter( 0 ) ) );

    const T_ObjectVariableVector& selection = static_cast< const DIA_Variable_ObjectList& >( diaCall.GetParameter( 0 ) ).GetContainer();
          MT_Vector2D*            pResult   = new MT_Vector2D( 0., 0. );

    if( selection.empty() )
    {
        assert( !"Should not be called when empty !" );
        pResult->rX_ = 0.;
        pResult->rY_ = 1.;
    }
    else
    {
        for( CIT_ObjectVariableVector it = selection.begin(); it != selection.end(); ++it )
            *pResult += *(*it)->ToUserPtr( pResult );

        if( pResult->IsZero() )
            *pResult = *selection.front()->ToUserPtr( pResult );
        else
            pResult->Normalize();
    }

    diaCall.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypeDirection() );
}


// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeKnowledgeAgentBarycenter
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeKnowledgeAgentBarycenter( DIA_Call_ABC& call, const MIL_AgentPion& caller )
{
    assert( DEC_Tools::CheckTypeListeConnaissancesAgent( call.GetParameter( 0 ) ) );

    const T_ObjectVariableVector& selection = static_cast< const DIA_Variable_ObjectList& >( call.GetParameter( 0 ) ).GetContainer();
          MT_Vector2D*            pResult   = new MT_Vector2D( 0., 0. );

    uint nNbr = 0;
    for( CIT_ObjectVariableVector it = selection.begin(); it != selection.end(); ++it )
    {
        const DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( **it, caller.GetKnowledgeGroup() );
        if( pKnowledge )
        {
            *pResult += pKnowledge->GetPosition();
            ++ nNbr;
        }
    }

    if( nNbr )
        *pResult /= (MT_Float)nNbr;

    call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeCoverPosition
// Created: JVT 2005-02-11
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeCoverPosition( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeListePions( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeDirection ( call.GetParameter( 1 ) ) );

    const T_ObjectVector pions = call.GetParameter( 0 ).ToSelection();

    if( pions.empty() )
    {
        assert( !"La liste ne doit pas etre vide" );
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return;
    }

    MT_Vector2D* pDirection = call.GetParameter( 1 ).ToUserPtr( pDirection );
    assert( pDirection );

    // calcul de la première ligne de support ( perpendiculaire à la direction passant par le pion le plus avancé )
    const DEC_RolePion_Decision* pFrontestPion = GetFrontestPion( pions, *pDirection );
    assert( pFrontestPion );
    const MT_Vector2D& vFrontestPionPosition = pFrontestPion->GetPion().GetRole< PHY_RolePion_Location >().GetPosition();
    const MT_Droite support1( vFrontestPionPosition, vFrontestPionPosition + pDirection->Rotated90() );

    // calcul de la seconde ligne de support ( dans la direction passant par le barycentre des pions )
    MT_Vector2D vOrigin;
    for( CIT_ObjectVector it = pions.begin(); it != pions.end(); ++it )
        vOrigin += static_cast< DEC_RolePion_Decision* >( *it )->GetPion().GetRole< PHY_RolePion_Location >().GetPosition();
    vOrigin /= (MT_Float)pions.size();
    const MT_Droite support2( vOrigin, vOrigin + *pDirection );

    // calcul du point de couverture
    MT_Vector2D* pRes = new MT_Vector2D();
    support1.Intersect2D( support2, *pRes );

    *pRes += ( *pDirection * MIL_Tools::ConvertMeterToSim( call.GetParameter( 2 ).ToFloat() ) );

    call.GetResult().SetValue( (void*)pRes, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeRandomPointOnCircle
// Created: JVT 2005-02-16
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeRandomPointOnCircle( DIA_Call_ABC& call )
{
    static MT_Random rand;

    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );

    MT_Vector2D* pResult = new MT_Vector2D( 0., 1. );
    pResult->Rotate( rand.rand_io( 0., 2. * MT_PI ) );
    *pResult *= ( MIL_Tools::ConvertMeterToSim( call.GetParameter( 1 ).ToFloat() ) );
    *pResult += *call.GetParameter( 0 ).ToUserPtr( pResult );
    TER_World::GetWorld().ClipPointInsideWorld( *pResult );
    call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeRandomPointInCircle
// Created: NLD 2004-04-28
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeRandomPointInCircle( DIA_Call_ABC& call )
{
    static MT_Random randomGenerator;

    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );

    MT_Vector2D* pCenter  = call.GetParameter( 0 ).ToUserPtr( pCenter );
    MT_Float     rRadius_ = MIL_Tools::ConvertMeterToSim( call.GetParameter( 1 ).ToFloat() );
    assert( pCenter );

    // retrieve a random position in the circle (vCenter_,rRadius_)
    MT_Float rAlpha = randomGenerator.rand_ii( -MT_PI, MT_PI );
    MT_Float rMod   = randomGenerator.rand_oi();

    MT_Vector2D* pRandomPosition = new MT_Vector2D( *pCenter );
    (*pRandomPosition) += MT_Vector2D( rMod * rRadius_ * cos( rAlpha ), rMod * rRadius_ * sin( rAlpha ) );

    TER_World::GetWorld().ClipPointInsideWorld( *pRandomPosition );
    call.GetResult().SetValue( pRandomPosition, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeLocalisationArea
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeLocalisationArea( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    const TER_Localisation* pLoc = call.GetParameter( 0 ).ToUserPtr( pLoc );
    call.GetResult().SetValue( (float)MIL_Tools::ConvertSimToMeterSquare( pLoc->GetArea() ) );
}

// =============================================================================
// Interception
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetInterceptionPoint
// Created: JVT 2005-02-17
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::GetInterceptionPoint( const MT_Vector2D& vToInterceptPosition, const MT_Vector2D& vToInterceptSpeed, const MT_Vector2D& vInterceptingPosition, MT_Float rInterceptingSpeed, MT_Vector2D& result )
{
    // Soit les équations suivantes ( inconnues = vxnB, vynB (vercteur direction normé de B) et t (temps de l'interception) ) :
    // vToInterceptPosition.rX_ + vToInterceptSpeed.rX_ * t = vInterceptingPosition.rX_ + rInterceptingSpeed * vxnB * t
    // vToInterceptPosition.rY_ + vToInterceptSpeed.rY_ * t = vInterceptingPosition.rY_ + rInterceptingSpeed * vynB * t
    //                            vxnB * vxnB + vynB * vynB = 1
    //
    // Les solutions pour t ( seule variable qui nous intéresse ) sont les racines de rA * X^2  + rB * X + rC avec :
    const MT_Float rA = vToInterceptSpeed.rX_ * vToInterceptSpeed.rX_ + vToInterceptSpeed.rY_ * vToInterceptSpeed.rY_ - rInterceptingSpeed * rInterceptingSpeed;
    const MT_Float rB = 2. * ( vToInterceptPosition.rY_ * vToInterceptSpeed.rY_ + vToInterceptPosition.rX_ * vToInterceptSpeed.rX_ - vToInterceptSpeed.rY_ * vInterceptingPosition.rY_ - vToInterceptSpeed.rX_ * vInterceptingPosition.rX_ );
    const MT_Float rC = vToInterceptPosition.rY_ * vToInterceptPosition.rY_ + vInterceptingPosition.rY_ * vInterceptingPosition.rY_ - 2. * vToInterceptPosition.rY_ * vInterceptingPosition.rY_ + vInterceptingPosition.rX_ * vInterceptingPosition.rX_ - 2. * vToInterceptPosition.rX_ * vInterceptingPosition.rX_ + vToInterceptPosition.rX_ * vToInterceptPosition.rX_;

    const MT_Float delta = rB * rB - 4. * rA * rC;

    if( rA == 0. || delta < 0. )
        return false;

    MT_Float t = ( std::sqrt( delta ) - rB ) / ( 2. * rA );

    if( t < 0. && ( t = ( std::sqrt( delta ) + rB ) / ( -2. * rA ) ) < 0. )
        return false;

    result.rX_ = vToInterceptPosition.rX_ + vToInterceptSpeed.rX_ * t;
    result.rY_ = vToInterceptPosition.rY_ + vToInterceptSpeed.rY_ * t;

    return TER_World::GetWorld().IsValidPosition( result );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetInterceptionPosition
// Created: JVT 2005-02-17
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::GetInterceptionPosition( DIA_Call_ABC& call, const MIL_AgentPion& caller )
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint            ( call.GetParameter( 1 ) ) );

    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return;
    }

    MT_Vector2D* pInterceptingPosition = call.GetParameter( 1 ).ToUserPtr( pInterceptingPosition );
    const MT_Float     rInterceptingSpeed    = MIL_Tools::ConvertSpeedMosToSim( 3.6 /*m.s-1 => km.h-1*/ * call.GetParameter( 2 ).ToFloat() );

    assert( pInterceptingPosition );

    MT_Vector2D vInterceptionPosition;
    if( !GetInterceptionPoint( pKnowledge->GetPosition(), pKnowledge->GetDirection() * pKnowledge->GetSpeed(), *pInterceptingPosition, rInterceptingSpeed, vInterceptionPosition ) )
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
    else
        call.GetResult().SetValue( new MT_Vector2D( vInterceptionPosition ), &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreateCircleLocalisation
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::CreateCircleLocalisation( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );

    const MT_Vector2D* pCenter = call.GetParameter( 0 ).ToUserPtr( pCenter );
    const MT_Float     rRadius = MIL_Tools::ConvertMeterToSim( call.GetParameter( 1 ).ToFloat() );
    call.GetResult().SetValue( new TER_Localisation( *pCenter, rRadius ), &DEC_Tools::GetTypeLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsPionCoordinated
// Created: NLD 2005-03-18
// -----------------------------------------------------------------------------
static
bool IsPionCoordinated( const MT_Vector2D& vPosToTest, const T_ObjectVector& coordinatedPions, const MT_Float rMinDist )
{
    for( CIT_ObjectVector itC = coordinatedPions.begin(); itC != coordinatedPions.end(); ++itC )
    {
        const MT_Vector2D& vPosTmp = static_cast< DEC_RolePion_Decision& >(**itC).GetPion().GetRole< PHY_RolePion_Location >().GetPosition();
        if( vPosToTest.Distance( vPosTmp ) <= rMinDist )
            return true;
    }
    return false;
}

void DEC_GeometryFunctions::IsPionCoordinated( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypePion      ( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeListePions( call.GetParameter( 1 ) ) );

    const MT_Float rMinDist = MIL_Tools::ConvertMeterToSim( call.GetParameter( 2 ).ToFloat() );

    T_ObjectVector coordinatedPions;
    T_ObjectVector notCoordinatedPions = call.GetParameter( 1 ).ToSelection();
    coordinatedPions.push_back( call.GetParameter( 0 ).ToObject() );

    bool bDummy = true;
    while( bDummy )
    {
        bDummy = false;
        for( IT_ObjectVector itNC = notCoordinatedPions.begin(); itNC != notCoordinatedPions.end(); )
        {
            const MT_Vector2D& vPosToTest = static_cast< DEC_RolePion_Decision& >(**itNC).GetPion().GetRole< PHY_RolePion_Location >().GetPosition();
            if( ::IsPionCoordinated( vPosToTest, coordinatedPions, rMinDist ) )
            {
                bDummy = true;
                coordinatedPions.push_back( *itNC );
                itNC = notCoordinatedPions.erase( itNC );
            }
            else
                ++itNC;
        }
    }
    call.GetResult().SetValue( notCoordinatedPions );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitListPoints
// Created: NLD 2005-04-04
// -----------------------------------------------------------------------------
static
T_PointVector* SplitListPoints( const T_PointVector& points, MT_Float rNbrParts )
{
    if( points.size() < 2 )
        return 0;

    MT_Polyline polyLine( points );

    T_PointVector* pResult = new T_PointVector();
    if( rNbrParts <= 0. )
        pResult->push_back( polyLine.GetPointAt( polyLine.Magnitude() / 2. ) );
    else
    {
        pResult->reserve( (uint)rNbrParts + 1 );
        const MT_Float rPartSize = polyLine.Magnitude() / rNbrParts;
        MT_Float rDist = 0.;
        for( uint i = 0; i < rNbrParts + 1; ++i, rDist+= rPartSize )
            pResult->push_back( polyLine.GetPointAt( rDist ) );
    }

    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitListPoints
// Created: NLD 2005-04-04
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::SplitListPoints( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeListePoints( call.GetParameter( 0 ) ) );

    T_PointVector* pListPoint = call.GetParameter( 0 ).ToUserPtr( pListPoint );
    const MT_Float rNbrParts  = call.GetParameter( 1 ).ToFloat();
    assert( pListPoint );

    call.GetResult().SetValue( ::SplitListPoints( *pListPoint, rNbrParts ), &DEC_Tools::GetTypeListePoints() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitPath
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::SplitPath( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeItineraire( call.GetParameter( 0 ) ) );

    DEC_Agent_Path* pPath     = call.GetParameter( 0 ).ToUserPtr( pPath );
    const MT_Float  rNbrParts = call.GetParameter( 1 ).ToFloat();
    assert( pPath );

    T_PointVector points;
    const DEC_Agent_Path::T_PathPointList& pathPoints = pPath->GetResult();
    points.reserve( pathPoints.size() );
    for( DEC_Agent_Path::CIT_PathPointList it = pathPoints.begin(); it != pathPoints.end(); ++it )
        points.push_back( (**it).GetPos() );

    call.GetResult().SetValue( ::SplitListPoints( points, rNbrParts ), &DEC_Tools::GetTypeListePoints() );
}

namespace
{
    float ComputeClosedTerrainRatio( const TER_Localisation& location )
    {
        uint nForestSurface = 0, nEmptySurface  = 0, nUrbanSurface  = 0;
        MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetVisionObjectsInSurface( location, nEmptySurface, nForestSurface, nUrbanSurface );
        return float( nForestSurface + nUrbanSurface ) / float( nForestSurface + nUrbanSurface + nEmptySurface );
    }

    float ComputeOpenTerrainRatio( const TER_Localisation& location )
    {
        return 1.f - ComputeClosedTerrainRatio( location );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeClosedTerrainRatioInZone
// Created: NLD 2007-04-13
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeClosedTerrainRatioInZone( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeFuseau( call.GetParameter( 0 ) ) || DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    if( DEC_Tools::CheckTypeFuseau( call.GetParameter( 0 ) ) )
    {
        if( const MIL_Fuseau* pFuseau = call.GetParameter( 0 ).ToUserPtr( pFuseau ) )
            call.GetResult().SetValue( pFuseau->ComputeClosedTerrainRatio() );
    }
    else if( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) )
    {
        if( const TER_Localisation* location = call.GetParameter( 0 ).ToUserPtr( location ) )
            call.GetResult().SetValue( ComputeClosedTerrainRatio( *location ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeOpenTerrainRatioInZone
// Created: SBO 2007-11-14
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeOpenTerrainRatioInZone( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeFuseau( call.GetParameter( 0 ) ) || DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    if( DEC_Tools::CheckTypeFuseau( call.GetParameter( 0 ) ) )
    {
        if( const MIL_Fuseau* pFuseau = call.GetParameter( 0 ).ToUserPtr( pFuseau ) )
            call.GetResult().SetValue( pFuseau->ComputeOpenTerrainRatio() );
    }
    else if( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) )
    {
        if( const TER_Localisation* location = call.GetParameter( 0 ).ToUserPtr( location ) )
            call.GetResult().SetValue( ComputeOpenTerrainRatio( *location ) );
    }
}


namespace {

    bool CompareTerrainOpening( DIA_Variable_ABC* dia1, DIA_Variable_ABC* dia2 )
    {
        if( DEC_Tools::CheckTypeFuseau( *dia1 ) && DEC_Tools::CheckTypeFuseau( *dia2 ) )
        {
            MIL_Fuseau* pFuseau1 = dia1->ToUserPtr( pFuseau1 );
            MIL_Fuseau* pFuseau2 = dia2->ToUserPtr( pFuseau2 );
            return pFuseau1->ComputeOpenTerrainRatio() < pFuseau2->ComputeOpenTerrainRatio();
        }
        else if( DEC_Tools::CheckTypeLocalisation( *dia1 ) && DEC_Tools::CheckTypeLocalisation( *dia2 ) )
        {
            TER_Localisation* location1 = dia1->ToUserPtr( location1 );
            TER_Localisation* location2 = dia2->ToUserPtr( location2 );
            return ComputeOpenTerrainRatio( *location1 ) < ComputeOpenTerrainRatio( *location2 );
        }
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SortZonesAccordingToTerrainOpening
// Created: NLD 2007-04-13
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::SortZonesAccordingToTerrainOpening( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeListeFuseaux( call.GetParameter( 0 ) ) || DEC_Tools::CheckTypeListeLocalisations( call.GetParameter( 0 ) ) );

    call.GetResult() = call.GetParameter( 0 );

    T_ObjectVariableVector& zones = const_cast< T_ObjectVariableVector& >( static_cast< DIA_Variable_ObjectList& >( call.GetResult() ).GetContainer() );
    std::sort( zones.begin(), zones.end(), CompareTerrainOpening );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsLocalisationInFuseau
// Created: NLD 2007-04-17
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::IsLocalisationInFuseau( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeFuseau      ( call.GetParameter( 1 ) ) );

    const TER_Localisation* pLocalisation = call.GetParameter( 0 ).ToUserPtr( pLocalisation );
    const MIL_Fuseau*       pFuseau       = call.GetParameter( 1 ).ToUserPtr( pFuseau );
    assert( pLocalisation && pFuseau );

    call.GetResult().SetValue( pLocalisation->IsIntersecting( *pFuseau ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ConvertFuseauToLocalisation
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ConvertFuseauToLocalisation( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeFuseau( call.GetParameter( 0 ) ) );

    const MIL_Fuseau* pFuseau = call.GetParameter( 0 ).ToUserPtr( pFuseau );
    assert( pFuseau );

    TER_Localisation* pLocalisation = new TER_Localisation( *pFuseau );
    call.GetResult().SetValue( pLocalisation, &DEC_Tools::GetTypeLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeAutomatesBarycenter
// Created: NLD 2007-04-29
// -----------------------------------------------------------------------------
MT_Vector2D DEC_GeometryFunctions::_ComputeAutomatesBarycenter( const T_ObjectVector& automates )
{
    MT_Vector2D barycenter;
    uint nNbrElt = 0;
    for( CIT_ObjectVector it = automates.begin(); it != automates.end(); ++it )
    {
        MT_Vector2D tmp;
        if( static_cast< DEC_AutomateDecision& >( **it ).GetAutomate().GetAlivePionsBarycenter( tmp ) )
        {
            barycenter += tmp;
            ++ nNbrElt;
        }
    }
    if( nNbrElt > 0 )
        barycenter /= nNbrElt;
    return barycenter;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeAutomatesBarycenter
// Created: NLD 2007-04-29
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeAutomatesBarycenter( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeListeAutomates( call.GetParameter( 0 ) ) );

    const T_ObjectVector automates = call.GetParameter( 0 ).ToSelection();
    MT_Vector2D* pBarycenter = new MT_Vector2D( _ComputeAutomatesBarycenter( automates ) );
    call.GetResult().SetValue( pBarycenter, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetNextObjectiveInFuseau
// Created: NLD 2007-05-16
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::GetNextObjectiveInFuseau( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeFuseau        ( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint         ( call.GetParameter( 1 ) ) );
    assert( DEC_Tools::CheckTypeListeObjectifs( call.GetParameter( 2 ) ) );

    const MIL_Fuseau*  pFuseau   = call.GetParameter( 0 ).ToUserPtr( pFuseau   );
    const MT_Vector2D* pRefPoint = call.GetParameter( 1 ).ToUserPtr( pRefPoint );
    if( !pFuseau )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeObjectif() );
        return;
    }

    DIA_Variable_ABC* pNextObjective = 0;
    MT_Float          rDist          = std::numeric_limits< MT_Float >::max();

    T_ObjectVariableVector& objectives = const_cast< T_ObjectVariableVector& >( static_cast< DIA_Variable_ObjectList& >( call.GetParameter( 2 ) ).GetContainer() );
    for( CIT_ObjectVariableVector it = objectives.begin(); it != objectives.end(); ++it )
    {
        const DEC_Objective* pObjective = (**it).ToUserPtr( pObjective );
        if( !pObjective->IsFlagged() && pFuseau->IsInside( pObjective->ComputerBarycenter() ) )
        {
            const MT_Float rTmp = pFuseau->ComputeAverageDistanceFromObjective( *pObjective, *pRefPoint );
            if( rTmp < rDist )
            {
                pNextObjective = *it;
                rDist         = rTmp;
            }
        }
    }

    if( pNextObjective )
        call.GetResult() = *pNextObjective;
    else
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeObjectif() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeAreaInZone
// Created: SBO 2007-12-07
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeAreaInZone( DIA_Call_ABC& call )
{
    assert( DEC_Tools::CheckTypeFuseau( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint ( call.GetParameter( 1 ) ) );
    const MIL_Fuseau*  zone   = call.GetParameter( 0 ).ToUserPtr( zone );
    const MT_Vector2D* center = call.GetParameter( 1 ).ToUserPtr( center );
    if( zone && center )
    {
        TER_Localisation* location = new TER_Localisation( *center, zone->Distance( *center, true ) );
        call.GetResult().SetValue( (void*)location, &DEC_Tools::GetTypeLocalisation() );
    }
    else
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypeLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDelayFromSchedule
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ComputeDelayFromSchedule( DIA_Call_ABC& call, float rDistanceFromScheduled, int nSchedule )
{
    assert( DEC_Tools::CheckTypeFuseau        ( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeListeAutomates( call.GetParameter( 1 ) ) );

    const MIL_Fuseau*    pFuseau   = call.GetParameter( 0 ).ToUserPtr( pFuseau );
    const T_ObjectVector automates = call.GetParameter( 1 ).ToSelection();
    assert( pFuseau );

    // Calcul vitesse moyenne de l'automate
    MT_Float rSpeed = std::numeric_limits< MT_Float >::max();
    for( CIT_ObjectVector it = automates.begin(); it != automates.end(); ++it )
    {
        const MIL_Automate& automate = static_cast< DEC_AutomateDecision& >( **it ).GetAutomate();
        rSpeed = std::min( rSpeed, automate.GetAlivePionsMaxSpeed() );
    }

    if( rDistanceFromScheduled == std::numeric_limits< MT_Float >::max() || rSpeed == 0. )
        call.GetResult().SetValue( 0.f );
    else
    {
        const MT_Float rTimeToGoToElement = 1.439 * rDistanceFromScheduled / rSpeed; //$$$ Deplacer la formule magique (Cf. PHY_ComposantePion où elle existe aussi...)
        const MT_Float rTimeLeeway        = 1.439 * 2000. / rSpeed;

        // Valeur de retour : = 0 : en avance, ou à 2km de la lima
        //                    = 1 : en retard
        //              entre les 2 : marge de sécurité

        const MT_Float rDelay = nSchedule - ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + rTimeToGoToElement );
        if( rDelay < 0 )
            call.GetResult().SetValue( 1.f );
        else
            call.GetResult().SetValue( (float)( 1. - std::min( 1., rDelay / rTimeLeeway ) ) );
    }
}