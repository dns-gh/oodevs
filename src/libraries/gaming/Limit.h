// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Limit_h_
#define __Limit_h_

#include "TacticalLine_ABC.h"

namespace kernel
{
    class Controller;
}

// =============================================================================
/** @class  Limit
    @brief  Represents a limit.
*/
// Created: APE 2004-03-12
// =============================================================================
class Limit : public TacticalLine_ABC
{

public:
    //! @name Constructor/Destructor
    //@{
             Limit( kernel::Controller& controller, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter );
             Limit( kernel::Controller& controller, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter, const MsgsMessengerToClient::MsgLimitCreation& message );
    virtual ~Limit();
    //@}

    //! @name Operations
    //@{
    virtual void Select( kernel::ActionController& actions ) const;
    virtual void ContextMenu( kernel::ActionController& actions, const QPoint& point ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    virtual bool IsLimit() const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Limit( const Limit& );
    Limit& operator=( const Limit& );
    //@}

    //! @name Helpers
    //@{
    virtual void UpdateToSim( E_State state );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

#endif // __Limit_h_
