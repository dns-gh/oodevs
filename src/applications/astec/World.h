//*****************************************************************************
//
// $Created: FBD 02-11-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/World.h $
// $Author: Age $
// $Modtime: 16/05/05 16:40 $
// $Revision: 6 $
// $Workfile: World.h $
//
//*****************************************************************************

#ifndef __World_h_
#define __World_h_

#include "Types.h"
#include "LodGraphicSetup.h"
#include "MT_Tools/MT_Rect.h"
#include "geocoord/MGRS.h"
#include "geocoord/PlanarCartesian.h"
#include "geocoord/Geodetic.h"
#include "graphics/GraphicManager_ABC.h"
#include "graphics/GraphicShape.h"

class DrawDetection;
class TesselatedShape;

// =============================================================================
// Created: APE 2004-07-19
// =============================================================================
class World : private GraphicManager_ABC
{
    MT_COPYNOTALLOWED( World )   
    friend class GLTool;

public:
    //! @name Constructor / Destructor
    //@{
    explicit World( const std::string& strArchive );
    virtual ~World();
    //@}

    //! @name Initialization
    //@{
    void Initialize();
    //@}

    //-------------------------------------------------------------------------
    /** @name Conversion */
    //-------------------------------------------------------------------------
    //@{
    void SimToMosMgrsCoord( const MT_Vector2D& vPos, std::string& sMgrs );
    void MosToSimMgrsCoord( const std::string& sMgrs, MT_Vector2D& vPos );
    void MosToSimMgrsCoord( const char sMgrs[15], MT_Vector2D& vPos );
    void MgrsToGeoCoord   ( const std::string& sMgrs, MT_Vector2D& vPos );
    void GeoToMgrsCoord   ( const MT_Vector2D& vPos, std::string& sMgrs );
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const MT_Rect& GetRect() const;
    std::string GetTerrainDirectory() const;
    //@}
   
private:
    //! @name Initialization
    //@{
    void Read( const std::string& strArchive, bool bWorldOnly = false );
    void ReadWorld( const std::string& strArchive );
    void ReadGraphics( const std::string& strArchive );
    void ReadGraphicFile( const std::string& strName );
    void ReadDetection( const std::string& strName );

    virtual void AddShape( GraphicShape& shape );
    virtual bool ShouldUseList( const std::string& filename );
    //@}
    
    //! @name Types
    //@{
    typedef std::vector< GraphicShape* > T_Shapes;
    typedef T_Shapes::const_iterator   CIT_Shapes;
    //@}
    
private:
    std::string    strArchiveName_;
    std::string    strTerrainDirectory_;
    bool           isInitialized_;
    MT_Rect        extent_;
    MT_Vector2D    translation_;
    LodGraphicSetup setup_;

    T_Shapes       lodshapes_[ 3 ];

    DrawDetection* pDetection_;

            geocoord::PlanarCartesian::Parameters parameters_;
    mutable geocoord::PlanarCartesian             planar_;
    mutable geocoord::MGRS                        mgrs_;
    mutable geocoord::Geodetic                    geodetic_;
};

#endif // __World_h_