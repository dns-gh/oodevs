// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlProxy_h_
#define __GlProxy_h_

#include "View_ABC.h"
#include "clients_kernel/GlTools_ABC.h"


namespace gui
{
    class GlWidget;
    class Gl3dWidget;
    class Layer_ABC;

// =============================================================================
/** @class  GlProxy
    @brief  GlProxy
*/
// Created: AGE 2006-03-29
// =============================================================================
class GlProxy : public View_ABC
              , public kernel::GlTools_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GlProxy();
    virtual ~GlProxy();
    //@}

    //! @name Operations
    //@{
    void RegisterTo( Gl3dWidget* newWidget );
    void RegisterTo( GlWidget* newWidget );
    void ChangeTo( Gl3dWidget* newWidget );
    void ChangeTo( GlWidget* newWidget );

    void Register       ( Layer_ABC& layer );

    virtual void CenterOn( const geometry::Point2f& point );

    virtual bool Select( bool ) const;
    virtual bool ShouldDisplay( const std::string& name ) const;
    virtual bool ShouldDisplay( const std::string& name, bool autoCondition ) const;

    virtual float Pixels() const;
    virtual unsigned short StipplePattern( int factor = 1 ) const;

    virtual void DrawCross      ( const geometry::Point2f& at, float size = -1.f ) const;
    virtual void DrawLine       ( const geometry::Point2f& from, const geometry::Point2f& to ) const;
    virtual void DrawLines      ( const T_PointVector& points ) const;
    virtual void DrawArrow      ( const geometry::Point2f& from, const geometry::Point2f& to, float size = -1.f ) const;
    virtual void DrawArc        ( const geometry::Point2f& center, const geometry::Point2f& p1, const geometry::Point2f& p2 ) const;
    virtual void DrawCurvedArrow( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio = 0.2f, float size = -1.f ) const;
    virtual void DrawCircle     ( const geometry::Point2f& center, float radius = -1.f ) const;
    virtual void DrawDisc       ( const geometry::Point2f& center, float radius = -1.f ) const;
    virtual void DrawLife       ( const geometry::Point2f& center, float height, float factor = 1.f ) const;
    virtual void Print          ( const std::string& message, const geometry::Point2f& where ) const;
    virtual void DrawApp6Symbol ( const std::string& symbol, const geometry::Point2f& where, float factor = 1.f ) const;
    virtual void DrawIcon       ( const char** xpm, const geometry::Point2f& where, float size = -1.f ) const;
    virtual void DrawImage      ( const QImage& image, const geometry::Point2f& where ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GlProxy( const GlProxy& );            //!< Copy constructor
    GlProxy& operator=( const GlProxy& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CheckView() const;
    void CheckTools() const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< Layer_ABC* > T_Layers;
    typedef T_Layers::iterator       IT_Layers;
    //@}

private:
    //! @name Member data
    //@{
    View_ABC*    view_;
    kernel::GlTools_ABC* tools_;
    
    T_Layers layers_;
    //@}
};

}

#endif // __GlProxy_h_
