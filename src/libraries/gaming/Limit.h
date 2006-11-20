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
             Limit( kernel::Controller& controller, Publisher_ABC& publisher );
             Limit( kernel::Controller& controller, Publisher_ABC& publisher, const ASN1T_MsgLimitCreation& asnMsg );
    virtual ~Limit();
    //@}

    //! @name Operations
    //@{
    virtual void Select( kernel::ActionController& actions ) const;
    virtual void ContextMenu( kernel::ActionController& actions, const QPoint& point ) const;
    virtual void Activate( kernel::ActionController& controller ) const;

    virtual void Serialize( xml::xostream& xos ) const;
    void CopyTo( ASN1T_Line& destination ) const;
    virtual bool IsLimit() const; // $$$$ SBO 2006-11-14: only for display...
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

public:
    //! @name Member data
    //@{
    static kernel::IDManager idManager_;

private:
    kernel::Controller& controller_;
    //@}
};

#endif // __Limit_h_
