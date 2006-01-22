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

#ifndef __MIL_OrderConduite_PasserEnSilenceRadio_h_
#define __MIL_OrderConduite_PasserEnSilenceRadio_h_

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduite_ABC.h"
#include "TER/TER_Localisation.h"

// =============================================================================
/** @class  MIL_OrderConduite_PasserEnSilenceRadio
    @brief  MIL_OrderConduite_PasserEnSilenceRadio
*/
// Created: AGR 
// =============================================================================
class MIL_OrderConduite_PasserEnSilenceRadio : public MIL_OrderConduite_ABC
{
    MT_COPYNOTALLOWED( MIL_OrderConduite_PasserEnSilenceRadio );
public:
    //! @name Constructors/Destructor
    //@{
             MIL_OrderConduite_PasserEnSilenceRadio( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup );
    virtual ~MIL_OrderConduite_PasserEnSilenceRadio();
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

};

#endif // __MIL_OrderConduite_PasserEnSilenceRadio_h_
