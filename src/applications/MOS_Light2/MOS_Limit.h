//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Limit.h $
// $Author: Ape $
// $Modtime: 26/07/04 11:10 $
// $Revision: 5 $
// $Workfile: MOS_Limit.h $
//
//*****************************************************************************

#ifndef __MOS_Limit_h_
#define __MOS_Limit_h_

#include "MOS_TacticalLine_ABC.h"
#include "MOS_IDManager.h"
#include "MOS_ASN_Types.h"


// =============================================================================
/** @class  MOS_Limit
    @brief  Represents a limit.
*/
// Created: APE 2004-03-12
// =============================================================================
class MOS_Limit : public MOS_TacticalLine_ABC
{
    MT_COPYNOTALLOWED( MOS_Limit );

public:
    static MOS_IDManager idManager_;

public:
     MOS_Limit();
     MOS_Limit( T_PointVector pointList );
     MOS_Limit( const ASN1T_MsgLimitCreation& asnMsg );
    ~MOS_Limit();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_TacticalLine_ABC::E_LineType GetLineType() const;

    bool UpdateToSim();
    //@}

    //! @name In/Out
    //@{
    void Read( MT_InputArchive_ABC& archive );
    void Write( MT_OutputArchive_ABC& archive ) const;
    //@}
};

#   include "MOS_Limit.inl"

#endif // __MOS_Limit_h_
