//*****************************************************************************
//
// $Created: FBD 02-11-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_World.h $
// $Author: Age $
// $Modtime: 28/02/05 16:13 $
// $Revision: 11 $
// $Workfile: MOS_World.h $
//
//*****************************************************************************

#ifndef __MOS_World_h_
#define __MOS_World_h_

#include "MOS_Types.h"
#include "MT_Tools/MT_Rect.h"
#include "geocoord/MGRS.h"
#include "geocoord/PlanarCartesian.h"
#include "geometry/Types.h"

class MOS_GLWidget;
class MT_XXmlInputArchive;
class GraphicShape;
class DrawDetection;
    
//*****************************************************************************
// Created: FBD 02-11-21
//*****************************************************************************
class MOS_World
{
    MT_COPYNOTALLOWED( MOS_World )   
public:
    explicit MOS_World( const std::string& strArchiveName );
    virtual ~MOS_World();

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
    /** @name Draw */
    //-------------------------------------------------------------------------
    //@{
    void Draw           ( const MT_Rect& viewport ) const;
    void DrawNameObjects( const MT_Rect& viewport, QGLWidget& widget ) const;
    //@}

    //! @name Accessors
    //@{
    const MT_Rect& GetRect() const { return extent_; };
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Initialize */
    //-------------------------------------------------------------------------
    //@{
    void Read( const std::string& strArchive );
    void ReadWorld( const std::string& strArchive );
    void ReadGraphics( const std::string& strArchive );
    void ReadGraphicFile( const std::string& strName );
    void ReadDetection( const std::string& strName );
    void DrawAreas( unsigned int nLod, const geometry::Rectangle2f& extent ) const;
    void DrawBorders( unsigned int nLod, const geometry::Rectangle2f& extent ) const;
    void DrawLines( unsigned int nLod, const geometry::Rectangle2f& extent ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< GraphicShape* > T_Shapes;
    typedef T_Shapes::const_iterator   CIT_Shapes;
    //@}
    
private:
    MT_Rect        extent_;
    MT_Vector2D    translation_;

    T_Shapes       lodshapes_[ 3 ];
    DrawDetection* pDetection_;

            geocoord::PlanarCartesian::Parameters parameters_;
    mutable geocoord::PlanarCartesian             planar_;
    mutable geocoord::MGRS                        mgrs_;
};

#include "MOS_World.inl"


#endif // __MOS_World_h_