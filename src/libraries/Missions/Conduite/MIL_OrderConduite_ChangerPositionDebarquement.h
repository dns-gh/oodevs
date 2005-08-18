// *****************************************************************************
//
// $Created: AGR 2005-08-17 - 16:30:44
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_OrderConduite_Pion_Skeleton.h $
// $Author: Nld $
// $Modtime: 19/10/04 14:30 $
// $Revision: 3 $
// $Workfile: AGR_OrderConduite_Pion_Skeleton.h $
//
// *****************************************************************************

#ifndef __MIL_OrderConduite_ChangerPositionDebarquement_h_
#define __MIL_OrderConduite_ChangerPositionDebarquement_h_

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduite_ABC.h"
#include "TER/TER_Localisation.h"

// =============================================================================
/** @class  MIL_OrderConduite_ChangerPositionDebarquement
    @brief  MIL_OrderConduite_ChangerPositionDebarquement
*/
// Created: AGR 2005-08-17 - 16:30:44
// =============================================================================
class MIL_OrderConduite_ChangerPositionDebarquement : public MIL_OrderConduite_ABC
{
    MT_COPYNOTALLOWED( MIL_OrderConduite_ChangerPositionDebarquement );
public:
    //! @name Constructors/Destructor
    //@{
             MIL_OrderConduite_ChangerPositionDebarquement( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type );
    virtual ~MIL_OrderConduite_ChangerPositionDebarquement();
    //@}

    //-------------------------------------------------------------------------
    /** @name Initialization */
    //-------------------------------------------------------------------------
    //@{
    /*final*/ ASN1T_EnumOrderErrorCode Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg );
    /*final*/ void                     Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx );

    static void InitializeDIA( const MIL_OrderConduiteType& type );
    //@}

private:
    MT_Vector2D orderConduiteChangerPositionDebarquement_;

private:
    static int nDIAOrderConduiteChangerPositionDebarquementIdx_;

};

#endif // __MIL_OrderConduite_ChangerPositionDebarquement_h_
