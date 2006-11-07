// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Lima_h_
#define __Lima_h_

#include "TacticalLine_ABC.h"
#include "clients_kernel/IDManager.h"
#include "Types.h"

namespace kernel
{
    class Controller;
}

// =============================================================================
/** @class  Lima
    @brief  Represents a lima.
*/
// Created: APE 2004-03-12
// =============================================================================
class Lima : public TacticalLine_ABC
{

public:
    //! @name Constructor/Destructor
    //@{
//             Lima( kernel::Controller& controller, Publisher_ABC& publisher );
             Lima( kernel::Controller& controller, Publisher_ABC& publisher, E_FuncLimaType nFuncType );
             Lima( kernel::Controller& controller, Publisher_ABC& publisher, const ASN1T_MsgLimaCreation& asnMsg );
             Lima( kernel::Controller& controller, Publisher_ABC& publisher, xml::xistream& xis );
    virtual ~Lima();
    //@}

    //! @name Operations
    //@{
    virtual void Select( kernel::ActionController& actions ) const;
    virtual void ContextMenu( kernel::ActionController& actions, const QPoint& point ) const;
    virtual void Activate( kernel::ActionController& controller ) const;

    virtual void Serialize( xml::xostream& xos ) const;
    virtual bool IsLimit() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Lima( const Lima& );
    Lima& operator=( const Lima& );
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
    E_FuncLimaType   nFuncType_;
    //@}
};

#endif // __Lima_h_
