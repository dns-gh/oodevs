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
#include "astec_kernel/IDManager.h"

class CoordinateConverter_ABC;
class Controller;

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
             Limit( Controller& controller, Publisher_ABC& publisher, const CoordinateConverter_ABC& converter);
             Limit( Controller& controller, Publisher_ABC& publisher, const T_PointVector& pointList, const CoordinateConverter_ABC& converter );
             Limit( Controller& controller, Publisher_ABC& publisher, const ASN1T_MsgLimitCreation& asnMsg, const CoordinateConverter_ABC& converter );
    virtual ~Limit();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const GlTools_ABC& tools ) const;
    virtual void Select( ActionController& actions ) const;
    virtual void ContextMenu( ActionController& actions, const QPoint& point ) const;
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
    static IDManager idManager_;

private:
    Controller& controller_;
    E_NatureLevel  nLevel_;
    //@}
};

#endif // __Limit_h_
