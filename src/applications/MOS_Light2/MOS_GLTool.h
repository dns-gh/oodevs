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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_GLTool.h $
// $Author: Age $
// $Modtime: 16/05/05 16:42 $
// $Revision: 17 $
// $Workfile: MOS_GLTool.h $
//
// *****************************************************************************

#ifndef __MOS_GLTool_h_
#define __MOS_GLTool_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MT_Tools/MT_Rect.h"
#include "MOS_ShapeEditorMapEventFilter.h"

class MOS_World;
class MOS_Patch;
class MOS_ShapePatch;
class MOS_PatchTexture;
class MOS_AgentManager;
class MOS_Agent;
class MOS_AgentKnowledge;
class MOS_Team;
class MOS_Gtia;
class MOS_ObjectManager;
class MOS_Object_ABC;
class MOS_ObjectKnowledge;
class MOS_LineManager;
class MOS_TacticalLine_ABC;
class MOS_Meteo_Manager;
class MOS_MissionPanel;
class MOS_MissionInterface_ABC;
class MOS_WeatherPanel;
class MOS_ObjectCreationPanel;
class MOS_InfoPanel;
class MOS_ParamPath;
class MOS_ParamPathList;
class MOS_ParamLimaList;
class MOS_ParamLimits;
class MOS_ParamPoint;
class MOS_ParamAgent;
class MOS_ParamAgentList;
class MOS_ParamLocation;
class MOS_ParamLocationList;
class MOS_ParamObstacle;
class MOS_ParamObstacleList;
class MOS_MapEventHandler_ABC;
class MOS_DefaultMapEventHandler;
class MOS_ShapeEditorMapEventFilter;
class MOS_Population;
class MOS_PopulationFlow;
class MOS_PopulationConcentration;
class MOS_PopulationKnowledge;
class MOS_PopulationFlowKnowledge;
class MOS_PopulationConcentrationKnowledge;


// =============================================================================
/** @class  MOS_GLTool
    @brief  
    //$$$$$ Apparament, séparer l'affichage des données comme ca n'etait pas la
    //chose a faire... http://www.javaworld.com/javaworld/jw-07-1999/jw-07-toolbox-p5.html
*/ 
// Created: APE 2004-03-04
// =============================================================================
class MOS_GLTool
{
    MT_COPYNOTALLOWED( MOS_GLTool )

public:
    enum E_State
    {
        eNormal,
        eHighlighted,
        eSelected
    };

    enum E_Icon
    {
        eGasIcon,
        eAmmoIcon,
        eNbcIcon,
        eConstructionIcon,
        eObservingIcon,
        eFlareIcon,
        eSmokeIcon,
        eSkullIcon,
        eNbrIcons
    };

private:
    //! @name Constructors/Destructor
    //@{
    MOS_GLTool();
    ~MOS_GLTool();
    //@}

public:
    //! @name Operations
    //@{
    static void Draw( MT_Rect& viewRect, float rClicksPerPix );

    static void Draw( MOS_World& world );
    static void DrawAreas( MOS_World& world, unsigned int nMaxLod );
    static void DrawBorders( MOS_World& world, unsigned int nMaxLod );
    static void DrawLines( MOS_World& world, unsigned int nMaxLod );
    static void DrawDetection( MOS_World& world );
    static void DrawNameObjects( MOS_World& world );

    static void Draw( MOS_AgentManager& agentManager );
    static void Draw( MOS_Agent& agent, E_State nState = eNormal );

	static void Draw( MOS_Population&              population   , E_State nState = eNormal );
    static void Draw( MOS_PopulationConcentration& concentration, E_State nState = eNormal );
    static void Draw( MOS_PopulationFlow&          flow         , E_State nState = eNormal );

	static void DrawPopulation( MOS_Population& pop, E_State nState );
    static void DrawPath( MOS_Agent& agent );
    static void DrawPath( MOS_PopulationFlow& flow );
    static void DrawAutomataStatus( MOS_Agent& agent, E_State nState );
    static void DrawVisionLines( MOS_Agent& agent );
    static void Draw( MOS_Team& team );
    static void Draw( MOS_Gtia& gtia );

    static void Draw( MOS_AgentKnowledge&                   knowledge, E_State nState = eNormal );
    static void Draw( MOS_PopulationKnowledge&              knowledge, E_State nState = eNormal );
    static void Draw( MOS_PopulationConcentrationKnowledge& knowledge, E_State nState = eNormal );
    static void Draw( MOS_PopulationFlowKnowledge&          knowledge, E_State nState = eNormal );

    static void Draw( MOS_ObjectManager& manager );
    static void Draw( MOS_Object_ABC& object, E_State nState = eNormal );
    static void Draw( MOS_ObjectKnowledge& knowledge, E_State nState = eNormal );

    static void Draw( MOS_LineManager& lineManager );
    static void Draw( MOS_TacticalLine_ABC& line, E_State nState = eNormal, int nSelectedPoint = -1 );

    static void Draw( MOS_Meteo_Manager& weatherManager );

    static void Draw( MOS_MissionPanel& panel );
    static void Draw( MOS_MissionInterface_ABC& missionInterface );

    static void Draw( MOS_WeatherPanel& panel );

    static void Draw( MOS_ObjectCreationPanel& panel );

    static void Draw( MOS_InfoPanel& panel );

    static void Draw( MOS_ParamPath& param );
    static void Draw( MOS_ParamPathList& param );
    static void Draw( MOS_ParamLimaList& param );
    static void Draw( MOS_ParamLimits& param );
    static void Draw( MOS_ParamPoint& param );
    static void Draw( MOS_ParamAgent& param );
    static void Draw( MOS_ParamAgentList& param );
    static void Draw( MOS_ParamLocation& param, bool bSelected = false );
    static void Draw( MOS_ParamLocationList& param );
    static void Draw( MOS_ParamObstacle& param, bool bSelected = false );
    static void Draw( MOS_ParamObstacleList& param );

    static void Draw( const MOS_DefaultMapEventHandler& eventHandler );
    static void Draw( const MOS_ShapeEditorMapEventFilter& eventHandler );

    static void DrawLocation( MOS_ShapeEditorMapEventFilter::E_LineType nType, const T_PointVector& pointList );
    static void DrawLine    ( CT_PointVector& pointList, MT_Float rArrowSize = 0, bool bDoubleArrow = false );
    static void DrawLine    ( const MT_Vector2D& src, const MT_Vector2D& dest, MT_Float rArrowSize = 0, bool bDoubleArrow = false );
    static void DrawArrow   ( const MT_Vector2D& src, const MT_Vector2D& dest, MT_Float rArrowSize );
    static void DrawPoints  ( CT_PointVector& pointList );
    static void DrawPoint   ( const MT_Vector2D& p );
    static void DrawPolygon ( CT_PointVector& pointList );
    static void DrawRect    ( const MT_Vector2D& point1, const MT_Vector2D& point2, bool bInside );
	static void Draw3DLine  ( MT_Vector3D src, MT_Vector3D dst, float width, GFX_Color color );
	static void Draw3DLines ( T_Point3DVector& points, float width, GFX_Color color );

    static void DrawUnit          ( MOS_Agent& agent, E_State nState = eNormal );
    static void DrawVisionCones   ( MOS_Agent& agent );
    static void DrawVisionSurfaces( MOS_Agent& agent );

    static void DrawLines     ( T_MOSLinePtrVector& lines, float rZ );
    static void DrawCircles   ( T_NodePtrVector&    nodes, MT_Float rRadius );
    static void DrawCircle    ( const MT_Vector2D& center, MT_Float rRadius, bool bFill = false );
    static void DrawArc       ( const MT_Vector2D& center, MT_Float rRadius, MT_Float rAngleStart, MT_Float rAngleEnd );
    static void DrawGrid      ( MOS_World& world );
    static void DrawCross     ( const MT_Vector2D& vCenter, double rSize, int nLineWidth );
    static void Vertex        ( MT_Float rX, MT_Float rY, MT_Float rZSign = 1 );
	static void DrawCylinder  ( const MT_Vector2D& center, MT_Float rRadius, MT_Float rHeight, GFX_Color& color );
    static void DrawTriangle3D( const MT_Vector3D&, const MT_Vector3D&, const MT_Vector3D&, const GFX_Color& );

    static T_PointVector Refine( const T_PointVector& points );
    //@}

    //! @name Helpers
    //@{
    static bool Overlaps( MT_Rect& a, MT_Rect& b );
    static GFX_Color GetColorForTeam( const MOS_Team& team );

    class SimpleMode
    {
    public:
         SimpleMode() { bBackup_ = MOS_GLTool::bSimpleMode_; MOS_GLTool::bSimpleMode_ = true; };
        ~SimpleMode() { MOS_GLTool::bSimpleMode_ = bBackup_; };
    private:
        bool bBackup_;
    };

    class DeltaZ
    {
    public:
        DeltaZ( MT_Float rRatio ) { rBackup_ = MOS_GLTool::rDeltaZ_; MOS_GLTool::rDeltaZ_ *= rRatio; };
        ~DeltaZ() { MOS_GLTool::rDeltaZ_ = rBackup_; };
    private:
        MT_Float rBackup_;
    };
    static void SetAltitudeRatio( MT_Float r );
    //@}

    static int InitializeIcon( const QImage& icon );
    static void DrawIcon( E_Icon nIcon, const MT_Vector2D& vCenter, float rSize = 150.0 );

private:
    static uint nFrame_;
    static GLushort stipplePattern_[16]; 
    static float rClicksPerPix_;
    static MT_Rect viewRect_;

    static int iconIds_[eNbrIcons];
    static MT_Vector2D iconLocations_[eNbrIcons];

    static MT_Float  rZRatio_;
    static MT_Float  rDeltaZ_;
    static bool      bSimpleMode_;
};

#endif // __MOS_GLTool_h_
