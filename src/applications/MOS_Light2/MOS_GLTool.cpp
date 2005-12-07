    // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-04 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_GLTool.cpp $
// $Author: Age $
// $Modtime: 4/07/05 11:08 $
// $Revision: 39 $
// $Workfile: MOS_GLTool.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_GLTool.h"

#include "MOS_GLToolColors.h"

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_Line.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"
#include "MOS_AgentKnowledge.h"
#include "MOS_Team.h"
#include "MOS_Gtia.h"
#include "MOS_ObjectManager.h"
#include "MOS_Object_ABC.h"
#include "MOS_ObjectKnowledge.h"
#include "MOS_Team.h"
#include "MOS_LineManager.h"
#include "MOS_TacticalLine_ABC.h"
#include "MOS_Meteo_Manager.h"
#include "MOS_Meteo_Local.h"
#include "MOS_MissionPanel.h"
#include "MOS_MissionInterface_ABC.h"
#include "MOS_WeatherPanel.h"
#include "MOS_ObjectCreationPanel.h"
#include "MOS_InfoPanel.h"
#include "MOS_AgentStatePanel.h"
#include "MOS_ReportListView.h"
#include "MOS_Report_ABC.h"
#include "MOS_ParamPath.h"
#include "MOS_ParamPathList.h"
#include "MOS_ParamLimaList.h"
#include "MOS_ParamLimits.h"
#include "MOS_ParamPoint.h"
#include "MOS_ParamAgent.h"
#include "MOS_ParamAgentList.h"
#include "MOS_ParamLocation.h"
#include "MOS_ParamLocationList.h"
#include "MOS_ParamObstacle.h"
#include "MOS_ParamObstacleList.h"
#include "MOS_DefaultMapEventHandler.h"
#include "MOS_ShapeEditorMapEventFilter.h"
#include "MOS_MainWindow.h"
#include "MOS_GL3DWidget.h"
#include "MOS_GLWidget.h"
#include "MOS_Options.h"
#include "MOS_Surface.h"
#include "MOS_RawVisionData.h"
#include "MOS_TypePion.h"
#include "MOS_ReportPanel.h"
#include "MOS_Population.h"
#include "MOS_PopulationFlow.h"
#include "MOS_PopulationConcentration.h"
#include "MOS_PopulationKnowledge.h"
#include "MOS_PopulationFlowKnowledge.h"
#include "MOS_PopulationConcentrationKnowledge.h"
#include "MOS_TypePopulation.h"
#include "MOS_LogMaintenanceConsign.h"
#include "MOS_LogMedicalConsign.h"
#include "MOS_LogSupplyConsign.h"

#include "MT_GLFont.h"
#include "MT_GLToolTip.h"
#undef min
#undef max
#include "geometry/Types.h"
#include "graphics/GraphicShape_ABC.h"
#include "graphics/DrawDetection.h"
#include "graphics/GraphicSetup.h"

DECLARE_ICON( flare );
DECLARE_ICON( smoke );
DECLARE_ICON( skull );

uint     MOS_GLTool::nFrame_ = 0;
float    MOS_GLTool::rClicksPerPix_ = 0;
MT_Rect  MOS_GLTool::viewRect_;
bool     MOS_GLTool::bSimpleMode_ = false;
MT_Float MOS_GLTool::rZRatio_     = 10;
MT_Float MOS_GLTool::rDeltaZ_     = 400;

GLushort MOS_GLTool::stipplePattern_[] = {
    0x00FF, 0x01FE, 0x03FC, 0x07F8,
    0x0FF0, 0x1FE0, 0x3FC0, 0x7F80,
    0xFF00, 0xFE01, 0xFC03, 0xF807,
    0xF00F, 0xE01F, 0xC03F, 0x807F
};

int MOS_GLTool::iconIds_[eNbrIcons];
MT_Vector2D MOS_GLTool::iconLocations_[eNbrIcons];

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-04-15
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MT_Rect& viewRect, float rClicksPerPix )
{
    viewRect_ = viewRect;
    rClicksPerPix_ = rClicksPerPix;
    ++nFrame_;
    if( nFrame_ == 1 )
    {
      iconIds_[eGasIcon]                                  = MOS_GLTool::InitializeIcon( QImage( xpm_gas ) );
        iconIds_[eAmmoIcon]                                 = MOS_GLTool::InitializeIcon( QImage( xpm_ammo ) );
        iconIds_[eNbcIcon]                                  = MOS_GLTool::InitializeIcon( QImage( xpm_nbc ) );
        iconIds_[eConstructionIcon]                         = MOS_GLTool::InitializeIcon( QImage( xpm_construction ) );
        iconIds_[eObservingIcon]                            = MOS_GLTool::InitializeIcon( QImage( xpm_observe ) );
        iconIds_[eFlareIcon]                                = MOS_GLTool::InitializeIcon( QImage( xpm_flare ) );
        iconIds_[eSmokeIcon]                                = MOS_GLTool::InitializeIcon( QImage( xpm_smoke ) );
        iconIds_[eSkullIcon]                                = MOS_GLTool::InitializeIcon( QImage( xpm_skull ) );
        iconIds_[objectTypeZoneProtegee]                    = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeZoneProtegee  ) );
        iconIds_[objectTypeZonePoserHelicoptere]            = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeZonePoserHelicoptere  ) );
        iconIds_[objectTypeZoneNBC]                         = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeZoneNBC  ) );
        iconIds_[objectTypeZoneMobiliteAmelioree]           = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeZoneMobiliteAmelioree  ) );
        iconIds_[objectTypeZoneInterditeTir]                = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeZoneInterditeTir  ) );
        iconIds_[objectTypeZoneInterditeMvt]                = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeZoneInterditeMvt  ) );
        iconIds_[objectTypeZoneImplantationLrm]             = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeZoneImplantationLrm  ) );
        iconIds_[objectTypeZoneImplantationCobra]           = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeZoneImplantationCobra  ) );
        iconIds_[objectTypeZoneImplantationCanon]           = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeZoneImplantationCanon  ) );
        iconIds_[objectTypeZoneBrouillardBrod]              = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeZoneBrouillardBrod  ) );
        iconIds_[objectTypeZoneBrouillageBromure]           = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeZoneBrouillageBromure  ) );
        iconIds_[objectTypeTerrainLargage]                  = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeTerrainLargage  ) );
        iconIds_[objectTypeSiteFranchissement]              = MOS_GLTool::InitializeIcon( QImage( objectTypeSiteFranchissement ) );
        iconIds_[objectTypeSiteDecontamination]             = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeSiteDecontamination  ) );
        iconIds_[objectTypeRota]                            = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeRota  ) );
        iconIds_[objectTypePosteTir]                        = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypePosteTir  ) );
        iconIds_[objectTypePosteControle]                   = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypePosteControle  ) );
        iconIds_[objectTypePontFlottant]                    = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypePontFlottant  ) );
        iconIds_[objectTypePlotRavitaillement]              = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypePlotRavitaillement  ) );
        iconIds_[objectTypePlateForme]                      = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypePlateforme  ) );
        iconIds_[objectTypePiste]                           = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypePiste  ) );
        iconIds_[objectTypeNuageNBC]                        = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeNuageNBC  ) );
        iconIds_[objectTypeMine]                            = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeMine  ) );
        iconIds_[objectTypeItineraireLog]                   = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeItineraireLog  ) );
        iconIds_[objectTypeImplantationMortier]             = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeImplantationMortier  ) );
        iconIds_[objectTypeFosseAC]                         = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeFosseAC  ) );
        iconIds_[objectTypeEboulement]                      = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeEboulement  ) );
        iconIds_[objectTypeDestructionRoute]                = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeDestructionRoute  ) );
        iconIds_[objectTypeDestructionPont]                 = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeDestructionPont  ) );
        iconIds_[objectTypeCampRefugies]                    = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeCampRefugies  ) );
        iconIds_[objectTypeCampPrisonniers]                 = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeCampPrisonniers  ) );
        iconIds_[objectTypeBarricade]                       = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeBarricade  ) );
        iconIds_[objectTypeAirePoser]                       = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeAirePoser  ) );
        iconIds_[objectTypeAireLogistique]                  = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeAireLogistique  ) );
        iconIds_[objectTypeAbattis]                         = MOS_GLTool::InitializeIcon( QImage( xpm_objectTypeAbattis  ) );
        iconIds_[cadenas]                                   = MOS_GLTool::InitializeIcon( QImage( xpm_cadenas ) );
        iconIds_[talkie_interdit]                           = MOS_GLTool::InitializeIcon( QImage( xpm_talkie_interdit ) );
        iconIds_[brouillage]                                = MOS_GLTool::InitializeIcon( QImage( xpm_brouillage ) );
        iconIds_[radars_on]                                 = MOS_GLTool::InitializeIcon( QImage( xpm_radars_on ) );

        iconLocations_[eGasIcon]                            = MT_Vector2D( -2.7, 2.3 );
        iconLocations_[eAmmoIcon]                           = MT_Vector2D( -2.7, 1.3 );
        iconLocations_[eNbcIcon]                            = MT_Vector2D( -2.7, 0.3 );
        iconLocations_[eConstructionIcon]                   = MT_Vector2D(  2.5, 2.0 );
        iconLocations_[eObservingIcon]                      = MT_Vector2D(  2.5, 2.0 );
        iconLocations_[eFlareIcon]                          = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[eSmokeIcon]                          = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[eSkullIcon]                          = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeZoneProtegee]              = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeZonePoserHelicoptere]      = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeZoneNBC]                   = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeZoneMobiliteAmelioree]     = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeZoneInterditeTir]          = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeZoneInterditeMvt]          = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeZoneImplantationLrm]       = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeZoneImplantationCobra]     = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeZoneImplantationCanon]     = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeZoneBrouillardBrod]        = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeZoneBrouillageBromure]     = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeTerrainLargage]            = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeSiteFranchissement]        = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeSiteDecontamination]       = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeRota]                      = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypePosteTir]                  = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypePosteControle]             = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypePontFlottant]              = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypePlotRavitaillement]        = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypePlateForme]                = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypePiste]                     = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeNuageNBC]                  = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeMine]                      = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeItineraireLog]             = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeImplantationMortier]       = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeFosseAC]                   = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeEboulement]                = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeDestructionRoute]          = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeDestructionPont]           = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeCampRefugies]              = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeCampPrisonniers]           = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeBarricade]                 = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeAirePoser]                 = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeAireLogistique]            = MT_Vector2D(  0.0, 0.0 );
        iconLocations_[objectTypeAbattis]                   = MT_Vector2D(  0.0, 0.0 );
    }

    DrawNameObjects( MOS_App::GetApp().GetWorld() );

    Draw( MOS_App::GetApp().GetObjectManager() );
    Draw( MOS_App::GetApp().GetAgentManager() );
    if( MOS_MainWindow::GetMainWindow().GetOptions().nDrawTacticalLines_ != MOS_Options::eNone )
        Draw( MOS_App::GetApp().GetLineManager() );
    Draw( MOS_App::GetApp().GetWeatherManager() );

    MOS_MainWindow& mainWindow = MOS_MainWindow::GetMainWindow();
    for( T_MapEventFilterVector::const_reverse_iterator it = mainWindow.GetMapEvents().rbegin(); it != mainWindow.GetMapEvents().rend(); ++it )
    {
        (*it)->Draw();
        if( (*it)->IsBlocking() )
            break;
    }

    Draw( *(mainWindow.pObjCreationPanel_) );

    if( MOS_App::GetApp().IsODBEdition() )
        return;

    Draw( *(mainWindow.pMissionPanel_) );
    Draw( *(mainWindow.pWeatherPanel_) );
    Draw( *(mainWindow.pInfoPanel_) );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-03-04
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_World& world )
{
    geometry::Rectangle2f extent( viewRect_.GetLeft(), viewRect_.GetBottom(), viewRect_.GetRight(), viewRect_.GetTop() );
    DrawDetection( world );

    unsigned int nMaxLod = 3;
    if( extent.Width() > 400000.f ) // $$$$ AGE 2005-02-28:
        nMaxLod = 0; // draw nothing
    else if( extent.Width() > 200000.f )
        nMaxLod = 1;
    else if( extent.Width() > 80000.f )
        nMaxLod = 2;

    DrawAreas( world, nMaxLod );
    if( nMaxLod > 2 )
        DrawBorders( world, nMaxLod );
    DrawLines( world, nMaxLod );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawDetection
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawDetection( MOS_World& world )
{
    if( world.pDetection_ )
        world.pDetection_->Draw();
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawAreas
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawAreas( MOS_World& world, unsigned int nMaxLod )
{
    geometry::Rectangle2f extent( viewRect_.GetLeft(), viewRect_.GetBottom(), viewRect_.GetRight(), viewRect_.GetTop() );
    for( unsigned int nLod = 0; nLod < nMaxLod; ++nLod )
    {
        for( MOS_World::CIT_Shapes it = world.lodshapes_[nLod].begin(); it != world.lodshapes_[nLod].end(); ++it )
        {
            assert( *it );
            const GraphicShape_ABC& shape = **it;
            shape.DrawArea( extent );
            glTranslatef( 0.f, 0.f, 0.0001f );
        }
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawBorders
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawBorders( MOS_World& world, unsigned int nMaxLod )
{
    geometry::Rectangle2f extent( viewRect_.GetLeft(), viewRect_.GetBottom(), viewRect_.GetRight(), viewRect_.GetTop() );
    glClear( GL_STENCIL_BUFFER_BIT );
    glEnable( GL_STENCIL_TEST );
    glStencilFunc( GL_NEVER, 1, 0xFFFFFFFF );
    glStencilOp( GL_INCR, GL_INCR, GL_INCR );

    for( unsigned int nLod = 0; nLod < nMaxLod; ++nLod )
    {
        for( MOS_World::CIT_Shapes it = world.lodshapes_[nLod].begin(); it != world.lodshapes_[nLod].end(); ++it )
        {
            assert( *it );
            const GraphicShape_ABC& shape = **it;
            shape.DrawBorders( extent );
        }
    }

    glStencilFunc( GL_GEQUAL, 1, 0xFFFFFFFF );
    for( unsigned int nLod = 0; nLod < nMaxLod; ++nLod )
    {
        for( MOS_World::CIT_Shapes it = world.lodshapes_[nLod].begin(); it != world.lodshapes_[nLod].end(); ++it )
        {
            assert( *it );
            const GraphicShape_ABC& shape = **it;
            shape.DrawBorders( extent );
        }
    }

    glDisable( GL_STENCIL_TEST );
    glTranslatef( 0.f, 0.f, 0.0001f );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawLines
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawLines( MOS_World& world, unsigned int nMaxLod )
{
    geometry::Rectangle2f extent( viewRect_.GetLeft(), viewRect_.GetBottom(), viewRect_.GetRight(), viewRect_.GetTop() );
    for( unsigned int nLod = 0; nLod < nMaxLod; ++nLod )
    {
        for( MOS_World::CIT_Shapes it = world.lodshapes_[nLod].begin(); it != world.lodshapes_[nLod].end(); ++it )
        {
            assert( *it );
            const GraphicShape_ABC& shape = **it;
            shape.DrawLines( extent );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawNameObjects
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawNameObjects( MOS_World& world )
{
    geometry::Rectangle2f extent( viewRect_.GetLeft(), viewRect_.GetBottom(), viewRect_.GetRight(), viewRect_.GetTop() );
    QGLWidget* pWidget = MOS_MainWindow::GetMainWindow().pGLWidget_;
    if( MOS_App::GetApp().Is3D() )
        pWidget = MOS_MainWindow::GetMainWindow().pGL3DWidget_;
    QGLWidget& widget = *pWidget;


    unsigned int nMaxLod = 0;
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    if( options.nDrawLargeText_ == MOS_Options::eOn
   || ( options.nDrawLargeText_ == MOS_Options::eAuto && extent.Width() < 20000 ) )
        nMaxLod = 1;
    if( options.nDrawSmallText_ == MOS_Options::eOn
   || ( options.nDrawSmallText_ == MOS_Options::eAuto && extent.Width() < 10000 ) )
        nMaxLod = 2;
    if( options.nDrawSmallText_ == MOS_Options::eOn
   || ( options.nDrawSmallText_ == MOS_Options::eAuto && extent.Width() < 5000 ) )
        nMaxLod = 3;

    for( unsigned int nLod = 0; nLod < nMaxLod; ++nLod )
    {
        for( MOS_World::CIT_Shapes it = world.lodshapes_[nLod].begin(); it != world.lodshapes_[nLod].end(); ++it )
        {
            assert( *it );
            const GraphicShape_ABC& shape = **it;
            glColor3f( 0, 0, 0 );
            shape.DrawName( extent, widget );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-03-05
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_AgentManager& manager )
{
    int nPlayedTeam = MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;

    // Draw all the team object knowledges, but not in 'all teams' mode (ie. controller view),
    // otherwise most knowledges and objects would overlap)
    if( nPlayedTeam != MOS_Options::eController )
    {
        for( MOS_AgentManager::CIT_TeamMap itTeam = manager.teamMap_.begin(); itTeam != manager.teamMap_.end(); ++itTeam )
        {
            // Avoid drawing knowledges concerning our own team.
            if( nPlayedTeam != (int)(itTeam->second->GetIdx()) )
                continue;
            Draw( *(itTeam->second) );
        }
    }

    // Draw all the gtia agent knowledges, but not in 'all teams' mode (ie. controller view),
    // otherwise most knowledges and agents would overlap)
    if( nPlayedTeam != MOS_Options::eController )
    {
        const MOS_Team& team = *manager.FindTeamFromIdx( nPlayedTeam );
        for( MOS_Team::CIT_GtiaMap itGtia = team.gtias_.begin(); itGtia != team.gtias_.end(); ++itGtia )
            Draw( *(itGtia->second) );
    }

    // Draw all agents. Selected / highlighted agents are redrawn on top by the selection
    // handler when appropriate.
    for( MOS_AgentManager::IT_AgentMap itAgent = manager.agentMap_.begin(); itAgent != manager.agentMap_.end(); ++itAgent )
    {
        // If not in controller view, avoid drawing the enemy units.
        if( nPlayedTeam != MOS_Options::eController && nPlayedTeam != (int)(itAgent->second->GetTeam().GetIdx()) )
            continue;
        // Draw the unit if not aggregated
        if( itAgent->second->IsAutomate() || (! itAgent->second->GetParent()->IsEmbraye())  || (! itAgent->second->GetParent()->IsAggregated()))
            Draw( *(itAgent->second), eNormal );
    }

    // Draw all populations.
    for( MOS_AgentManager::IT_PopulationMap itPop = manager.populationMap_.begin(); itPop != manager.populationMap_.end(); ++itPop )
    {
        // If not in controller view, avoid drawing the enemy units.
        if( nPlayedTeam != MOS_Options::eController && nPlayedTeam != (int)(itPop->second->GetTeam().GetIdx()) )
            continue;
        Draw( *(itPop->second), eNormal );
    }

    // Draw  the conflicts.
    glLineWidth( 4 );
    glColor4d( MOS_COLOR_CONFLICT );
    for( MOS_AgentManager::IT_ConflictMap itC = manager.conflictMap_.begin(); itC != manager.conflictMap_.end(); ++itC )
        if( (*itC).second.pDirectFireTarget_  )
            DrawLine( (*itC).second.pOrigin_->GetPos(), (*itC).second.pDirectFireTarget_->GetPos(), 300.0 ); // $$$$ AGE 2005-03-22:
        else
            DrawArc( (*itC).second.pOrigin_->GetPos(), (*itC).second.vIndirectFireTarget_, 300.0 * rClicksPerPix_ , true );

    //Draw the real time logistic actions
    if ( MOS_MainWindow::GetMainWindow().GetOptions().bDisplayRealTimeLog_ )
    {
        //glLineStipple( 1, MOS_GLTool::stipplePattern_[ (nFrame_  % 16) ] );
        glEnable( GL_LINE_STIPPLE );
        glLineWidth( 2.0 );
        const MOS_AgentManager::T_MaintenanceConsigns& consignsMain = MOS_App::GetApp().GetAgentManager().GetMaintenanceConsigns();
        glColor4d( MOS_COLOR_MAROON );
        for( MOS_AgentManager::CIT_MaintenanceConsigns itMain = consignsMain.begin(); itMain != consignsMain.end(); ++itMain )
        {
            const MOS_LogMaintenanceConsign* con = itMain->second;
            switch( con->GetState() )
            {
            case MOS_LogMaintenanceConsign::eCarrierGoingTo :
                {
                    glLineStipple( 1, MOS_GLTool::stipplePattern_[ (nFrame_  % 16) ] );
                    break;
                }
            case MOS_LogMaintenanceConsign::eCarrierGoingFrom :
                {
                    glLineStipple( 1, MOS_GLTool::stipplePattern_[ ((15 - nFrame_)  % 16) ] );
                    break;
                }
            default:
                {
                    glLineStipple( 1, MOS_GLTool::stipplePattern_[ 0 ] );
                    break;
                }
            }
            DrawArc( con->GetPionLogHandling()->GetPos() ,con->GetPion().GetPos(), 40.0, true );
        }
        const MOS_AgentManager::T_MedicalConsigns& consignsSan = MOS_App::GetApp().GetAgentManager().GetMedicalConsigns();
        glColor4d( MOS_COLOR_PINK );
        for( MOS_AgentManager::CIT_MedicalConsigns itSan = consignsSan.begin(); itSan != consignsSan.end(); ++itSan )
        {
            const MOS_LogMedicalConsign* con = itSan->second;
            switch( con->GetState() )
            {
            case MOS_LogMedicalConsign::eEvacuationGoingTo :
                {
                    glLineStipple( 1, MOS_GLTool::stipplePattern_[ (nFrame_  % 16) ] );
                    break;
                }
            case MOS_LogMedicalConsign::eCollectionGoingTo :
                {
                    glLineStipple( 1, MOS_GLTool::stipplePattern_[ (nFrame_  % 16) ] );
                    break;
                }
            case MOS_LogMedicalConsign::eEvacuationGoingFrom :
                {
                    glLineStipple( 1, MOS_GLTool::stipplePattern_[ ((15 - nFrame_)  % 16) ] );
                    break;
                }
            default:
                {
                    glLineStipple( 1, MOS_GLTool::stipplePattern_[ 0 ] );
                    break;
                }
            }
            DrawArc( con->GetPionLogHandling()->GetPos() ,con->GetPion().GetPos(), 0.0, true );
        }
        const MOS_AgentManager::T_SupplyConsigns& consignsRav = MOS_App::GetApp().GetAgentManager().GetSupplyConsigns();
        glColor4d( MOS_COLOR_ORANGE );
        for( MOS_AgentManager::CIT_SupplyConsigns itRav = consignsRav.begin(); itRav != consignsRav.end(); ++itRav )
        {
            const MOS_LogSupplyConsign* con = itRav->second;
            switch( con->GetState() )
            {
            case MOS_LogSupplyConsign::eConvoyGoingTo :
                {
                    glLineStipple( 1, MOS_GLTool::stipplePattern_[ (nFrame_  % 16) ] );
                    break;
                }
            case MOS_LogSupplyConsign::eConvoyGoingFrom :
                {
                    glLineStipple( 1, MOS_GLTool::stipplePattern_[ ((15 - nFrame_)  % 16) ] );
                    break;
                }
            default:
                {
                    glLineStipple( 1, MOS_GLTool::stipplePattern_[ 0 ] );
                    break;
                }
            }
            DrawArc( con->GetPionLogHandling()->GetPos() ,con->GetPion().GetPos(), 20.0, true );
        }
        glDisable( GL_LINE_STIPPLE );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-03-05
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_Agent& agent, E_State nState )
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();

    // Vision surfaces
    if( options.nDrawVisionSurfaces_ == MOS_Options::eOn
       || ( options.nDrawVisionSurfaces_ == MOS_Options::eAuto && nState == eSelected ) )
        DrawVisionSurfaces( agent );

    // Draw the path if necessary
    if( options.nDrawPaths_ == MOS_Options::eOn
        || (options.nDrawPaths_ == MOS_Options::eAuto && nState != eNormal) )
        DrawPath( agent );

    if( ! viewRect_.IsInside( agent.GetPos() ) )
        return;

    // Draw the unit.
    DrawUnit( agent, nState );

    // Draw the blinking indicator icons
    if( nFrame_%4 == 0 )
        glColor4d( MOS_COLOR_WHITE );
    else
        glColor4d( MOS_COLOR_RED );

    if( ! MOS_App::GetApp().IsODBEdition() )
    {
        if( ! agent.GetContaminatingNBCAgents().empty() )
            DrawIcon( eNbcIcon, agent.vPos_ );

        if( agent.IsOutOfGas() && !agent.IsDead() )
            DrawIcon( eGasIcon, agent.vPos_ );
    }

    
    //// Draw specific NBC parameters
    if( agent.bNbcProtectionSuitWorn_ )
    {
        glColor4d( MOS_COLOR_NBCSUIT );
        DrawCircle( agent.vPos_, MOS_GL_CROSSSIZE * 0.8f , true );
    }

    //Death
    if( agent.IsDead() )
    {
        glColor4d( MOS_COLOR_WHITE );
        DrawIcon( eSkullIcon, agent.GetPos() );
    }

    
    //embrayage
    if( (! agent.IsAutomate()) && agent.pParent_->IsEmbraye() )
    {
        glColor4d( MOS_COLOR_WHITE );
        DrawIcon( cadenas, agent.GetPos() + MT_Vector2D( - 200 , 270  ) );
    }
    
    //radars on
    if( agent.bRadarEnabled_ )
    {
        glColor4d( MOS_COLOR_WHITE );
        DrawIcon( radars_on, agent.GetPos() + MT_Vector2D(  200 , 270  ) );
    }

    //jamming
    if( agent.bCommJammed_ )
    {
        glColor4d( MOS_COLOR_WHITE );
        DrawIcon( brouillage, agent.GetPos() + MT_Vector2D(  200 , 50  ) );
    }

    //radio silence
    if( agent. bRadioSilence_ )
    {
        glColor4d( MOS_COLOR_WHITE );
        DrawIcon( talkie_interdit, agent.GetPos() + MT_Vector2D(  120 , 50  ) );
    }

    // Vision cones
    if( options.nDrawVisionCones_ == MOS_Options::eOn
       || ( options.nDrawVisionCones_ == MOS_Options::eAuto && nState == eSelected ) )
       DrawVisionCones( agent );

    // Vision lines
    if( options.nDrawVisionLines_ == MOS_Options::eOn
   || ( options.nDrawVisionLines_ == MOS_Options::eAuto && nState != eNormal ) )
       DrawVisionLines( agent );
    
   
    //Logistic links
    if ( agent.IsAutomate() && (
            MOS_MainWindow::GetMainWindow().GetOptions().nDisplayMissingLogLinks_ == MOS_Options::eOn
            || ( nState == eSelected && (MOS_MainWindow::GetMainWindow().GetOptions().nDisplayMissingLogLinks_ == MOS_Options::eAuto ))))
    {
        // Display the missing links
        MT_Vector2D translation = MT_Vector2D(0, 150);
        glLineWidth( 3 );
        if ( ! agent.IsLogisticBLT() )
        {
            if ( agent.nTC2_ == 0 && ( ! agent.IsLogisticTC2() ) )
            {
                glColor4d( MOS_COLOR_YELLOW );
                DrawCircle( agent.GetPos() + translation, 300, false );
            }
            if ( agent.nLogMedicalSuperior_ == 0 && ( agent.IsLogisticTC2() || agent.IsLogisticSante() ) )
            {
                glColor4d( MOS_COLOR_PINK );
                DrawCircle( agent.GetPos() + translation, 375, false );
            }
            if ( agent.nLogMaintenanceSuperior_ == 0 && ( agent.IsLogisticTC2() || agent.IsLogisticMaintenance() ) )
            {
                glColor4d( MOS_COLOR_MAROON );
                DrawCircle( agent.GetPos() + translation, 450, false );
            }
            if ( agent.nLogSupplySuperior_ == 0 && ( agent.IsLogisticTC2() || agent.IsLogisticRavitaillement() ) )
            {
                glColor4d( MOS_COLOR_ORANGE );
                DrawCircle( agent.GetPos() + translation, 525, false );
            }
        }
        else
        {
            // decommenter pour voir dessiner un rond vert autour des BLT
            //glColor4d( MOS_COLOR_GREEN );
            //DrawCircle( agent.GetPos() + translation, 300, false );
        }
    }
    // Draw the logistic links of the selected automata
    if ( agent.IsAutomate() && (
        MOS_MainWindow::GetMainWindow().GetOptions().nDisplayLogLinks_ == MOS_Options::eOn
        || ( nState == eSelected && (MOS_MainWindow::GetMainWindow().GetOptions().nDisplayLogLinks_ == MOS_Options::eAuto ))))
    {
        float offset = 100.0;
        glLineWidth( 3 );
        if ( agent.nTC2_ != 0 )
        {
            glColor4d( MOS_COLOR_YELLOW );
            DrawArc( MOS_App::GetApp().GetAgentManager().FindAgent( agent.nTC2_ )->GetPos(), agent.GetPos(), offset, true );
            offset += 100.0;
        }
        if ( agent.nLogMaintenanceSuperior_ != 0 )
        {
            glColor4d( MOS_COLOR_MAROON );
            DrawArc( MOS_App::GetApp().GetAgentManager().FindAgent( agent.nLogMaintenanceSuperior_ )->GetPos(), agent.GetPos(), offset , true );
            offset += 100.0;
        }
        if ( agent.nLogMedicalSuperior_ != 0 )
        {
            glColor4d( MOS_COLOR_PINK );
            DrawArc( MOS_App::GetApp().GetAgentManager().FindAgent( agent.nLogMedicalSuperior_ )->GetPos(), agent.GetPos(), offset, true );
            offset += 100.0;
        }
        if ( agent.nLogSupplySuperior_ != 0 )
        {
            glColor4d( MOS_COLOR_ORANGE );
            DrawArc(  MOS_App::GetApp().GetAgentManager().FindAgent( agent.nLogSupplySuperior_ )->GetPos(), agent.GetPos(), offset, true );
            offset += 100.0;
        }
    }
    //Draw the RC tooltips
    if( MOS_MainWindow::GetMainWindow().GetOptions().bDisplayRConMap_ )
    {
        QGLWidget* pWidget = MOS_MainWindow::GetMainWindow().GetQGLWidget( MOS_App::GetApp().Is3D() );
        const MOS_Agent::T_ReportVector& reports = agent.GetReports();
        bool bTodo = false;
        MT_GLToolTip tooltip = MT_GLToolTip( pWidget );
        tooltip.SetBackgroundColor( 255.0, 255.0, 150.0, 0.5 );
        uint duration = 4 * MOS_App::GetApp().GetTickDuration(); //4 seconds
        for( MOS_Agent_ABC::T_ReportVector::const_reverse_iterator it = reports.rbegin(); it < reports.rend(); ++it )
        {
            int time = MOS_App::GetApp().GetTime() - (*it)->GetTime();
            if( ((*it)->GetType() == MOS_Report_ABC::eRC 
                  || (*it)->GetType() == MOS_Report_ABC::eWarning 
                  || (*it)->GetType() == MOS_Report_ABC::eEvent 
                  || ( (*it)->GetType() == MOS_Report_ABC::eMessage && MOS_MainWindow::GetMainWindow().GetOptions().bDisplayMessagesOnMap_ ))
                && time  < duration )
            {     
                bTodo = true;
                tooltip.AddLine( (*it)->GetStrippedTitle(), 128.0 * time / duration , 128.0 * time / duration , 128.0 * time / duration, 1.0 , true );
            }
            else if ( time  > 4 )
                break;
        }
        if( bTodo )
            tooltip.Draw( viewRect_, agent.GetPos(), rClicksPerPix_ );
    }
    
    
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_Population& population, E_State nState /*= eNormal*/ )
{
    //Draw concentrations
    for( MOS_Population::CIT_ConcentrationMap it = population.concentrationMap_.begin(); it != population.concentrationMap_.end(); ++it )
        Draw( *it->second, nState );

    //Draw flows
    for( MOS_Population::CIT_FlowMap it = population.flowMap_.begin(); it != population.flowMap_.end(); ++it )
        Draw( *it->second, nState );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_PopulationConcentration& concentration, E_State nState /*= eNormal*/ )
{
    GFX_Color color = MOS_GLTool::GetColorForTeam( concentration.GetPopulation().GetTeam() );
    if( nState == eSelected )
        color.AddRGB( 50, 200, 50 );
    if( nState == eHighlighted )
        color.AddRGB( 50, 100, 50 );
    color.SetAlpha( 0.9 );
    color.SetGLColor();
    MT_Float rSurface = concentration.GetLivingHumans() / concentration.GetPopulation().GetType().GetConcentrationDensity();
    DrawCircle( concentration.GetPos(), std::sqrt( rSurface / MT_PI ), true );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_PopulationFlow& flow, E_State nState /*= eNormal*/ )
{
    GFX_Color color = MOS_GLTool::GetColorForTeam( flow.GetPopulation().GetTeam() );
    if( nState == eSelected )
        color.AddRGB( 50, 200, 50 );
    if( nState == eHighlighted )
        color.AddRGB( 50, 100, 50 );
    color.SetGLColor();
    glColor4d( MOS_COLOR_WHITE );
    DrawPath( flow );
    color.SetAlpha( 0.9 );
    color.SetGLColor();
    DrawCircle( flow.GetHeadPosition(), MOS_GL_CROSSSIZE * 0.5, true );
    glLineWidth( 5.0 );
    DrawLine( flow.GetFlow() );
    //glLineWidth( 1.0 );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawPath
// Created: AGE 2005-03-17
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawPath( MOS_Agent& agent )
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    // Update the path data.
    agent.UpdatePathFind();

        // Draw the path if it exists
    T_PointVector vPath( agent.pathFindVector_ );
    if( vPath.size() > 1 )
    {
        // Replace the first point (which is the first point of the segment the agent is on)
        // by the agent's position.
        vPath[0] = agent.vPos_;
        glColor4d( MOS_COLOR_PATH );
        glLineWidth( 3 );
        glLineStipple( 1, MOS_GLTool::stipplePattern_[ (nFrame_ % 16) ] );
        glEnable( GL_LINE_STIPPLE );
        DrawLine( vPath );
        glDisable( GL_LINE_STIPPLE );
    }

    // Draw the old path if necessary
    if( options.bDrawOldPaths_ )
    {
        glColor4d( MOS_COLOR_BLACK );
        glLineWidth( 3 );
        DrawLine( agent.oldPathVector_ );
        glColor4d( MOS_COLOR_OLDPATH );
        glLineWidth( 2 );
        DrawLine( agent.oldPathVector_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawPath
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawPath( MOS_PopulationFlow& flow )
{
//    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions(); // $$$$ SBO 2005-10-20: not used
    // Update the path data.
    flow.UpdatePathFind();

        // Draw the path if it exists
    T_PointVector vPath( flow.GetItineraire() );
    if( vPath.size() > 1 )
    {
        // Replace the first point (which is the first point of the segment the agent is on)
        // by the agent's position.
        vPath[0] = flow.GetPos();
        glColor4d( MOS_COLOR_PATH );
        glLineWidth( 3 );
        glLineStipple( 1, MOS_GLTool::stipplePattern_[ (nFrame_ % 16) ] );
        glEnable( GL_LINE_STIPPLE );
        DrawLine( vPath );
        glDisable( GL_LINE_STIPPLE );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawAutomataStatus
// Created: AGE 2005-03-17
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawAutomataStatus( MOS_Agent& agent, E_State nState )
{
    MT_Vector2D p1 = agent.vPos_ + MT_Vector2D( 1.15, 0.0 );
    MT_Vector2D p2 = agent.vPos_ + MT_Vector2D( 2.15, 1.0 );
    if( nState == eNormal )
        glColor4d( 0.0, 0.8, 0.0, 1.0 );
    else
        glColor4d( 0.0, 1.0, 0.0, 1.0 );
    glLineWidth( 1 );
    DrawRect( p1, p2, true );
    glColor4d( 0.0, 0.0, 0.0, 1.0 );
    DrawRect( p1, p2, false );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawVisionLines
// Created: AGE 2005-03-17
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawVisionLines( MOS_Agent& agent )
{
    SimpleMode sm;

    glLineWidth( 1 );

    glColor4d( MOS_COLOR_RECORDED );
    for( MOS_Agent::CIT_AgentConstPtrSet it = agent.recordedAgents_.begin(); it != agent.recordedAgents_.end(); ++it )
        if( (*it)->GetTeam().GetID() != agent.GetTeam().GetID() )
            DrawLine( agent.vPos_, (*it)->vPos_, 150.0 );

    glColor4d( MOS_COLOR_IDENTIFIED );
    MOS_Agent::CIT_AgentConstPtrSet itAgent;
    for( itAgent = agent.identifiedAgents_.begin(); itAgent != agent.identifiedAgents_.end(); ++itAgent )
        if( (*itAgent)->GetTeam().GetID() != agent.GetTeam().GetID() )
            DrawLine( agent.vPos_, (*itAgent)->vPos_, 150.0 );

    glColor4d( MOS_COLOR_RECO );
    for( itAgent = agent.recognizedAgents_.begin(); itAgent != agent.recognizedAgents_.end(); ++itAgent )
        if( (*itAgent)->GetTeam().GetID() != agent.GetTeam().GetID() )
            DrawLine( agent.vPos_, (*itAgent)->vPos_, 150.0 );

    glColor4d( MOS_COLOR_DETECTED );
    for( itAgent = agent.detectedAgents_.begin(); itAgent != agent.detectedAgents_.end(); ++itAgent )
        if( (*itAgent)->GetTeam().GetID() != agent.GetTeam().GetID() )
            DrawLine( agent.vPos_, (*itAgent)->vPos_, 150.0 );

    glColor4d( MOS_COLOR_DETECTED );
    MOS_Agent::CIT_ObjectConstPtrSet itObject;
    for( itObject = agent.objectsPerceptionInProgress_.begin(); itObject != agent.objectsPerceptionInProgress_.end(); ++itObject )
        DrawLine( agent.vPos_, (*itObject)->center_, 150.0 );

    glColor4d( MOS_COLOR_IDENTIFIED );
    for( itObject = agent.objectsPerceived_.begin(); itObject != agent.objectsPerceived_.end(); ++itObject )
        DrawLine( agent.vPos_, (*itObject)->center_, 150.0 );

//    glColor4d( MOS_COLOR_IDENTIFIED );
    glColor3d( 0., 1., 0. );
    for( MOS_Agent::CIT_ConcentrationSet itConcentration = agent.concentrationsPerceived_.begin(); itConcentration != agent.concentrationsPerceived_.end(); ++itConcentration )
        DrawLine( agent.vPos_, (*itConcentration)->GetPos(), 150.0 );

    glColor3d( 1., 1., 0. );
    for( MOS_Agent::CIT_FlowVisionMap it = agent.flowsPerceived_.begin(); it != agent.flowsPerceived_.end(); ++it )
    {
        const T_PointVector& shape = it->second;
        assert( !shape.empty() );

        DrawLine( agent.vPos_, shape.back(), 150.0 );
        DrawLine( agent.vPos_, shape.front(), 150.0 );
        glLineWidth( 3.0 );
        DrawLine( shape );
        glLineWidth( 1.0 );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-05-25
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_Team& team )
{
    for( MOS_Team::IT_ObjectKnowledgeMap it = team.objectKnowledges_.begin(); it != team.objectKnowledges_.end(); ++it )
        Draw( *(it->second) );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-05-25
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_Gtia& gtia )
{
    for( MOS_Gtia::IT_AgentKnowledgeMap it = gtia.agentKnowledges_.begin(); it != gtia.agentKnowledges_.end(); ++it )
        Draw( *(it->second) );

    for( MOS_Gtia::IT_PopulationKnowledgeMap it = gtia.populationKnowledges_.begin(); it != gtia.populationKnowledges_.end(); ++it ) 
        Draw( *it->second );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-05-25
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_AgentKnowledge& knowledge, E_State nState )
{
    // Avoid drawing agent knowledges about our own team.
    if( knowledge.GetOwner().GetTeam().GetID() == knowledge.GetRealAgent().GetTeam().GetID() )
        return;

    if( knowledge.nCurrentPerceptionLevel_ == eDetection || knowledge.nCurrentPerceptionLevel_ == eNotSeen  )
    {
        GFX_Color color( 200, 200, 200 );
        if( nState == eSelected )
            color = GFX_Color( 255, 255, 255 );
        if( knowledge.nTeam_ != (uint)-1 )
        {
            color = MOS_GLTool::GetColorForTeam( knowledge.realAgent_.GetTeam() );
            if( nState == eSelected )
                color.AddRGB( 50, 200, 50 );
            if( nState == eHighlighted )
                color.AddRGB( 50, 100, 50 );
        }
        MT_Float rSize = 600.0;
        std::string level;
        if ( knowledge.nLevel_ != eNatureLevel_None )
            level = knowledge.realAgent_.levelSymbolName_;
        else
            level = "a";
        std::string symbol;
        if ( knowledge.nWeapon_ != eUnitNatureWeapon_None )
            symbol = knowledge.realAgent_.symbolName_;
        else
            symbol = "a";
        GFX_Tools::CreateGLAgentShadow( knowledge.vPosition_, rSize, 4., 8., color , true, symbol, knowledge.nEtatOps_ );
        GFX_Tools::CreateGLAgentShadow( knowledge.vPosition_, rSize, 4., 8., color , true, level, -1 );
    
    }
    else
        DrawUnit( knowledge.realAgent_, nState );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_PopulationKnowledge& knowledge, E_State nState /*= eNormal*/ )
{
    const MOS_PopulationKnowledge::T_ConcentrationKnowledgeMap& concentrations = knowledge.GetConcentrations();
    for( MOS_PopulationKnowledge::CIT_ConcentrationKnowledgeMap it = concentrations.begin(); it != concentrations.end(); ++it )
        Draw( *it->second, nState );

    const MOS_PopulationKnowledge::T_FlowKnowledgeMap& flows = knowledge.GetFlows();
    for( MOS_PopulationKnowledge::CIT_FlowKnowledgeMap it = flows.begin(); it != flows.end(); ++it )
        Draw( *it->second, nState );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_PopulationConcentrationKnowledge& knowledge, E_State nState /*= eNormal*/ )
{
    assert( knowledge.GetPopulationKnowledge() );
    GFX_Color color = MOS_GLTool::GetColorForTeam( *knowledge.GetPopulationKnowledge()->GetTeam() );
    if( nState == eSelected )
        color.AddRGB( 50, 200, 50 );
    if( nState == eHighlighted )
        color.AddRGB( 50, 100, 50 );
    color.SetAlpha( 0.8 );
    if( knowledge.IsValidNbrAliveHumans() )
    {
        color.SetGLColor();
        MT_Float rSurface = knowledge.GetArea();
        DrawCircle( knowledge.GetPosition(), std::sqrt( rSurface / MT_PI ), true );
    }
    else
    {
        color.SetAlpha( 0.4 );
        color.SetGLColor();
        DrawCircle( knowledge.GetPosition(), 100, true );
    }
}
    
// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_PopulationFlowKnowledge& knowledge, E_State nState /*= eNormal*/ )
{
    assert( knowledge.GetPopulationKnowledge() );
    GFX_Color color = MOS_GLTool::GetColorForTeam( *knowledge.GetPopulationKnowledge()->GetTeam() );
    if( nState == eSelected )
        color.AddRGB( 50, 200, 50 );
    if( nState == eHighlighted )
        color.AddRGB( 50, 100, 50 );
    color.SetAlpha( 0.8 );
    color.SetGLColor();
    glLineWidth( 5.0 );

    const MOS_PopulationFlowKnowledge::T_FlowParts& parts = knowledge.GetFlowParts();
    for( MOS_PopulationFlowKnowledge::CIT_FlowParts it = parts.begin(); it != parts.end(); ++it )
        DrawLine( ( *it )->GetPart() );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_ObjectManager& manager )
{
    // Only draw the real objects when in all teams mode (ie. controller view).
    if( MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_ != MOS_Options::eController )
        return;

    for( MOS_ObjectManager::IT_ObjectMap it = manager.ObjectMap_.begin(); it != manager.ObjectMap_.end(); ++it )
        Draw( *(it->second) );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_Object_ABC& object, E_State nState )
{
    glLineWidth( 2 );


    if( nState == eSelected )
    {
        glColor4d( MOS_COLOR_SEL_Object );
    }
    else
    {
        GFX_Color color = MOS_GLTool::GetColorForTeam( object.GetTeam() );
        color.AddRGB( 100, 200, 100 );
        color.SetGLColor();
    }

    /*
    if( nState == eSelected )
        glColor4f( MOS_COLOR_SEL_Object );
    else
        glColor4f( MOS_COLOR_Object );
    */
    const T_PointVector& p = object.pointVector_;
    switch( object.nTypeLocalisation_ )
    {
        case EnumTypeLocalisation::point:
            DrawCross( p.front(), 300, 1.0 );
            break;
        case EnumTypeLocalisation::line:
            DrawLine( p );
            break;
        case EnumTypeLocalisation::polygon:
            DrawPolygon( p );
            break;
        case EnumTypeLocalisation::circle:
            DrawCircle( p.front(), p.front().Distance( p.back() ) );
            break;
        default:
            DrawLine( p );
    }
    if ( MOS_MainWindow::GetMainWindow().GetOptions().bDrawObjetIcons_ )
    {
        E_Icon objectIcon = IconOfObjectType( (E_ObjectType) object.GetType() );
        if ( objectIcon != eNoneIcon && p.size() > 0 )
            DrawIcon( objectIcon, *(--p.end()), 900 );
    }
    
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-05-26
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_ObjectKnowledge& knowledge, E_State nState )
{
    if( knowledge.bIsPerceived_ && knowledge.pRealObject_ != 0 )
    {
        Draw( *knowledge.pRealObject_, nState );
    }
    else
    {
        glLineWidth( 1 );
        if( nState == eSelected )
            glColor4d( MOS_COLOR_SEL_KNOWLEDGE );
        else
            glColor4d( MOS_COLOR_KNOWLEDGE );

        if( knowledge.points_.size() == 1 )
            DrawCross( knowledge.points_.front(), 300, 1.0 );
        else
            DrawLine( knowledge.points_ );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-04-15
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_LineManager& lineManager )
{
    MOS_LineManager::CT_TacticalLineSet& lineList = lineManager.GetLineList();
    for( MOS_LineManager::CIT_TacticalLineSet it = lineList.begin(); it != lineList.end(); ++it )
        Draw( **it, eNormal );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-04-15
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_TacticalLine_ABC& line, E_State nState, int nSelectedPoint )
{
    CT_PointVector& pointList = line.GetPointList();
    if( pointList.empty() )
        return;

    glColor4d( 1.0, 1.0, 1.0, 1.0 );
    glLineWidth( 2.0 );

    if( nState == eSelected )
        glColor4d( 0.7, 0.7, 1.0, 1.0 );
    else if( nState == eHighlighted )
        glColor4d( 1.0, 0.4, 0.4, 1.0 );

    DrawLine( pointList );

    if( nState == eSelected )
    {
        glPointSize( 7 );
        DrawPoints( pointList );

        if( nSelectedPoint != -1 )
        {
            glColor4d( 0.5, 0.5, 1.0, 1.0 );
            assert( nSelectedPoint < (int)pointList.size() );
            DrawPoint( pointList[nSelectedPoint] );
        }
    }

    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    float rSize = 300 * (options.nFontSize_ / 10.0);

    // $$$$ AGE 2005-05-19: 3D !
    if( ! MOS_App::GetApp().Is3D() )
    {
        glColor4d( 0.0, 0.0, 0.0, 1.0 );
        MT_GLFont::Print( "Arial", pointList.front(), line.GetName().c_str(), rSize, 4 );
        MT_GLFont::Print( "Arial", pointList.back(),  line.GetName().c_str(), rSize, 4 );
        glColor4d( 1.0, 1.0, 1.0, 1.0 );
        MT_GLFont::Print( "Arial", pointList.front(), line.GetName().c_str(), rSize );
        MT_GLFont::Print( "Arial", pointList.back(),  line.GetName().c_str(), rSize );
    }
    else
    {
        static const MOS_RawVisionData& data = MOS_App::GetApp().GetRawVisionData();
        glPushMatrix();

        MT_Float rAltitude = ( data.GetHeight( pointList.front().rX_, pointList.front().rX_ ) + 10 ) * rZRatio_ + rDeltaZ_;
        glTranslatef( 0, 0, rAltitude );
        glColor4d( 0.0, 0.0, 0.0, 1.0 );
        MT_GLFont::Print( "Arial", pointList.front(), line.GetName().c_str(), rSize, 4 );
        glColor4d( 1.0, 1.0, 1.0, 1.0 );
        MT_GLFont::Print( "Arial", pointList.front(), line.GetName().c_str(), rSize );
        glTranslatef( 0, 0, -rAltitude );

        rAltitude = ( data.GetHeight( pointList.back().rX_, pointList.back().rX_ ) + 10 ) * rZRatio_ + rDeltaZ_;
        glTranslatef( 0, 0, rAltitude );
        glColor4d( 0.0, 0.0, 0.0, 1.0 );
        MT_GLFont::Print( "Arial", pointList.back(),  line.GetName().c_str(), rSize, 4 );
        glColor4d( 1.0, 1.0, 1.0, 1.0 );
        MT_GLFont::Print( "Arial", pointList.back(),  line.GetName().c_str(), rSize );
        glPopMatrix();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-06-03
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_Meteo_Manager& weatherManager )
{
    MOS_Meteo_Manager::T_AmmoMeteoEffetMap& ammoMeteoEffects = weatherManager.ammoMeteoEffects_;
    const MT_Float rCellSize = MOS_App::GetApp().GetRawVisionData().GetCellSize();
    for( MOS_Meteo_Manager::CIT_AmmoMeteoEffetMap it2 = ammoMeteoEffects.begin(); it2 != ammoMeteoEffects.end(); ++it2 )
    {
        if( it2->second.first == EnumFireEffectType::fumigene )
            glColor4d( MOS_COLOR_SMOKE );
        else
            glColor4d( MOS_COLOR_LIGHT );


        const MT_Ellipse& ellipse = *it2->second.second;


        const MT_Rect bb = ellipse.GetBoundingBox();
        MT_Float x          = floor( bb.GetLeft()   / rCellSize ) * rCellSize;
        const MT_Float xMax = ceil ( bb.GetRight()  / rCellSize ) * rCellSize;
        const MT_Float yMin = floor( bb.GetBottom() / rCellSize ) * rCellSize;
        const MT_Float yMax = ceil ( bb.GetTop()    / rCellSize ) * rCellSize;

        for( ; x < xMax; x+=rCellSize )
        {
            for( MT_Float y = yMin; y < yMax; y+=rCellSize )
            {
                if( ellipse.IsInside( MT_Vector2D( x, y ) ) )
                    DrawRect( MT_Vector2D( x, y + rCellSize ), MT_Vector2D( x+rCellSize, y ), true );
            }
        }

        glColor4d( MOS_COLOR_WHITE  );
        if( it2->second.first == EnumFireEffectType::fumigene )
            DrawIcon( eSmokeIcon, ellipse.GetCenter(), 300.f );
        else
            DrawIcon( eFlareIcon, ellipse.GetCenter(), 300.f );
    }

    // Only display the weather if enabled in the options.
    if( ! MOS_MainWindow::GetMainWindow().GetOptions().bDisplayWeather_ )
        return;

    glLineWidth( 2 );
    MOS_Meteo_Manager::T_Meteo_Vector& localWeathers = weatherManager.GetLocalWeatherList();
    for( MOS_Meteo_Manager::IT_Meteo_Vector it = localWeathers.begin(); it != localWeathers.end(); ++it )
    {
        glColor4d( MOS_COLOR_WEATHER_BLEND );
        DrawRect( (*it)->GetUpperLeftCorner(), (*it)->GetLowerRightCorner(), true );
        glColor4d( MOS_COLOR_WEATHER );
        DrawRect( (*it)->GetUpperLeftCorner(), (*it)->GetLowerRightCorner(), false );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_MissionPanel& panel )
{
    if( panel.pMissionInterface_ != 0 )
        Draw( *(panel.pMissionInterface_) );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_MissionInterface_ABC& missionInterface )
{
    for( IT_ParamVector it = missionInterface.paramVector_.begin(); it != missionInterface.paramVector_.end(); ++it )
        (*it)->Draw();
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-06-04
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_WeatherPanel& panel )
{
    if( ! panel.isVisible() )
        return;

    // If the weather is not displayed, display it while the panel is visible.
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    if( ! options.bDisplayWeather_ )
    {
        options.bDisplayWeather_ = true;
        Draw( MOS_App::GetApp().GetWeatherManager() );
        options.bDisplayWeather_ = false;
    }

    QListViewItem* pItem = panel.pWeatherListView_->currentItem();
    if(    pItem == 0 
        || pItem->rtti() != MOS_WeatherPanel::eLocal )
        return;

    MOS_Meteo_Local* pWeather = ((MT_ValuedListViewItem< MOS_Meteo_Local*>*)pItem)->GetValue();

    glLineWidth( 3 );
    glColor4d( MOS_COLOR_SEL_WEATHER_BLEND );
    DrawRect( pWeather->GetUpperLeftCorner(), pWeather->GetLowerRightCorner(), true );
    glColor4d( MOS_COLOR_SEL_WEATHER );
    DrawRect( pWeather->GetUpperLeftCorner(), pWeather->GetLowerRightCorner(), false );

    // Is a new position for the weather set?
    if( panel.point1_ + panel.point2_ != pWeather->GetUpperLeftCorner() + pWeather->GetLowerRightCorner() )
    {
        glLineWidth( 3 );
        glColor4d( MOS_COLOR_NEW_WEATHER_BLEND );
        DrawRect( panel.point1_, panel.point2_, true );
        glColor4d( MOS_COLOR_NEW_WEATHER );
        DrawRect( panel.point1_, panel.point2_, false );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-06-07
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_ObjectCreationPanel& panel )
{
    if( ! panel.isVisible() )
        return;

    panel.pLocation_->Draw();
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-06-22
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_InfoPanel& panel )
{
    if(    ! panel.isVisible()  
        || panel.pTabWidget_->currentPage() != panel.pStatePanel_
        || panel.pReportPanel_->pReportListView_->selectedItem() == 0 )
        return;

    QListViewItem* pItem = panel.pReportPanel_->pReportListView_->selectedItem();
    glColor4d( MOS_COLOR_RC );
    MOS_Report_ABC& report = panel.pReportPanel_->pReportListView_->GetItemValue( *pItem );
    DrawCross( report.GetPos(), 300, 1 );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_ParamPath& param )
{
    glLineWidth( 3.0 );
    T_PointVector& pointList1 = param.pLineEditor_->pointList_;
    T_PointVector& pointList2 = param.pointList_;

    if( ! pointList1.empty() )
    {
        // Draw the line from the agent to the first point of the path that is under
        // construction. If that's the only line, draw the arrowhead.
        glColor4d( MOS_COLOR_UNDERCONST );
        DrawLine( param.agent_.GetPos(), pointList1.front(), pointList1.size() == 1 ? 300.0 : 0.0 );
    }
    else if( ! pointList2.empty() )
    {
        T_PointVector& pointList2 = param.pointList_;

        // Draw the line from the agent to the first point of the path.
        // If that's the only line, draw the arrowhead.
        glColor4d( MOS_COLOR_PARAM );
        DrawLine( param.agent_.GetPos(), pointList2.front(), pointList2.size() == 1 ? 300.0 : 0.0 );

        DrawLine( pointList2, 2.0 );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_ParamPathList& param )
{
    QListViewItem* pItem = param.firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< T_PointVector >* pCastItem = (MT_ValuedListViewItem< T_PointVector >*)pItem;
        if( pItem == param.selectedItem() )
            glColor4d( MOS_COLOR_SEL_PARAM );
        else
            glColor4d( MOS_COLOR_PARAM );
        DrawLine( pCastItem->GetValue(), 2.0 );
        pItem = pItem->nextSibling();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-04-23
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_ParamLimaList& param )
{
    QListViewItem* pItem = param.firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< MOS_TacticalLine_ABC* >* pCastItem = (MT_ValuedListViewItem< MOS_TacticalLine_ABC* >*)pItem;
        if( pItem == param.selectedItem() )
            glColor4d( MOS_COLOR_SEL_PARAM );
        else
            glColor4d( MOS_COLOR_PARAM );
        DrawLine( pCastItem->GetValue()->GetPointList() );
        pItem = pItem->nextSibling();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-04-23
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_ParamLimits& param )
{
    // Updates the data in the parameter, useful if a limit was deleted in the last update.
    param.UpdateLabels();

    if( param.pLimit1_ != 0 )
        Draw( *param.pLimit1_, eHighlighted );

    if( param.pLimit2_ != 0 )
        Draw( *param.pLimit2_, eHighlighted ); 
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-04-23
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_ParamPoint& param )
{
    glColor4d( MOS_COLOR_PARAM );
    DrawCross( param.point_, 100, 4 );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-04-23
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_ParamAgent& param )
{
    glColor4d( MOS_COLOR_PARAM );
    DrawCross( param.pAgent_->vPos_, 100, 3 );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-04-23
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_ParamAgentList& param )
{
    QListViewItem* pItem = param.firstChild();
    while( pItem != 0 )
    {
        if( pItem == param.selectedItem() )
            glColor4d( MOS_COLOR_SEL_PARAM );
        else
            glColor4d( MOS_COLOR_PARAM );
        MT_ValuedListViewItem< MOS_Agent* >* pCastItem = (MT_ValuedListViewItem< MOS_Agent* >*)pItem;
        DrawCross( pCastItem->GetValue()->vPos_, 100, 3 );
        pItem = pItem->nextSibling();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-05-07
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_ParamLocation& param, bool bSelected )
{
    if( param.pointList_.empty() )
        return;

    if( bSelected ) 
        glColor4d( MOS_COLOR_SEL_PARAM );
    else
        glColor4d( MOS_COLOR_PARAM );
    DrawLocation( (MOS_ShapeEditorMapEventFilter::E_LineType)param.nType_, param.pointList_ );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-05-07
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_ParamLocationList& param )
{
    QListViewItem* pItem = param.firstChild();

    while( pItem != 0 )
    {
        MOS_ParamLocationList::T_LocationItem* pCastItem = (MOS_ParamLocationList::T_LocationItem*)pItem;
        if( pItem == param.selectedItem() )
            glColor4d( MOS_COLOR_SEL_PARAM );
        else
            glColor4d( MOS_COLOR_PARAM );
        DrawLocation( (MOS_ShapeEditorMapEventFilter::E_LineType)pCastItem->GetValue().first, pCastItem->GetValue().second );
        pItem = pItem->nextSibling();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-05-25
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_ParamObstacle& param, bool bSelected )
{
    Draw( *(param.pLocation_), bSelected );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-05-25
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( MOS_ParamObstacleList& param )
{
    QListViewItem* pItem = param.pListView_->firstChild();

    glColor4d( MOS_COLOR_PARAM );
    while( pItem != 0 )
    {
        if( pItem == param.pListView_->selectedItem() )
        {
            pItem = pItem->nextSibling();
            continue;
        }

        MOS_ParamObstacleList::T_Item& item = ((MOS_ParamObstacleList::T_ListItem*)pItem)->GetValue();
        DrawLocation( (MOS_ShapeEditorMapEventFilter::E_LineType)item.first->pos_obstacle.type, item.second );
        pItem = pItem->nextSibling();
    }

    if( param.pObstacleEditor_ != 0 )
        Draw( *(param.pObstacleEditor_), true );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-04-16
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( const MOS_DefaultMapEventHandler& eventHandler )
{
    // Agent
    if( eventHandler.selectedElement_.pAgent_ != 0 )
    {
        MOS_Agent* pSelectedAgent = eventHandler.selectedElement_.pAgent_;

        // Draw the parents, siblilings and children of selected agents with highlighted colors
        MOS_AgentManager& manager = MOS_App::GetApp().GetAgentManager();
        for( MOS_AgentManager::IT_AgentMap itAgent = manager.agentMap_.begin(); itAgent != manager.agentMap_.end(); ++itAgent )
        {
            MOS_Agent* pAgent = itAgent->second;
            MOS_Agent* pParent = pAgent->GetParent();

            if( pAgent == pSelectedAgent )
                continue;

            // Highlight the selected agent's father, its siblings and its children.
            bool bDraw =   ( pParent == pSelectedAgent )                                // Child
                        || ( pAgent  == pSelectedAgent->GetParent() )                   // Parent
                        || ( pParent != 0 && pParent == pSelectedAgent->GetParent() );  // Sibling
            if( bDraw )
                Draw( *(itAgent->second), eHighlighted );
        }

        // Draw the selected agent on top of all that.
        Draw( *pSelectedAgent, eSelected );
    }
    if( eventHandler.selectedElement_.pAgentKnowledge_ != 0 )
        Draw( *eventHandler.selectedElement_.pAgentKnowledge_, eSelected );

    // Population
    if( eventHandler.selectedElement_.pPopulation_ != 0 )
        Draw( *eventHandler.selectedElement_.pPopulation_, eSelected );
    if( eventHandler.selectedElement_.pPopulationConcentration_ != 0 )
        Draw( *eventHandler.selectedElement_.pPopulationConcentration_, eSelected );
    if( eventHandler.selectedElement_.pPopulationFlow_ != 0 )
        Draw( *eventHandler.selectedElement_.pPopulationFlow_, eSelected );

    if( eventHandler.selectedElement_.pPopulationKnowledge_ != 0 )
        Draw( *eventHandler.selectedElement_.pPopulationKnowledge_, eSelected );
    if( eventHandler.selectedElement_.pPopulationConcentrationKnowledge_ != 0 )
        Draw( *eventHandler.selectedElement_.pPopulationConcentrationKnowledge_, eSelected );
    if( eventHandler.selectedElement_.pPopulationFlowKnowledge_ != 0 )
        Draw( *eventHandler.selectedElement_.pPopulationFlowKnowledge_, eSelected );

    // Objects
    if( eventHandler.selectedElement_.pObject_ != 0 )
        Draw( *eventHandler.selectedElement_.pObject_, eSelected );
    if( eventHandler.selectedElement_.pObjectKnowledge_ != 0 )
        Draw( *eventHandler.selectedElement_.pObjectKnowledge_, eSelected );
    if( eventHandler.selectedElement_.pLine_ != 0 )
        Draw( * eventHandler.selectedElement_.pLine_, eSelected, eventHandler.selectedElement_.nLinePoint_);
    
    //Draw an info box under the mouse if a pawn is Hovered  
    if( ! MOS_App::GetApp().IsODBEdition()
        && MOS_MainWindow::GetMainWindow().GetOptions().bDisplayHoveredInfo_
        && eventHandler.hoveredElement_.pAgent_ 
        && eventHandler.hoveredElement_.pAgent_ != eventHandler.selectedElement_.pAgent_  )
    {
        QGLWidget* pWidget = MOS_MainWindow::GetMainWindow().GetQGLWidget( MOS_App::GetApp().Is3D() );
        MT_Float zoom = rClicksPerPix_; 
        MOS_Agent* pAgent = eventHandler.hoveredElement_.pAgent_;
        const MT_Vector2D posAgent = eventHandler.hoveredElement_.pAgent_->GetPos();
        MT_Vector2D pos = MT_Vector2D( posAgent.rX_, posAgent.rY_ );

        GFX_Color color( 0.0, 0.0, 0.0, 1.0 ); //color of the text
        
        //create a ToolTip
        MT_GLToolTip toolTip = MT_GLToolTip( pWidget );
        
        //write the name of the pawn
        QString strName =  pAgent->GetName().c_str();
        if ( pAgent->bNeutralized_ )
            strName = QString("Neutralisé: ") + strName;
        else if ( pAgent->nOpState_ == eEtatOperationnel_DetruitTotalement )
            strName = QString( ENT_Tr::ConvertFromEtatOperationnel( pAgent->nOpState_ ).c_str() ) + QString( " " ) + strName;
        else if ( pAgent->nOpState_ == eEtatOperationnel_DetruitTactiquement )
            strName = QString( ENT_Tr::ConvertFromEtatOperationnel( pAgent->nOpState_ ).c_str() ) + QString( " " ) + strName;
        else if ( pAgent->bSurrendered_ )
            strName = QString("Rendu: ") + strName;
        else if ( pAgent->bPrisoner_ )
            strName = QString("Prisonnier: ") + strName;
        
        if ( pAgent->bPrisoner_ || pAgent->bSurrendered_ )
            color.SetRGB( 128.0, 0.0, 128.0 );
        else if ( pAgent->bNeutralized_ 
                  || pAgent->nOpState_ == eEtatOperationnel_DetruitTactiquement )
            color.SetRGB( 255.0, 128.0 , 0.0 );
        else if ( pAgent->nOpState_ == eEtatOperationnel_DetruitTotalement  )
            color.SetRGB( 255.0, 0.0, 0.0 );
        else
            color.SetRGB( 255.0, 255.0, 0.0 );

        toolTip.AddLine( strName, color ,true, 1.3f );

        //write the current mission
        if( pAgent->GetCurrentMission() != 0 )
        {
            //ok, not proud of it...
            QString strMission = QString( "MISSION: " ) + QString( ENT_Tr::ConvertFromUnitMission( (E_UnitMission)( pAgent->GetCurrentMission() - 1 ) ).c_str() );
            color.SetRGB( 255.0, 255.0, 255.0 );
            toolTip.AddLine( strMission, color );
        }
        
        //write the decisional state and the RapFor of the pawn
        QString strRapFor = QString( "Rapport de force " ) + ENT_Tr::ConvertFromEtatRapFor( pAgent->nFightRateState_ ).c_str();
        if ( pAgent->nCloseCombatState_  != eEtatCombatRencontre_None )
            strRapFor = QString(ENT_Tr::ConvertFromEtatCombatRencontre( pAgent->nCloseCombatState_ ).c_str()).upper() 
                        + QString( ", " ) 
                        + strRapFor;  

        if ( pAgent->nFightRateState_ == eEtatRapFor_Defavorable )
            color.SetRGB( 255.0, 154.0, 154.0 );
        else
            color.SetRGB( 204.0, 255, 204.0 );

        toolTip.AddLine( strRapFor, color );
        
        //write the ROE and the stance of the pawn
        QString strROE = QString( "%1 %2 %" ).arg( ENT_Tr::ConvertFromUnitPosture( pAgent->GetStance() ).c_str(), QString::number( pAgent->nPostureCompletionPourcentage_ ) )
                        + QString( ", ROE: " ) 
                        + QString( ENT_Tr::ConvertFromRoe( pAgent->nRulesOfEngagementState_ ).c_str());
        
        color.SetRGB( 255.0, 255.0, 255.0 );
        
        toolTip.AddLine( strROE, color );
        
        //write the 5 last RCs
        toolTip.AddLine( QString(""), color );
        const MOS_Agent_ABC::T_ReportVector& reports = pAgent->GetReports();
        uint i = 0;
        for( MOS_Agent_ABC::T_ReportVector::const_reverse_iterator it = reports.rbegin(); it < reports.rend(); ++it )
        {
            ++i;
            //int time = MOS_App::GetApp().GetTime() - (*it)->GetTime();
            if ( (*it)->IsRCType() )
            {
                const QString strTime = MT_FormatString( "%02d:%02d:%02d", ( (*it)->GetTime() / 3600 ) % 24, ( (*it)->GetTime() / 60 ) % 60 , (*it)->GetTime() % 60  ).c_str();
                const QString strTitle = (*it)->GetStrippedTitle().c_str();
                toolTip.AddLine( QString("%1 %2").arg( strTime, strTitle ), 150.0 , 150.0 , 150.0, 1.0 , false );
            }
            if ( i > 5 )
                break;
        }
        
        //draw the tooltip
        toolTip.Draw( viewRect_, pos, zoom  );
    }

}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw
// Created: APE 2004-04-15
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw( const MOS_ShapeEditorMapEventFilter& eventHandler )
{
    glColor4d( MOS_COLOR_UNDERCONST );
    DrawLocation( eventHandler.nLineType_, eventHandler.pointList_ );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawLocation
// Created: APE 2004-05-07
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawLocation( MOS_ShapeEditorMapEventFilter::E_LineType nType, const T_PointVector& pointList )
{
    glLineWidth( 3.0 );
    glPointSize( 3 );

    switch( nType )
    {
    case MOS_ShapeEditorMapEventFilter::eCircle:
        if( (int)pointList.size() <= 1 )
            DrawPoints( pointList );
        else
        {
            DrawCircle( pointList[0], pointList[0].Distance( pointList[1] ) );
        }
        break;
    case MOS_ShapeEditorMapEventFilter::eLine:
        if( (int)pointList.size() == 1 )
            DrawPoints( pointList );
        else
            DrawLine( pointList );
        break;
    case MOS_ShapeEditorMapEventFilter::ePolygon:
        if( (int)pointList.size() == 1 )
            DrawPoints( pointList );
        else
            DrawPolygon( pointList );
        break;
    case MOS_ShapeEditorMapEventFilter::ePoint:
        if( ! pointList.empty() )
            DrawCross( pointList.front(), 100, 1 );
        break;
    case MOS_ShapeEditorMapEventFilter::eTacticalLine:
        glPointSize( 8.0 );
        DrawLine( pointList );
        DrawPoints( pointList );
        break;
    case MOS_ShapeEditorMapEventFilter::ePath:
        DrawLine( pointList, 100.0 );
        break;
    case MOS_ShapeEditorMapEventFilter::eRectangle:
        if( (int)pointList.size() == 1 )
            DrawCross( pointList.front(), 100, 1 );
        else if( (int)pointList.size() == 2 )
        {
            double oldColor[4];
            glGetDoublev( GL_CURRENT_COLOR, oldColor );
            glColor4d( oldColor[0], oldColor[1], oldColor[2], 0.25 );
            DrawRect( pointList[0], pointList[1], true );
            glColor4dv( oldColor );
            DrawRect( pointList[0], pointList[1], false );

        }
        break;
    default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Vertex
// Created: AGE 2005-05-09
// -----------------------------------------------------------------------------
void MOS_GLTool::Vertex( MT_Float rX, MT_Float rY, MT_Float rZSign )
{
    static const MOS_RawVisionData& data = MOS_App::GetApp().GetRawVisionData();
    const MT_Float rAltitude = data.GetHeight( rX, rY ) * rZRatio_;
    glVertex3f( rX, rY, rAltitude + rZSign * rDeltaZ_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Refine
// Created: AGE 2005-05-09
// -----------------------------------------------------------------------------
T_PointVector MOS_GLTool::Refine( const T_PointVector& points )
{
    if( ! MOS_App::GetApp().Is3D() )
        return points;

    const MT_Vector2D* pPrevious = 0;
    T_PointVector result;
    result.reserve( 2*points.size() );
    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
    {
        const MT_Vector2D& current = *it;
        if( pPrevious )
        {
            MT_Vector2D u = current - *pPrevious;
            const MT_Float rDistance = u.Magnitude();
            if( rDistance > 0 )
                u /= rDistance;
            unsigned int nSteps = unsigned int( 1 + rDistance / 1000. );
            const MT_Float rIncrease = rDistance / nSteps;
            MT_Float rAbscissa = 0;
            for( unsigned int i = 0; i < nSteps; ++i )
            {
                rAbscissa += rIncrease;
                const MT_Vector2D inBetween = *pPrevious + u * rAbscissa;
                result.push_back( inBetween );
            }
        }
        else
            result.push_back( current );
        pPrevious = &current;
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawLine
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawLine( CT_PointVector& points, MT_Float rArrowSize, bool bDoubleArrow )
{
    const T_PointVector pointList = Refine( points );
    if( pointList.size() <= 1 )
        return;

    if( ! MOS_App::GetApp().Is3D() )
    {
        glBegin( GL_LINE_STRIP );
        for( CIT_PointVector it = pointList.begin(); it != pointList.end(); ++it )
            glVertex3f( (*it).rX_, (*it).rY_, 0 );
        glEnd();
    }
    else
    {
        if( bSimpleMode_ )
        {
            glBegin( GL_LINE_STRIP );
            for( CIT_PointVector it = pointList.begin(); it != pointList.end(); ++it )
                Vertex( (*it).rX_, (*it).rY_, 1 );
            glEnd();
        }
        else
        {
            glBegin( GL_LINE_LOOP );
            for( CIT_PointVector it = pointList.begin(); it != pointList.end(); ++it )
                Vertex( (*it).rX_, (*it).rY_, -2 );
            for( CRIT_PointVector it = pointList.rbegin(); it != pointList.rend(); ++it )
                Vertex( (*it).rX_, (*it).rY_,  1 );
            glEnd();

            float color[4];
            glGetFloatv( GL_CURRENT_COLOR, color );
            color[3] = 0.5f;
            glColor4fv( color );
            glBegin( GL_QUAD_STRIP );
            for( CIT_PointVector it = pointList.begin(); it != pointList.end(); ++it )
            {
                Vertex( (*it).rX_, (*it).rY_, -2 );
                Vertex( (*it).rX_, (*it).rY_,  1 );
            }
            glEnd();
        }
    }

    if( rArrowSize == 0 )
        return;

    DrawArrow( pointList[pointList.size()-2], pointList[pointList.size()-1], rArrowSize );
    if( bDoubleArrow )
        DrawArrow( pointList[1], pointList[0], rArrowSize );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawLine
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw3DLine( MT_Vector3D src, MT_Vector3D dst, float width, GFX_Color color )
{
    color.SetGLColor();
    glLineWidth( width );

    if( ! MOS_App::GetApp().Is3D() )
    {
        glBegin( GL_LINE_STRIP );
        glVertex3f( src.rX_, src.rY_, src.rZ_ );
        glVertex3f( dst.rX_, dst.rY_, dst.rZ_ );
        glEnd();
    }
    else
    {
        glBegin( GL_LINE_STRIP );
        Vertex( src.rX_, src.rY_, 1 );
        Vertex( dst.rX_, dst.rY_, 1 );
        glEnd();
     }
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawArrow
// Created: AGE 2005-05-09
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawArrow( const MT_Vector2D& src, const MT_Vector2D& dest, MT_Float rArrowSize )
{
    MT_Vector2D u( (src - dest).Normalized() * ( rArrowSize * 2.0 ) );
    MT_Vector2D v( 0.5 * u );
    v.Rotate90ClockWise();

    T_PointVector points;
    points.reserve( 3 );
    points.push_back( dest + u + v ); points.push_back( dest ); points.push_back( dest + u - v );
    DrawLine( points );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawLine
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawLine( const MT_Vector2D& src, const MT_Vector2D& dest, MT_Float rArrowSize, bool bDoubleArrow )
{
    if( src == dest )
        return;

    T_PointVector points;
    points.reserve( 2 );
    points.push_back( src ); points.push_back( dest );
    DrawLine( points, rArrowSize, bDoubleArrow );
}



// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawPoints
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawPoints( CT_PointVector& pointList )
{
    if( ! MOS_App::GetApp().Is3D() )
    {
        glBegin( GL_POINTS );
        for( CIT_PointVector it = pointList.begin(); it != pointList.end(); ++it )
            glVertex3f( (*it).rX_, (*it).rY_, 0 );
        glEnd();
    }
    else
    {
        glBegin( GL_LINES );
        for( CIT_PointVector it = pointList.begin(); it != pointList.end(); ++it )
        {
            Vertex( (*it).rX_, (*it).rY_, -2 );
            Vertex( (*it).rX_, (*it).rY_,  1 );
        }
        glEnd();
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawPoint
// Created: AGE 2005-05-09
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawPoint( const MT_Vector2D& p )
{
    T_PointVector point;
    point.push_back( p );
    DrawPoints( point );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawPolygon
// Created: APE 2004-05-07
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawPolygon( CT_PointVector& pointList )
{
    if( pointList.empty() )
        return;
    T_PointVector points( pointList );
    points.push_back( points.front() );
    DrawLine( points );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawRect
// Created: APE 2004-06-03
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawRect( const MT_Vector2D& point1, const MT_Vector2D& point2, bool bInside )
{
    if( ! bInside )
    {
        T_PointVector pointList; pointList.reserve( 4 );
        pointList.push_back( point1 );
        pointList.push_back( MT_Vector2D( point2.rX_, point1.rY_ ) );
        pointList.push_back( point2 );
        pointList.push_back( MT_Vector2D( point1.rX_, point2.rY_ ) );
        DrawLine( pointList );
    }
    else
    {
        glBegin( GL_QUADS );
        if( MOS_App::GetApp().Is3D() )
        {
            Vertex( point1.rX_, point1.rY_ );
            Vertex( point2.rX_, point1.rY_ );
            Vertex( point2.rX_, point2.rY_ );
            Vertex( point1.rX_, point2.rY_ );
        }
        else
        {
            glVertex2f( point1.rX_, point1.rY_ );
            glVertex2f( point2.rX_, point1.rY_ );
            glVertex2f( point2.rX_, point2.rY_ );
            glVertex2f( point1.rX_, point2.rY_ );
        }
        glEnd();
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawUnit
// Created: APE 2004-06-10
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawUnit( MOS_Agent& agent, E_State nState )
{
    static const MOS_RawVisionData& data = MOS_App::GetApp().GetRawVisionData();

    MT_Float rSize = 600.;

    GFX_Color color = MOS_GLTool::GetColorForTeam( agent.GetTeam() );
    if( nState == eSelected )
        color.AddRGB( 50, 200, 50 );
    if( nState == eHighlighted )
        color.AddRGB( 50, 100, 50 );
    //$$$ Redo selection colors.

    glPushMatrix();

    MT_Float rAltitude = 0; 
    if( MOS_App::GetApp().Is3D() )
        rAltitude = ( data.GetHeight( agent.vPos_.rX_, agent.vPos_.rY_ ) + 10 ) * rZRatio_;
    glTranslatef( agent.vPos_.rX_, agent.vPos_.rY_, rAltitude );

    if( MOS_App::GetApp().Is3D() )
    {
        float modelview[ 16 ];
	    glPushMatrix();
	    glGetFloatv( GL_MODELVIEW_MATRIX, modelview );
	    // undo all rotations
	    for( uint i = 0; i < 3; i++ ) 
	        for( uint j = 0; j < 3; j++ )
                modelview[ i * 4 + j ] = ( i == j ) ? 1.0 : 0.0;
	    glLoadMatrixf( modelview );
    }

    //aggregated mode ?
    if( agent.IsAutomate() && agent.IsAggregated() )
        rSize *= 2;

    GFX_Tools::CreateGLAgentShadow( MT_Vector2D(0, 0), rSize, 4., 8., color, true, agent.symbolName_     , agent.nRawOpState_, agent.IsAutomate() && agent.IsEmbraye() );
    if( ! ( agent.IsAutomate() && agent.IsAggregated() ) )
        GFX_Tools::CreateGLAgentShadow( MT_Vector2D(0, 0), rSize, 4., 8., color, true, agent.levelSymbolName_, -1 );

    if( MOS_App::GetApp().Is3D() )
        glPopMatrix();

    if( agent.IsAutomate() && agent.IsAggregated() )
        GFX_Tools::CreateGLAgentShadow( MT_Vector2D(0, 0), rSize, 4., 8., color , true, "W", -1 );
    else if ( agent.IsAutomate() )
        GFX_Tools::CreateGLAgentShadow( MT_Vector2D(0, 0), rSize, 4., 8., color , true, "O", -1 );

    glPopMatrix();

    DeltaZ dz( 0.25 );
    color.SetGLColor();
    DrawCross( agent.vPos_, MOS_GL_CROSSSIZE, 4.0 );

//    for( std::vector< MT_Vector2D >::const_iterator it = agent.reportPoints_.begin(); it != agent.reportPoints_.end(); ++it )
//        DrawCross( *it, MOS_GL_CROSSSIZE, 4.0 );

    std::vector< MT_Vector2D > debugPoints = agent.GetDebugDrawPoints();
    if( !debugPoints.empty() )
    {
        GFX_Color color( 0, 255, 0, 1 );
        color.SetGLColor();

        std::vector< MT_Vector2D >::const_iterator itCur  = debugPoints.begin();
        std::vector< MT_Vector2D >::const_iterator itNext = itCur;
        ++ itNext;
        for( ; itNext != debugPoints.end(); ++itCur, ++itNext )
            DrawLine( *itCur, *itNext );

        DrawCross( *itCur, MOS_GL_CROSSSIZE, 4.0 );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawVisionCones
// Created: APE 2004-06-15
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawVisionCones( MOS_Agent& agent )
{
    glLineWidth( 1 );
    glColor4d( MOS_COLOR_WHITE );

    for( MOS_Agent::CIT_SurfaceVector itSurface = agent.surfaces_.begin(); itSurface != agent.surfaces_.end(); ++itSurface )
        (**itSurface).Draw( agent );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawVisionSurfaces
// Created: APE 2004-06-16
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawVisionSurfaces( MOS_Agent& agent )
{
    DeltaZ dz( 0.25 );
    if( agent.bVisionSurfacesNeedUpdating_ )
    {
        agent.surfaceVision_.clear();
        for( MOS_Agent::CIT_SurfaceVector itSurface = agent.surfaces_.begin(); itSurface != agent.surfaces_.end(); ++itSurface )
            (*itSurface)->UpdateVisionMatrice( agent, agent.surfaceVision_ );
        agent.bVisionSurfacesNeedUpdating_ = false;
    }

    glLineWidth( 1 );
    for( CIT_VisionResultMap it = agent.surfaceVision_.begin(); it != agent.surfaceVision_.end(); ++it )
    {
        if( it->second == eNotSeen )
            continue;

        if( it->second == eIdentification )
            glColor4d( MOS_COLOR_VISION_IDENTIED );
        else if( it->second == eRecognition )
            glColor4d( MOS_COLOR_VISION_RECO );
        else
            glColor4d( MOS_COLOR_VISION_DETECTED );

        MT_Vector2D bl = ( it->first );
        MOS_App::GetApp().GetRawVisionData().AlignToCell( bl, MOS_RawVisionData::eBottomLeft );

        MT_Vector2D tr = ( it->first );
        MOS_App::GetApp().GetRawVisionData().AlignToCell( tr, MOS_RawVisionData::eTopRight );

        DrawRect( bl, tr, 1.0 );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawLines
// Created: APE 2004-03-04
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawLines( T_MOSLinePtrVector& lines, float /*rZ */)
{
    glBegin( GL_LINES );
    T_PointVector points( 2 );
    for( CIT_MOSLinePtrVector it = lines.begin() ; it != lines.end() ; ++it )
    {
        const MT_Line& line = (**it).GetLine();
        points[0] = line.GetPosStart();
        points[1] = line.GetPosEnd  ();
        DrawLine( points );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Draw3DLines
// Created: HME 2005-10-04
// -----------------------------------------------------------------------------
void MOS_GLTool::Draw3DLines( T_Point3DVector& points, float width, GFX_Color color )
{
    if( points.size() < 2 )
        return;
    glBegin( GL_LINES );
    MT_Vector3D last = *points.begin();
    for( CIT_Point3DVector it = points.begin() ; it != points.end() ; ++it )
    {
        Draw3DLine( *it, last, width, color );
        last = *it;
    }
    glEnd();
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawCircles
// Created: APE 2004-03-04
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawCircles( T_NodePtrVector& nodes, MT_Float rRadius )
{
    for( IT_NodePtrVector it = nodes.begin(); it != nodes.end(); ++it )
        DrawCircle( (*it)->GetPos(), rRadius );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawCircle
// Created: APE 2004-03-17
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawCircle( const MT_Vector2D& center, MT_Float rRadius, bool bFill )
{
    T_PointVector points;
    points.reserve( 24 );
    for( MT_Float angle = 0; angle <= 2 * MT_PI; angle += 0.3f )
        points.push_back( MT_Vector2D( center.rX_ + rRadius * cos( angle )
                                     , center.rY_ + rRadius * sin( angle ) ) );
    points.push_back( MT_Vector2D( center.rX_ + rRadius, center.rY_ ) );

    if( bFill )
    {
        glBegin( GL_TRIANGLE_FAN );
        if( ! MOS_App::GetApp().Is3D() )
        {
            glVertex3f( center.rX_, center.rY_, 0 );
            for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
                glVertex3f( it->rX_, it->rY_ , 0 );
        }
        else
        {
            Vertex( center.rX_, center.rY_, 1 );
            for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
                Vertex( it->rX_, it->rY_ , 1 );
        }
        glEnd();
    }
    DrawLine( points );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawArc
// Created: APE 2004-06-16
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawArc( const MT_Vector2D& center, MT_Float rRadius, MT_Float rAngleStart, MT_Float rAngleEnd , bool arrowed)
{
    if( rAngleStart > rAngleEnd )
        std::swap( rAngleStart, rAngleEnd );

    T_PointVector points;
    points.reserve( 24 );
    float incr = (rAngleEnd - rAngleStart) / 24.0;
    for( MT_Float rAngle = rAngleStart; rAngle <= rAngleEnd; rAngle += incr )
        points.push_back( MT_Vector2D( center.rX_ + rRadius * cos( rAngle )
                                     , center.rY_ + rRadius * sin( rAngle ) ) );
    points.push_back( MT_Vector2D( center.rX_ + rRadius * cos( rAngleEnd )
                                 , center.rY_ + rRadius * sin( rAngleEnd ) ) );
    DrawLine( points );
    if( arrowed && ( points.size() > 2 ) )
    {
        DrawArrow( *(--(--(--(points.end())))), *(--(points.end())), 100.0 );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawGrid
// Created: APE 2004-03-22
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawGrid( MOS_World& world )
{
    MT_Float rGridStep = MOS_MainWindow::GetMainWindow().GetOptions().rGridStep_;
    const MT_Rect& worldRect = world.GetRect();

    glColor4d( 1.0, 1.0, 1.0, 0.3 );
    glLineWidth( 1.0 );

    glBegin( GL_LINES );
    for( MT_Float rX = worldRect.GetLeft(); rX < worldRect.GetRight(); rX += rGridStep * 1000.0)
    {
        glVertex2f( rX, worldRect.GetTop()    );
        glVertex2f( rX, worldRect.GetBottom() );
    }

    for( MT_Float rY = worldRect.GetBottom(); rY < worldRect.GetTop(); rY += rGridStep *1000.0)
    {
        glVertex2f( worldRect.GetLeft(),  rY );
        glVertex2f( worldRect.GetRight(), rY );
    }
    glEnd();
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawCross
// Created: APE 2004-04-23
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawCross( const MT_Vector2D& vCenter, double rSize, int nLineWidth )
{
    glLineWidth( nLineWidth );
    DrawLine( MT_Vector2D( vCenter.rX_ - rSize, vCenter.rY_ - rSize )
            , MT_Vector2D( vCenter.rX_ + rSize, vCenter.rY_ + rSize ) );
    DrawLine( MT_Vector2D( vCenter.rX_ + rSize, vCenter.rY_ - rSize )
            , MT_Vector2D( vCenter.rX_ - rSize, vCenter.rY_ + rSize ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::Overlaps
// Created: APE 2004-03-04
// -----------------------------------------------------------------------------
bool MOS_GLTool::Overlaps( MT_Rect& a, MT_Rect& b )
{
    return(   a.GetTop()    >= b.GetBottom()
           && a.GetBottom() <= b.GetTop()
           && a.GetLeft()   <= b.GetRight()
           && a.GetRight()  >= b.GetLeft() );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLTool::InitializeIcon
// Created: APE 2004-09-13
// -----------------------------------------------------------------------------
int MOS_GLTool::InitializeIcon( const QImage& icon )
{
    if( icon.isNull() )
        return -1;

    QImage image = icon.convertDepth( 32 );

    int nTrueWidth = image.width();
    int nTrueHeight = image.height();
    int nWidth = 2;
    int nHeight = 2;

    while( nWidth < nTrueWidth )
        nWidth *= 2;
    while( nHeight < nTrueHeight )
        nHeight *= 2;

    QImage resizedImage = image.copy( 0, 0, nWidth, nHeight );

    glNormal3f( 0.0, 0.0, 1.0 );
    unsigned int nTexture;
    glGenTextures( 1, & nTexture );
    glBindTexture( GL_TEXTURE_2D, nTexture );
    glTexImage2D( GL_TEXTURE_2D, 0, 4, resizedImage.width(), resizedImage.height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, resizedImage.bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    float rWidthRatio = (float)nTrueWidth / (float)nWidth;
    float rHeightRatio = (float)nTrueHeight / (float)nHeight;

    int nListId = glGenLists(1);
    glNewList( nListId, GL_COMPILE);

    glBindTexture( GL_TEXTURE_2D, nTexture );
    glBegin( GL_QUADS );
        glTexCoord2f( 0, rHeightRatio );
        glVertex2f( -0.5, -0.5 );
        glTexCoord2f( rWidthRatio, rHeightRatio );
        glVertex2f( 0.5, -0.5 );
        glTexCoord2f( rWidthRatio, 0 );
        glVertex2f( 0.5, 0.5 );
        glTexCoord2f( 0, 0 );
        glVertex2f( -0.5, 0.5 );
    glEnd();
    glBindTexture( GL_TEXTURE_2D, 0 );

    glEndList();

    return nListId;
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawIcon
// Created: APE 2004-09-14
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawIcon( E_Icon nIcon, const MT_Vector2D& vCenter, float rSize )
{
    glPushMatrix();
    glTranslated( vCenter.rX_ + iconLocations_[nIcon].rX_, vCenter.rY_ + iconLocations_[nIcon].rY_, 0.0 );
    glScalef( rSize, rSize, rSize );
    glCallList( iconIds_[nIcon] );
    glPopMatrix();
}

namespace
{
    std::map< std::string, GFX_Color > InitializeColors()
    {
        std::map< std::string, GFX_Color > colors;
        colors.insert( std::make_pair( "rouge",   GFX_Color( 255, 000, 000 ) ) );
        colors.insert( std::make_pair( "bleu",    GFX_Color( 000, 000, 255 ) ) );
        colors.insert( std::make_pair( "vert",    GFX_Color( 000, 255, 000 ) ) );
        colors.insert( std::make_pair( "cyan",    GFX_Color( 000, 255, 255 ) ) );
        colors.insert( std::make_pair( "magenta", GFX_Color( 255, 000, 255 ) ) );
        colors.insert( std::make_pair( "orange",  GFX_Color( 255, 150, 10 ) ) );
        colors.insert( std::make_pair( "marron",  GFX_Color( 139, 69, 19 ) ) );
        colors.insert( std::make_pair( "jaune",   GFX_Color( 255, 255, 000 ) ) );
        colors.insert( std::make_pair( "gris",    GFX_Color( 100, 100, 100 ) ) );
        colors.insert( std::make_pair( "noir",    GFX_Color( 0, 0, 0 ) ) );
        colors.insert( std::make_pair( "cobalt",    GFX_Color( 0, 0, 128 ) ) );
        colors.insert( std::make_pair( "",        GFX_Color( 255, 100, 255 ) ) );

        return colors;
    };
    std::map< std::string, std::string > InitializeSynonyms()
    {
        std::map< std::string, std::string > synonyms;
        synonyms[ "rouge"  ] = "rouge";
        synonyms[ "rouges"  ] = "rouge";
        synonyms[ "red"    ] = "rouge";
        synonyms[ "ennemi" ] = "rouge";
        synonyms[ "ennemis" ] = "rouge";
        synonyms[ "enemy" ] = "rouge";
        synonyms[ "enemies" ] = "rouge";
        synonyms[ "eni"    ] = "rouge";
        synonyms[ "enis"    ] = "rouge";
        synonyms[ "glaise" ] = "rouge";
        synonyms[ "humus" ] = "rouge";
        synonyms[ "tourbe" ] = "rouge";
        synonyms[ "amarante" ] = "rouge";
        synonyms[ "amarantes" ] = "rouge";
        synonyms[ "aniline" ] = "rouge";
        synonyms[ "anilines" ] = "rouge";
        synonyms[ "brique" ] = "rouge";
        synonyms[ "briques" ] = "rouge";
        synonyms[ "carmin" ] = "rouge";
        synonyms[ "carmins" ] = "rouge";
        synonyms[ "cerise" ] = "rouge";
        synonyms[ "cerises" ] = "rouge";
        synonyms[ "corail" ] = "rouge";
        synonyms[ "coraux" ] = "rouge";
        synonyms[ "corails" ] = "rouge";
        synonyms[ "garance" ] = "rouge";
        synonyms[ "garances" ] = "rouge";
        synonyms[ "grenat" ] = "rouge";
        synonyms[ "grenats" ] = "rouge";
        synonyms[ "pourpre" ] = "rouge";
        synonyms[ "pourpres" ] = "rouge";
        synonyms[ "vermeil" ] = "rouge";
        synonyms[ "vermeils" ] = "rouge";
        
        synonyms[ "bleu"   ] = "bleu";
        synonyms[ "bleus"   ] = "bleu";
        synonyms[ "blue"   ] = "bleu";
        synonyms[ "ami"    ] = "bleu";
        synonyms[ "amis"    ] = "bleu";
        synonyms[ "friend"    ] = "bleu";
        synonyms[ "friends"    ] = "bleu";

        synonyms[ "gris"   ] = "gris";
        synonyms[ "blanc"   ] = "gris";
        synonyms[ "blancs"   ] = "gris";
        synonyms[ "grey"   ] = "gris";
        synonyms[ "white"   ] = "gris";
        synonyms[ "neutre" ] = "gris";
        synonyms[ "neutres" ] = "gris";
        synonyms[ "neutral" ] = "gris";
        synonyms[ "neutrals" ] = "gris";
        synonyms[ "argent" ] = "gris";
        synonyms[ "argents" ] = "gris";
        synonyms[ "silver" ] = "gris";
        synonyms[ "silvers" ] = "gris";
        synonyms[ "platine" ] = "gris";
        synonyms[ "platines" ] = "gris";

        synonyms[ "cobalt" ] = "cobalt";
        synonyms[ "azur" ] = "cobalt";
        synonyms[ "ardoise" ] = "cobalt";
        synonyms[ "cobalts" ] = "cobalt";
        synonyms[ "azurs" ] = "cobalt";
        synonyms[ "ardoises" ] = "cobalt";

        synonyms[ "turquoise" ] = "cyan";
        synonyms[ "cyan" ] = "cyan";
        synonyms[ "turquoises" ] = "cyan";
        synonyms[ "cyans" ] = "cyan";

        synonyms[ "orange" ] = "orange";
        synonyms[ "oranges" ] = "orange";
        synonyms[ "orange" ] = "orange";
        synonyms[ "orange" ] = "orange";
        synonyms[ "ocre" ] = "orange";
        synonyms[ "ocres" ] = "orange";
        synonyms[ "cuivre" ] = "orange";
        synonyms[ "cuivres" ] = "orange";

        synonyms[ "vert" ] = "vert";
        synonyms[ "verts" ] = "vert";
        synonyms[ "green" ] = "vert";
        synonyms[ "greens" ] = "vert";
        synonyms[ "amande" ] = "vert";
        synonyms[ "amandes" ] = "vert";
        synonyms[ "émeraudes" ] = "vert";
        synonyms[ "émeraude" ] = "vert";
        synonyms[ "jade" ] = "vert";
        synonyms[ "jades" ] = "vert";
        synonyms[ "mélèze" ] = "vert";
        synonyms[ "mélèzes" ] = "vert";
        synonyms[ "olive" ] = "vert";
        synonyms[ "olives" ] = "vert";


        synonyms[ "marron" ] = "marron";
        synonyms[ "marrons" ] = "marron";
        synonyms[ "maroon" ] = "marron";
        synonyms[ "maroons" ] = "marron";
        synonyms[ "brown" ] = "marron";
        synonyms[ "browns" ] = "marron";
        synonyms[ "acajou" ] = "marron";
        synonyms[ "acajous" ] = "marron";
        synonyms[ "auburn" ] = "marron";
        synonyms[ "auburns" ] = "marron";
        synonyms[ "bruns" ] = "marron";
        synonyms[ "brun" ] = "marron";
        synonyms[ "caramel" ] = "marron";
        synonyms[ "caramels" ] = "marron";
        synonyms[ "chataigne" ] = "marron";
        synonyms[ "chataignes" ] = "marron";
        synonyms[ "mordorés" ] = "marron";
        synonyms[ "mordorés" ] = "marron";
        synonyms[ "noisettes" ] = "marron";
        synonyms[ "noisettes" ] = "marron";

        synonyms[ "noir" ] = "noir";
        synonyms[ "noirs" ] = "noirs";
        synonyms[ "black" ] = "noir";
        synonyms[ "blacks" ] = "noir";
        synonyms[ "chocolat" ] = "noir";
        synonyms[ "chocolats" ] = "noir";
        synonyms[ "bitume" ] = "noir";
        synonyms[ "cachou" ] = "noir";
        synonyms[ "bitumes" ] = "noir";
        synonyms[ "cachous" ] = "noir";
        synonyms[ "charbon" ] = "noir";
        synonyms[ "carbone" ] = "noir";
        synonyms[ "ébène" ] = "noir";
        synonyms[ "ivoire" ] = "noir";
        synonyms[ "jais" ] = "noir";
        synonyms[ "réglisse" ] = "noir";
        synonyms[ "charbons" ] = "noir";
        synonyms[ "carbones" ] = "noir";
        synonyms[ "ébènes" ] = "noir";
        synonyms[ "ivoires" ] = "noir";
        synonyms[ "réglisses" ] = "noir";

        synonyms[ "jaune" ] = "jaune";
        synonyms[ "jaunes" ] = "jaune";
        synonyms[ "gold" ] = "jaune";
        synonyms[ "yellow" ] = "jaune";
        synonyms[ "golds" ] = "jaune";
        synonyms[ "yellows" ] = "jaune";
        synonyms[ "or" ] = "jaune";
        synonyms[ "ors" ] = "jaune";
        synonyms[ "topaze" ] = "jaune";
        synonyms[ "soufre" ] = "jaune";
        synonyms[ "safran" ] = "jaune";
        synonyms[ "mimoza" ] = "jaune";
        synonyms[ "citron" ] = "jaune";
        synonyms[ "chrome" ] = "jaune";
        synonyms[ "blé" ] = "jaune";
        synonyms[ "ambre" ] = "jaune";
        synonyms[ "topazes" ] = "jaune";
        synonyms[ "soufres" ] = "jaune";
        synonyms[ "safrans" ] = "jaune";
        synonyms[ "mimozas" ] = "jaune";
        synonyms[ "citrons" ] = "jaune";
        synonyms[ "chromes" ] = "jaune";
        synonyms[ "blés" ] = "jaune";
        synonyms[ "ambres" ] = "jaune";
        synonyms[ "sable" ] = "jaune";
        synonyms[ "sables" ] = "jaune";

        synonyms[ "fuchsia" ] = "magenta";
        synonyms[ "fuchsias" ] = "magenta";
        synonyms[ "magenta" ] = "magenta";
        synonyms[ "magentas" ] = "magenta";
        synonyms[ "rose" ] = "magenta";
        synonyms[ "roses" ] = "magenta";
        synonyms[ "mauve" ] = "magenta";
        synonyms[ "mauves" ] = "magenta";
        
        synonyms[ "inconnu" ] = "";
        return synonyms;
    }

    unsigned char RandomChar()
    {
        return 63 + rand() % 192;
    };

    GFX_Color RandomColor()
    {
        return GFX_Color( RandomChar(), RandomChar(), RandomChar() );
    }
};

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::GetColorForTeam
// Created: AGE 2005-04-22
// -----------------------------------------------------------------------------
GFX_Color MOS_GLTool::GetColorForTeam( const MOS_Team& team )
{
    typedef std::map< std::string, std::string > T_StringMap;
    static T_StringMap synonyms = InitializeSynonyms();
    typedef std::map< std::string, GFX_Color > T_ColorMap;
    static T_ColorMap colors = InitializeColors();
    typedef std::map< std::string, GFX_Color > T_TeamColorMap;

    static T_TeamColorMap teamColors;
    T_TeamColorMap::iterator itTeam = teamColors.find( team.GetName() );
    if( itTeam != teamColors.end() )
        return itTeam->second;

    std::string strTeamName = lower( team.GetName() );
    T_ColorMap::iterator itColor = colors.find( synonyms[ strTeamName ] );
    if( itColor == colors.end() )
        itColor = colors.begin();
    GFX_Color color( 0, 0, 0 );
    if( colors.empty() )
        color = RandomColor();
    else
    {
        color = itColor->second;
        colors.erase( itColor );
    }
    teamColors.insert( std::make_pair( team.GetName(), color ) );
    return color;
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::SetAltitudeRatio
// Created: AGE 2005-05-16
// -----------------------------------------------------------------------------
void MOS_GLTool::SetAltitudeRatio( MT_Float r )
{
    rZRatio_ = r;
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawCylinder
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawCylinder( const MT_Vector2D& center, MT_Float rRadius, MT_Float rHeight, GFX_Color& color )
{
    T_Point3DVector points;
    points.reserve( 24 );

    for( MT_Float angle = 0 ; angle <= 2 * MT_PI; angle += 0.3f )
        points.push_back( MT_Vector3D( center.rX_ + rRadius * cos( angle )
                                     , center.rY_ + rRadius * sin( angle )
                                     , 0 ) );
    points.push_back( MT_Vector3D( center.rX_ + rRadius, center.rY_, 0 ) );

    points.push_back( *points.begin() );

    MT_Vector3D height = MT_Vector3D( 0, 0, rHeight );
    MT_Vector3D base   = MT_Vector3D( 0, 0, 0 );
    MT_Vector3D last = *points.begin();
    IT_Point3DVector itPointPrev = points.begin();
    IT_Point3DVector itPointNext = points.begin();
    ++itPointNext;
    for( ; itPointNext != points.end(); ++itPointPrev, ++itPointNext )
    {
        DrawTriangle3D( *itPointPrev + base, *itPointNext + base, *itPointPrev + height, color );
        DrawTriangle3D( *itPointNext + height, *itPointPrev + height, *itPointNext + base , color );
        GFX_Tools::CreateGLLine3D( *itPointPrev + height, *itPointNext + height, 1.0, color);
        GFX_Tools::CreateGLLine3D( *itPointNext + base, *itPointPrev + height, 1.0, color);
    }

    glBegin( GL_TRIANGLE_FAN );
    Vertex( center.rX_, center.rY_, 0 );
    for( CIT_Point3DVector it = points.begin(); it != points.end(); ++it )
            Vertex( it->rX_, it->rY_, 0 );
    glEnd();

    glBegin( GL_TRIANGLE_FAN );
    Vertex( center.rX_, center.rY_, rHeight  );
    for( CIT_Point3DVector it = points.begin(); it != points.end(); ++it )
            Vertex( it->rX_, it->rY_, rHeight );
    glEnd();
}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawTriangle3D
// Created: HME 2005-10-07
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawTriangle3D( const MT_Vector3D& vPos1 , const MT_Vector3D& vPos2 , const MT_Vector3D& vPos3 , const GFX_Color& color )
{
    glBindTexture( GL_TEXTURE_2D, 0 );
    color.SetGLColor();


    glBegin( GL_TRIANGLES );
    glVertex3f( vPos1.rX_, vPos1.rY_, vPos1.rZ_ );
    glVertex3f( vPos2.rX_, vPos2.rY_, vPos2.rZ_ );
    glVertex3f( vPos3.rX_, vPos3.rY_, vPos3.rZ_);
    glEnd();

}

// -----------------------------------------------------------------------------
// Name: MOS_GLTool::DrawArc
// Created: HME 2005-11-03
// -----------------------------------------------------------------------------
void MOS_GLTool::DrawArc ( const MT_Vector2D& src, const MT_Vector2D& dst, MT_Float height, bool arrowed )
{
    if ( src == dst )
        return;
    if ( height == 0 )
    {       DrawLine( src, dst );
            return;
    }
    float middle_x = (dst.rX_ + src.rX_) / 2.0;
    float middle_y = (dst.rY_ + src.rY_) / 2.0;
    //const MT_Vector2D middle = MT_Vector2D( middle_x, middle_y ); 
    //DrawCircle( middle, 50 );
    float dist2 = (dst.rX_ - src.rX_) * (dst.rX_ - src.rX_) + (dst.rY_ - src.rY_) * (dst.rY_ - src.rY_);
    float dist = sqrt( dist2 );
    float radius = (dist2 / (8 * height)) + (height / 2);
    float ortho_x = (dst.rY_ - src.rY_) / dist;
    float ortho_y = (src.rX_ - dst.rX_) / dist;
    MT_Vector2D center = MT_Vector2D( middle_x + (radius - height)* ortho_x,  middle_y + (radius - height)* ortho_y );
    //MT_Vector2D center2 = MT_Vector2D( middle_x - (radius - height)* ortho_x,  middle_y - (radius - height)* ortho_y );
    float angleStart = acos( (dst.rX_ - center.rX_) / radius );
    if ( asin( ( dst.rY_ - center.rY_ ) / radius ) < 0 )
        angleStart = 6.283 - angleStart;
    float angleEnd = acos( (src.rX_ - center.rX_) / radius );
    if ( asin( (src.rY_ - center.rY_) / radius ) < 0 )
        angleEnd = 6.283 - angleEnd;
    //DrawCircle( center, radius );
    if ( abs(angleEnd - angleStart) > 3.141 )
        if ( ((angleEnd > angleStart) ? angleEnd : angleStart ) == angleStart )
            angleStart = angleStart - 6.283;
        else
            angleEnd = angleEnd - 6.283;
    DrawArc( center, radius, angleStart, angleEnd, arrowed );
}



// -----------------------------------------------------------------------------
// Name: MOS_GLTool::IconOfObjectType
// Created: HME 2005-11-14
// -----------------------------------------------------------------------------
MOS_GLTool::E_Icon MOS_GLTool::IconOfObjectType( E_ObjectType nType )
{
    switch( nType )
    {
    case eObjectType_ZoneProtegee: return objectTypeZoneProtegee ;
    case eObjectType_ZonePoserHelicoptere: return objectTypeZonePoserHelicoptere ;
    case eObjectType_ZoneNbc: return objectTypeZoneNBC ;
    case eObjectType_ZoneMobiliteAmelioree: return objectTypeZoneMobiliteAmelioree ;
    case eObjectType_ZoneInterditeTir: return objectTypeZoneInterditeTir ;
    case eObjectType_ZoneInterditeMouvement: return objectTypeZoneInterditeMvt ;
    case eObjectType_ZoneImplantationLrm: return objectTypeZoneImplantationLrm ;
    case eObjectType_ZoneImplantationCobra: return objectTypeZoneImplantationCobra ;
    case eObjectType_ZoneImplantationCanon: return objectTypeZoneImplantationCanon ;
    case eObjectType_ZoneBrouillageBrod: return objectTypeZoneBrouillardBrod ;
    case eObjectType_ZoneBrouillageBromure: return objectTypeZoneBrouillageBromure ;
    case eObjectType_TerrainLargage: return objectTypeTerrainLargage ;
    case eObjectType_SiteFranchissement: return objectTypeSiteFranchissement ;
    case eObjectType_SiteDecontamination: return objectTypeSiteDecontamination ;
    case eObjectType_Rota: return objectTypeRota ;
    case eObjectType_PosteTir: return objectTypePosteTir ;
    case eObjectType_PosteControle: return objectTypePosteControle ;
    case eObjectType_PontFlottantContinu: return objectTypePontFlottant ;
    case eObjectType_PontFlottantDiscontinu: return objectTypePontFlottant ;
    case eObjectType_PlotRavitaillement: return objectTypePlotRavitaillement ;
    case eObjectType_Plateforme: return objectTypePlateForme ;
    case eObjectType_Piste: return objectTypePiste ;
    case eObjectType_NuageNbc: return objectTypeNuageNBC ;
    case eObjectType_BouchonMines: return objectTypeMine ;
    case eObjectType_ZoneMineeLineaire: return objectTypeMine ;
    case eObjectType_ZoneMineeParDispersion: return objectTypeMine ;
    case eObjectType_ItineraireLogistique: return objectTypeItineraireLog ;
    case eObjectType_ZoneImplantationMortier: return objectTypeImplantationMortier ;
    case eObjectType_FosseAntiChar: return  objectTypeFosseAC;
    case eObjectType_Eboulement: return objectTypeEboulement ;
    case eObjectType_DestructionRoute: return objectTypeDestructionRoute ;
    case eObjectType_DestructionPont: return objectTypeDestructionPont ;
    case eObjectType_CampRefugies: return objectTypeCampRefugies ;
    case eObjectType_CampPrisonniers: return objectTypeCampPrisonniers ;
    case eObjectType_Barricade: return objectTypeBarricade ;
    case eObjectType_AirePoser: return objectTypeAirePoser ;
    case eObjectType_AireLogistique: return objectTypeAireLogistique ;
    case eObjectType_Abattis: return objectTypeAbattis ;
    default:
        return eNoneIcon;
    }
}
