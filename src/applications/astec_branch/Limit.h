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
    MT_COPYNOTALLOWED( Limit );

public:
    static IDManager idManager_;

public:
             Limit();
    explicit Limit( const T_PointVector& pointList );
             Limit( const ASN1T_MsgLimitCreation& asnMsg, const CoordinateConverter& converter );
    virtual ~Limit();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    TacticalLine_ABC::E_LineType GetLineType() const;

    bool UpdateToSim();
    //@}

    //! @name In/Out
    //@{
    void Read( MT_InputArchive_ABC& archive );
    void Write( MT_OutputArchive_ABC& archive ) const;
    //@}
};

#   include "Limit.inl"

#endif // __Limit_h_
