// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __TacticalGraphics_h_
#define __TacticalGraphics_h_

#include <tools/ElementObserver_ABC.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Location_ABC;
}

namespace gui
{
    class DrawingCategory;
    class DrawingTemplate;
    class DrawingTypes;
    class GLView_ABC;
    class SvgLocationDrawer;

// =============================================================================
/** @class  TacticalGraphics
    @brief  TacticalGraphics
*/
// Created: SBO 2009-05-29
// =============================================================================
class TacticalGraphics : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit TacticalGraphics( const DrawingTypes& drawingTypes );
    virtual ~TacticalGraphics();
    //@}

    //! @name Operations
    //@{
    void SetCurrentColor( float r, float g, float b, float a = 1 );
    void Draw( const std::string& symbol, const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport,
               const GLView_ABC& tools, bool overlined, float pointSize, float zoom );
    //@}

private:
    //! @name Helpers
    //@{
    boost::shared_ptr< SvgLocationDrawer > FindRenderer( const std::string symbol );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, const DrawingTemplate* > T_Templates;
    typedef std::map< std::string, boost::shared_ptr< SvgLocationDrawer > > T_Renderers;
    //@}

private:
    //! @name Member data
    //@{
    T_Templates templates_;
    T_Renderers renderers_;
    QColor color_;
    //@}
};

}

#endif // __TacticalGraphics_h_
