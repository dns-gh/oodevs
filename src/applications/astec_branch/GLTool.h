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
// $Archive: /MVW_v10/Build/SDK/Light2/src/GLTool.h $
// $Author: Age $
// $Modtime: 16/05/05 16:42 $
// $Revision: 17 $
// $Workfile: GLTool.h $
//
// *****************************************************************************

#ifndef __GLTool_h_
#define __GLTool_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MT_Tools/MT_Rect.h"
#include "ShapeEditorMapEventFilter.h"

class World;
class Patch;
class ShapePatch;
class PatchTexture;
class AgentManager;
class Agent;
class AgentKnowledge;
class Team;
class Gtia;
class ObjectManager;
class Object_ABC;
class ObjectKnowledge;
class LineManager;
class TacticalLine_ABC;
class Meteo_Manager;
class MissionPanel;
class MissionInterface_ABC;
class WeatherPanel;
class ObjectCreationPanel;
class InfoPanel;
class ParamPath;
class ParamPathList;
class ParamLimaList;
class ParamLimits;
class ParamPoint;
class ParamAgent;
class ParamAgentList;
class ParamLocation;
class ParamLocationList;
class ParamObstacle;
class ParamObstacleList;
class MapEventHandler_ABC;
class DefaultMapEventHandler;
class ShapeEditorMapEventFilter;
class Population;
class PopulationFlow;
class PopulationConcentration;
class PopulationKnowledge;
class PopulationFlowKnowledge;
class PopulationConcentrationKnowledge;


// =============================================================================
/** @class  GLTool
    @brief  
    //$$$$$ Apparament, séparer l'affichage des données comme ca n'etait pas la
    //chose a faire... http://www.javaworld.com/javaworld/jw-07-1999/jw-07-toolbox-p5.html
*/ 
// Created: APE 2004-03-04
// =============================================================================
class GLTool
{
    MT_COPYNOTALLOWED( GLTool )

public:
    enum E_State
    {
        eNormal,
        eHighlighted,
        eSelected
    };

    enum E_Icon
    {
        eNoneIcon,
        eGasIcon,
        eAmmoIcon,
        eNbcIcon,
        eConstructionIcon,
        eObservingIcon,
        eFlareIcon,
        eSmokeIcon,
        eSkullIcon,
        objectTypeZoneProtegee,
        objectTypeZonePoserHelicoptere,
        objectTypeZoneNBC,
        objectTypeZoneMobiliteAmelioree,
        objectTypeZoneInterditeTir,
        objectTypeZoneInterditeMvt,
        objectTypeZoneImplantationLrm,
        objectTypeZoneImplantationCobra,
        objectTypeZoneImplantationCanon,
        objectTypeZoneBrouillardBrod,
        objectTypeZoneBrouillageBromure,
        objectTypeTerrainLargage,
        objectTypeSiteFranchissement,
        objectTypeSiteDecontamination,
        objectTypeRota,
        objectTypePosteTir,
        objectTypePosteControle,
        objectTypePontFlottant,
        objectTypePlotRavitaillement,
        objectTypePlateForme,
        objectTypePiste,
        objectTypeNuageNBC,
        objectTypeMine,
        objectTypeItineraireLog,
        objectTypeImplantationMortier,
        objectTypeFosseAC,
        objectTypeEboulement,
        objectTypeDestructionRoute,
        objectTypeDestructionPont,
        objectTypeCampRefugies,
        objectTypeCampPrisonniers,
        objectTypeBarricade,
        objectTypeAirePoser,
        objectTypeAireLogistique,
        objectTypeAbattis,
        radars_on,
        cadenas,
        talkie_interdit,
        brouillage,
        eNbrIcons
    };

private:
    //! @name Constructors/Destructor
    //@{
    GLTool();
    ~GLTool();
    //@}

public:
    //! @name Operations
    //@{
    static void Draw( MT_Rect& viewRect, float rClicksPerPix );

    static void Draw( World& world );
    static void DrawAreas( World& world, unsigned int nMaxLod );
    static void DrawBorders( World& world, unsigned int nMaxLod );
    static void DrawLines( World& world, unsigned int nMaxLod );
    static void DrawDetection( World& world );
    static void DrawNameObjects( World& world );

    static void Draw( AgentManager& agentManager );
    static void Draw( Agent& agent, E_State nState = eNormal );

	static void Draw( Population&              population   , E_State nState = eNormal );
    static void Draw( PopulationConcentration& concentration, E_State nState = eNormal );
    static void Draw( PopulationFlow&          flow         , E_State nState = eNormal );

	static void DrawPopulation( Population& pop, E_State nState );
    static void DrawPath( Agent& agent );
    static void DrawPath( PopulationFlow& flow );
    static void DrawAutomataStatus( Agent& agent, E_State nState );
    static void DrawVisionLines( Agent& agent );
    static void Draw( Team& team );
    static void Draw( Gtia& gtia );

    static void Draw( AgentKnowledge&                   knowledge, E_State nState = eNormal );
    static void Draw( PopulationKnowledge&              knowledge, E_State nState = eNormal );
    static void Draw( PopulationConcentrationKnowledge& knowledge, E_State nState = eNormal );
    static void Draw( PopulationFlowKnowledge&          knowledge, E_State nState = eNormal );

    static void Draw( ObjectManager& manager );
    static void Draw( Object_ABC& object, E_State nState = eNormal );
    static void Draw( ObjectKnowledge& knowledge, E_State nState = eNormal );

    static void Draw( LineManager& lineManager );
    static void Draw( TacticalLine_ABC& line, E_State nState = eNormal, int nSelectedPoint = -1 );

    static void Draw( Meteo_Manager& weatherManager );

    static void Draw( MissionPanel& panel );
    static void Draw( MissionInterface_ABC& missionInterface );

    static void Draw( WeatherPanel& panel );

    static void Draw( ObjectCreationPanel& panel );

    static void Draw( InfoPanel& panel );

    static void Draw( ParamPath& param );
    static void Draw( ParamPathList& param );
    static void Draw( ParamLimaList& param );
    static void Draw( ParamLimits& param );
    static void Draw( ParamPoint& param );
    static void Draw( ParamAgent& param );
    static void Draw( ParamAgentList& param );
    static void Draw( ParamLocation& param, bool bSelected = false );
    static void Draw( ParamLocationList& param );
    static void Draw( ParamObstacle& param, bool bSelected = false );
    static void Draw( ParamObstacleList& param );

    static void Draw( const DefaultMapEventHandler& eventHandler );
    static void Draw( const ShapeEditorMapEventFilter& eventHandler );

    static void DrawLocation( ShapeEditorMapEventFilter::E_LineType nType, const T_PointVector& pointList );
    static void DrawLine    ( CT_PointVector& pointList, MT_Float rArrowSize = 0, bool bDoubleArrow = false );
    static void DrawLine    ( const MT_Vector2D& src, const MT_Vector2D& dest, MT_Float rArrowSize = 0, bool bDoubleArrow = false );
    static void DrawArrow   ( const MT_Vector2D& src, const MT_Vector2D& dest, MT_Float rArrowSize );
    static void DrawPoints  ( CT_PointVector& pointList );
    static void DrawPoint   ( const MT_Vector2D& p );
    static void DrawPolygon ( CT_PointVector& pointList );
    static void DrawRect    ( const MT_Vector2D& point1, const MT_Vector2D& point2, bool bInside );
	static void Draw3DLine  ( MT_Vector3D src, MT_Vector3D dst, float width, GFX_Color color );
	static void Draw3DLines ( T_Point3DVector& points, float width, GFX_Color color );

    static void DrawUnit          ( Agent& agent, E_State nState = eNormal );
    static void DrawVisionCones   ( Agent& agent );
    static void DrawVisionSurfaces( Agent& agent );

    static void DrawLines     ( T_MOSLinePtrVector& lines, float rZ );
    static void DrawCircles   ( T_NodePtrVector&    nodes, MT_Float rRadius );
    static void DrawCircle    ( const MT_Vector2D& center, MT_Float rRadius, bool bFill = false );
    static void DrawArc       ( const MT_Vector2D& center, MT_Float rRadius, MT_Float rAngleStart, MT_Float rAngleEnd , bool arrowed = false);
    static void DrawArc       ( const MT_Vector2D& src, const MT_Vector2D& dst, MT_Float height , bool arrowed = false);
    static void DrawGrid      ( World& world );
    static void DrawCross     ( const MT_Vector2D& vCenter, double rSize, int nLineWidth );
    static void Vertex        ( MT_Float rX, MT_Float rY, MT_Float rZSign = 1 );
	static void DrawCylinder  ( const MT_Vector2D& center, MT_Float rRadius, MT_Float rHeight, GFX_Color& color );
    static void DrawTriangle3D( const MT_Vector3D&, const MT_Vector3D&, const MT_Vector3D&, const GFX_Color& );

    static T_PointVector Refine( const T_PointVector& points );
    //@}

    //! @name Helpers
    //@{
    static bool Overlaps( MT_Rect& a, MT_Rect& b );
    static GFX_Color GetColorForTeam( const Team& team );

    class SimpleMode
    {
    public:
         SimpleMode() { bBackup_ = GLTool::bSimpleMode_; GLTool::bSimpleMode_ = true; };
        ~SimpleMode() { GLTool::bSimpleMode_ = bBackup_; };
    private:
        bool bBackup_;
    };

    class DeltaZ
    {
    public:
        DeltaZ( MT_Float rRatio ) { rBackup_ = GLTool::rDeltaZ_; GLTool::rDeltaZ_ *= rRatio; };
        ~DeltaZ() { GLTool::rDeltaZ_ = rBackup_; };
    private:
        MT_Float rBackup_;
    };
    static void SetAltitudeRatio( MT_Float r );
    //@}

    static int InitializeIcon( const QImage& icon );
    static void DrawIcon( E_Icon nIcon, const MT_Vector2D& vCenter, float rSize = 150.0 );
    static E_Icon IconOfObjectType( E_ObjectType nType );

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

#endif // __GLTool_h_
