// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectsLayer_h_
#define __ObjectsLayer_h_

#include "graphics/MapLayer_ABC.h"
#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"
#include "ActivationObserver_ABC.h"

class Object;
class Controllers;
class GlTools_ABC;
class ColorStrategy_ABC;
class MapWidget;

// =============================================================================
/** @class  ObjectsLayer
    @brief  ObjectsLayer
    // $$$$ AGE 2006-03-22: Factorisations avec AgentsLayer ?
*/
// Created: AGE 2006-03-22
// =============================================================================
class ObjectsLayer : public MapLayer_ABC
                   , private Observer_ABC
                   , public ElementObserver_ABC< Object >
                   , public ActivationObserver_ABC< Object > 
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, MapWidget& widget );
    virtual ~ObjectsLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectsLayer( const ObjectsLayer& );            //!< Copy constructor
    ObjectsLayer& operator=( const ObjectsLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Object& );
    virtual void NotifyUpdated( const Object& );
    virtual void NotifyDeleted( const Object& );
    virtual void NotifyActivated( const Object& );
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    bool IsInSelection( const Object& object, const geometry::Point2f& point ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< const Object* > T_Objects;
    typedef T_Objects::iterator         IT_Objects;
    typedef T_Objects::const_iterator  CIT_Objects;
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    const GlTools_ABC& tools_;
    ColorStrategy_ABC& strategy_;
    MapWidget& widget_;

    T_Objects objects_;
    unsigned selected_;
    //@}
};

#endif // __ObjectsLayer_h_
