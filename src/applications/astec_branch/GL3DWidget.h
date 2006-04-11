// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gl3dWidget_h_
#define __Gl3dWidget_h_

#include "graphics/Widget3D.h"
#include "WorldParameters.h"
#include "GlToolsBase.h"
#include "View_ABC.h"
#include "SetGlOptions.h"

class Layer_ABC;
class ElevationMap;
class Controllers;
class GlFont;

// =============================================================================
/** @class  Gl3dWidget
    @brief  Gl3dWidget
*/
// Created: AGE 2006-03-28
// =============================================================================
class Gl3dWidget : private WorldParameters, private SetGlOptions, public Widget3D, public GlToolsBase, public View_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Gl3dWidget( QWidget* pParent, Controllers& controllers, const std::string& scipioXml, ElevationMap& elevation );
    virtual ~Gl3dWidget();
    //@}

    //! @name Operations
    //@{
    void Register( Layer_ABC& layer );
    void SetDefaultLayer( Layer_ABC& layer );

    virtual float Pixels() const;
    virtual unsigned short StipplePattern( int factor = 1 ) const;

    virtual void DrawCross      ( const geometry::Point2f& at, float size = -1.f ) const;
    virtual void DrawLine       ( const geometry::Point2f& from, const geometry::Point2f& to ) const;
    virtual void DrawLines      ( const T_PointVector& points ) const;
    virtual void DrawArrow      ( const geometry::Point2f& from, const geometry::Point2f& to, float size = -1.f ) const;
    virtual void DrawCurvedArrow( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio = 0.2f, float size = -1.f ) const; // $$$$ AGE 2006-03-23: 0 <= curveRatio <= 1
    virtual void DrawCircle     ( const geometry::Point2f& center, float radius = -1.f ) const;
    virtual void DrawDisc       ( const geometry::Point2f& center, float radius = -1.f ) const;
    virtual void DrawRectangle  ( const geometry::Point2f& center, float height, float factor = 1.f ) const;
    virtual void Print          ( const std::string& message, const geometry::Point2f& where ) const;
    virtual void DrawApp6Symbol ( const std::string& symbol, const geometry::Point2f& where, float factor = 1.f ) const;
    virtual void DrawIcon       ( const char** xpm, const geometry::Point2f& where, float size = -1.f ) const;

    virtual void CenterOn( const geometry::Point2f& point );
    virtual void CenterView();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Gl3dWidget( const Gl3dWidget& );            //!< Copy constructor
    Gl3dWidget& operator=( const Gl3dWidget& ); //!< Assignement operator
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void mousePressEvent( QMouseEvent* );
    virtual void mouseMoveEvent ( QMouseEvent* );
    virtual void mouseReleaseEvent     ( QMouseEvent* );
    virtual void mouseDoubleClickEvent ( QMouseEvent* );
    virtual void keyPressEvent( QKeyEvent* );
    virtual void Paint( const ViewFrustum& view );
    virtual void initializeGL();
    float ElevationAt( const geometry::Point2f& point ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< Layer_ABC* >  T_Layers;
    typedef T_Layers::iterator        IT_Layers;
    typedef T_Layers::const_iterator CIT_Layers;
    //@}

private:
    //! @name Member data
    //@{
    ElevationMap& elevation_;
    T_Layers layers_;
    IT_Layers last_;
    Layer_ABC* default_;
    float zRatio_;
    unsigned int frame_;
    float pixels_;
    //@}
};

#endif // __Gl3dWidget_h_
