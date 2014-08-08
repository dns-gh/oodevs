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

namespace kernel
{
    class Controllers;
    class Location_ABC;
}

namespace gui
{
    class DrawingCategory;
    class DrawingTemplate;
    class GlTools_ABC;
    class SvgLocationDrawer;
}

namespace gui
{
// =============================================================================
/** @class  TacticalGraphics
    @brief  TacticalGraphics
*/
// Created: SBO 2009-05-29
// =============================================================================
class TacticalGraphics : public tools::Observer_ABC
                       , public tools::ElementObserver_ABC< DrawingCategory >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit TacticalGraphics( kernel::Controllers& controllers );
    virtual ~TacticalGraphics();
    //@}

    //! @name Operations
    //@{
    void SetCurrentColor( float r, float g, float b, float a = 1 );
    void Draw( const std::string& symbol, const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport,
               const GlTools_ABC& tools, bool overlined, float pointSize, float zoom );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalGraphics( const TacticalGraphics& );            //!< Copy constructor
    TacticalGraphics& operator=( const TacticalGraphics& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const DrawingCategory& category );
    virtual void NotifyDeleted( const DrawingCategory& category );
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
    kernel::Controllers& controllers_;
    T_Templates templates_;
    T_Renderers renderers_;
    QColor color_;
    //@}
};

}

#endif // __TacticalGraphics_h_
