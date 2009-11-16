// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanLayer_h_
#define __UrbanLayer_h_

#include "tools/Observer_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include "Layer_ABC.h"


namespace urban
{
    class Drawer_ABC;
}

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class Location_ABC;
    class Viewport_ABC;
}

namespace gui
{
    class TerrainObjectProxy;
    class View_ABC;

// =============================================================================
/** @class  UrbanLayer
@brief  UrbanLayer
*/
// Created: SLG 2006-03-23
// =============================================================================
class UrbanLayer : public Layer_ABC
                 , public tools::Observer_ABC
                 , public tools::SelectionObserver< TerrainObjectProxy >
                 , public tools::ElementObserver_ABC< TerrainObjectProxy >
{
public:
    //! @name Constructors/Destructor
    //@{    
    UrbanLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools );
    virtual ~UrbanLayer();
    //@}    

    //! @name Operations
    //@{
    virtual void Paint( kernel::Viewport_ABC& viewport );
    virtual void Reset2d();
    void NotifyDone(); // Pas top
    //@}

protected:
    //! @name Events
    //@{    
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMouseMove( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleKeyPress( QKeyEvent* input );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated      ( const TerrainObjectProxy& );
    virtual void NotifyDeleted      ( const TerrainObjectProxy& );
    virtual void NotifyActivated    ( const TerrainObjectProxy& );
    virtual void NotifySelected     ( const TerrainObjectProxy* );

    //@}

private:
    //! @name Copy/Assignement
    //@{
    UrbanLayer( const UrbanLayer& );            //!< Copy constructor
    UrbanLayer& operator=( const UrbanLayer& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const TerrainObjectProxy >  T_TerrainObjects;
    typedef T_TerrainObjects::iterator                      IT_TerrainObjects;
    typedef T_TerrainObjects::const_iterator                CIT_TerrainObjects;
    //@}

private:
    //! @name Member data
    //@{

    kernel::Controllers& controllers_;
    const kernel::GlTools_ABC& tools_;
    urban::Drawer_ABC* urbanDrawer_;
    kernel::Location_ABC* selectionArea_;
    geometry::Point2f lastPoint_;
    T_TerrainObjects objects_;
    const TerrainObjectProxy* selectedObject_;
    unsigned tooltiped_;
    bool selectionMode_;
    //@}
};

}
#endif // __UrbanLayer_h_