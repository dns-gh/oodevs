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

class CoordinateConverter;

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
    explicit Limit( const CoordinateConverter& converter);
             Limit( const T_PointVector& pointList, const CoordinateConverter& converter );
             Limit( const ASN1T_MsgLimitCreation& asnMsg, const CoordinateConverter& converter );
    virtual ~Limit();
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
    E_NatureLevel  nLevel_;
    //@}
};

#endif // __Limit_h_
