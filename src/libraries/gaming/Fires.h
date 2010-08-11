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


#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "protocol/Protocol.h"
#include "tools/Resolver.h"

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
            , public kernel::Updatable_ABC< MsgsSimToClient::MsgStartUnitFire >
            , public kernel::Updatable_ABC< MsgsSimToClient::MsgStopUnitFire >
            , public kernel::Updatable_ABC< MsgsSimToClient::MsgStartPopulationFire >
            , public kernel::Updatable_ABC< MsgsSimToClient::MsgStopPopulationFire >
            , public tools::Resolver< Fire_ABC >
            , public kernel::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Fires( kernel::Controller& controller, FireFactory& factory );
    virtual ~Fires();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
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
    virtual void DoUpdate( const MsgsSimToClient::MsgStartUnitFire& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgStopUnitFire& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgStartPopulationFire& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgStopPopulationFire& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    FireFactory& factory_;
    //@}
};

#endif // __Fires_h_
