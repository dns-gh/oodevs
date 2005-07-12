// *****************************************************************************
//
// $Created: AGR 2005-5-9 - 11:49:5
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_OrderConduite_Automate_Skeleton.h $
// $Author: Nld $
// $Modtime: 19/10/04 14:30 $
// $Revision: 3 $
// $Workfile: AGR_OrderConduite_Automate_Skeleton.h $
//
// *****************************************************************************

#ifndef __MIL_OrderConduite_Automate_TransfererRenfort_h_
#define __MIL_OrderConduite_Automate_TransfererRenfort_h_

#include "Entities/Orders/Conduite/MIL_OrderConduite_ABC.h"
#include "TER_Localisation.h"

// =============================================================================
/** @class  MIL_OrderConduite_Automate_TransfererRenfort
    @brief  MIL_OrderConduite_Automate_TransfererRenfort
*/
// Created: AGR 2005-5-9 - 11:49:5
// =============================================================================
class MIL_OrderConduite_Automate_TransfererRenfort : public MIL_OrderConduite_ABC
{
    MT_COPYNOTALLOWED( MIL_OrderConduite_Automate_TransfererRenfort );
public:
             MIL_OrderConduite_Automate_TransfererRenfort( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type );
    virtual ~MIL_OrderConduite_Automate_TransfererRenfort();
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

private:
    static int nDIAPionATransfererIdx_;
    static int nDIAAutomateARenforcerIdx_;

};

#endif // __MIL_OrderConduite_Automate_TransfererRenfort_h_
