// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Limit_h_
#define __Limit_h_

#include "TacticalLine_ABC.h"
#include "clients_kernel/IDManager.h"

namespace kernel
{
    class CoordinateConverter_ABC;
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
             Limit( kernel::Controller& controller, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter);
             Limit( kernel::Controller& controller, Publisher_ABC& publisher, const T_PointVector& pointList, const kernel::CoordinateConverter_ABC& converter );
             Limit( kernel::Controller& controller, Publisher_ABC& publisher, const ASN1T_MsgLimitCreation& asnMsg, const kernel::CoordinateConverter_ABC& converter );
    virtual ~Limit();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const kernel::GlTools_ABC& tools ) const;
    virtual void Select( kernel::ActionController& actions ) const;
    virtual void ContextMenu( kernel::ActionController& actions, const QPoint& point ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Limit( const Limit& );
    Limit& operator=( const Limit& );
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void FillAndSend();
    virtual void UpdateToSim( E_State state );
    //@}

public:
    //! @name Member data
    //@{
    static kernel::IDManager idManager_;

private:
    kernel::Controller& controller_;
    E_NatureLevel  nLevel_;
    //@}
};

#endif // __Limit_h_
