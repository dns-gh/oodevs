// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RasterTextureSet_h_
#define __RasterTextureSet_h_

#include "graphics/TextureSet.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/WorldParameters.h"

namespace kernel
{
    class ModelLoaded;
    class Controller;
}

namespace gui
{

// =============================================================================
/** @class  RasterTextureSet
    @brief  RasterTextureSet
*/
// Created: AGE 2007-01-04
// =============================================================================
class RasterTextureSet : public TextureSet
                       , public kernel::Observer_ABC
                       , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
                       , private kernel::WorldParameters
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit RasterTextureSet( kernel::Controller& controller );
    virtual ~RasterTextureSet();
    //@}

    //! @name Operations
    //@{
    virtual void Accept( TextureVisitor_ABC& visitor, unsigned level, const geometry::Rectangle2f& extent );
    virtual void NotifyUpdated( const kernel::ModelLoaded& modelLoaded );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    RasterTextureSet( const RasterTextureSet& );            //!< Copy constructor
    RasterTextureSet& operator=( const RasterTextureSet& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    bool loaded_;
    //@}
};

}

#endif // __RasterTextureSet_h_
