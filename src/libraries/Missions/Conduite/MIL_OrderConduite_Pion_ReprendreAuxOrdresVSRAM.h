// *****************************************************************************
//
// $Created: AGR 2005-08-19 - 11:30:56
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_OrderConduite_Pion_Skeleton.h $
// $Author: Nld $
// $Modtime: 19/10/04 14:30 $
// $Revision: 3 $
// $Workfile: AGR_OrderConduite_Pion_Skeleton.h $
//
// *****************************************************************************

#ifndef __MIL_OrderConduite_Pion_ReprendreAuxOrdresVSRAM_h_
#define __MIL_OrderConduite_Pion_ReprendreAuxOrdresVSRAM_h_

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduite_ABC.h"
#include "TER/TER_Localisation.h"

// =============================================================================
/** @class  MIL_OrderConduite_Pion_ReprendreAuxOrdresVSRAM
    @brief  MIL_OrderConduite_Pion_ReprendreAuxOrdresVSRAM
*/
// Created: AGR 2005-08-19 - 11:30:56
// =============================================================================
class MIL_OrderConduite_Pion_ReprendreAuxOrdresVSRAM : public MIL_OrderConduite_ABC
{
    MT_COPYNOTALLOWED( MIL_OrderConduite_Pion_ReprendreAuxOrdresVSRAM );
public:
    //! @name Constructors/Destructor
    //@{
             MIL_OrderConduite_Pion_ReprendreAuxOrdresVSRAM( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type );
    virtual ~MIL_OrderConduite_Pion_ReprendreAuxOrdresVSRAM();
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
    static int nDIANbrAmbulancesIdx_;

};

#endif // __MIL_OrderConduite_Pion_ReprendreAuxOrdresVSRAM_h_
