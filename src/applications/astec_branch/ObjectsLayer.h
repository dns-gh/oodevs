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

// =============================================================================
/** @class  ObjectsLayer
    @brief  ObjectsLayer
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
             ObjectsLayer();
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
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __ObjectsLayer_h_
