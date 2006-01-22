// *****************************************************************************
//
// $Created: AGR $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_OrderConduite_Pion_Skeleton.h $
// $Author: Nld $
// $Modtime: 19/10/04 14:30 $
// $Revision: 3 $
// $Workfile: AGR_OrderConduite_Pion_Skeleton.h $
//
// *****************************************************************************

#ifndef __MIL_OrderConduite_$OrderName$_h_
#define __MIL_OrderConduite_$OrderName$_h_

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduite_ABC.h"
#include "TER/TER_Localisation.h"

// =============================================================================
/** @class  MIL_OrderConduite_$OrderName$
    @brief  MIL_OrderConduite_$OrderName$
*/
// Created: AGR 
// =============================================================================
class MIL_OrderConduite_$OrderName$ : public MIL_OrderConduite_ABC
{
    MT_COPYNOTALLOWED( MIL_OrderConduite_$OrderName$ );
public:
    //! @name Constructors/Destructor
    //@{
             MIL_OrderConduite_$OrderName$( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup );
    virtual ~MIL_OrderConduite_$OrderName$();
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
$StaticOrderMember$
};

#endif // __MIL_OrderConduite_$OrderName$_h_
