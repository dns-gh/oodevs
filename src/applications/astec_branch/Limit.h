//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Limit.h $
// $Author: Ape $
// $Modtime: 26/07/04 11:10 $
// $Revision: 5 $
// $Workfile: Limit.h $
//
//*****************************************************************************

#ifndef __Limit_h_
#define __Limit_h_

#include "TacticalLine_ABC.h"
#include "IDManager.h"

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
             Limit( Controller& controller, const CoordinateConverter_ABC& converter);
             Limit( Controller& controller, const T_PointVector& pointList, const CoordinateConverter_ABC& converter );
             Limit( Controller& controller, const ASN1T_MsgLimitCreation& asnMsg, const CoordinateConverter_ABC& converter );
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
    Controller& controller_;
    E_NatureLevel  nLevel_;
    //@}
};

#endif // __Limit_h_
