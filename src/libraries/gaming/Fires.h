// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Fires_h_
#define __Fires_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include <tools/Resolver.h>

namespace sword
{
    class StartCrowdFire;
    class StopCrowdFire;
    class StartUnitFire;
    class StopUnitFire;
    class StartUnitFireDetection;
    class StopUnitFireDetection;
}

namespace kernel
{
    class Controller;
}

class Fire_ABC;
class FireFactory;

// =============================================================================
/** @class  Fires
    @brief  Fires
    // $$$$ AGE 2006-11-13: updates !
*/
// Created: AGE 2006-03-10
// =============================================================================
class Fires : public kernel::Extension_ABC
            , public kernel::Updatable_ABC< sword::StartUnitFire >
            , public kernel::Updatable_ABC< sword::StopUnitFire >
            , public kernel::Updatable_ABC< sword::StartCrowdFire >
            , public kernel::Updatable_ABC< sword::StopCrowdFire >
            , public kernel::Updatable_ABC< sword::StartUnitFireDetection >
            , public kernel::Updatable_ABC< sword::StopUnitFireDetection >
            , public tools::Resolver< Fire_ABC >
            , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Fires( kernel::Controller& controller, FireFactory& factory, unsigned long agentId );
    virtual ~Fires();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Fires( const Fires& );            //!< Copy constructor
    Fires& operator=( const Fires& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void CreateFire( const T& message );
    template< typename T >
    void DestroyFire( const T& message );

    virtual void DoUpdate( const sword::StartUnitFire& message );
    virtual void DoUpdate( const sword::StopUnitFire& message );
    virtual void DoUpdate( const sword::StartCrowdFire& message );
    virtual void DoUpdate( const sword::StopCrowdFire& message );
    virtual void DoUpdate( const sword::StartUnitFireDetection& message );
    virtual void DoUpdate( const sword::StopUnitFireDetection& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    FireFactory& factory_;
    unsigned long agentId_;
    //@}
};

#endif // __Fires_h_
