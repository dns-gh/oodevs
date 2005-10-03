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

#ifndef __MIL_OrderConduite_Pion_TransfererVSRAM_h_
#define __MIL_OrderConduite_Pion_TransfererVSRAM_h_

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduite_ABC.h"
#include "TER/TER_Localisation.h"

// =============================================================================
/** @class  MIL_OrderConduite_Pion_TransfererVSRAM
    @brief  MIL_OrderConduite_Pion_TransfererVSRAM
*/
// Created: AGR 
// =============================================================================
class MIL_OrderConduite_Pion_TransfererVSRAM : public MIL_OrderConduite_ABC
{
    MT_COPYNOTALLOWED( MIL_OrderConduite_Pion_TransfererVSRAM );
public:
    //! @name Constructors/Destructor
    //@{
             MIL_OrderConduite_Pion_TransfererVSRAM( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type );
    virtual ~MIL_OrderConduite_Pion_TransfererVSRAM();
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
    static int nDIAPionRenforceIdx_;
    static int nDIAPionARenforcerIdx_;
    static int nDIANbrAmbulancesIdx_;

};

#endif // __MIL_OrderConduite_Pion_TransfererVSRAM_h_
