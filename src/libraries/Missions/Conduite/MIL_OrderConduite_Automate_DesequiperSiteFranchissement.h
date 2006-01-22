// *****************************************************************************
//
// $Created: AGR $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_OrderConduite_Automate_Skeleton.h $
// $Author: Nld $
// $Modtime: 19/10/04 14:30 $
// $Revision: 3 $
// $Workfile: AGR_OrderConduite_Automate_Skeleton.h $
//
// *****************************************************************************

#ifndef __MIL_OrderConduite_Automate_DesequiperSiteFranchissement_h_
#define __MIL_OrderConduite_Automate_DesequiperSiteFranchissement_h_

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduite_ABC.h"
#include "TER/TER_Localisation.h"

// =============================================================================
/** @class  MIL_OrderConduite_Automate_DesequiperSiteFranchissement
    @brief  MIL_OrderConduite_Automate_DesequiperSiteFranchissement
*/
// Created: AGR 
// =============================================================================
class MIL_OrderConduite_Automate_DesequiperSiteFranchissement : public MIL_OrderConduite_ABC
{
    MT_COPYNOTALLOWED( MIL_OrderConduite_Automate_DesequiperSiteFranchissement );
public:
             MIL_OrderConduite_Automate_DesequiperSiteFranchissement( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup );
    virtual ~MIL_OrderConduite_Automate_DesequiperSiteFranchissement();
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
    static int nDIASiteFranchissementIdx_;

};

#endif // __MIL_OrderConduite_Automate_DesequiperSiteFranchissement_h_
