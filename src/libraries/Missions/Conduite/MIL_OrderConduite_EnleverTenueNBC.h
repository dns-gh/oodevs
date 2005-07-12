// *****************************************************************************
//
// $Created: AGR 2005-6-28 - 14:3:4
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_OrderConduite_Pion_Skeleton.h $
// $Author: Nld $
// $Modtime: 19/10/04 14:30 $
// $Revision: 3 $
// $Workfile: AGR_OrderConduite_Pion_Skeleton.h $
//
// *****************************************************************************

#ifndef __MIL_OrderConduite_EnleverTenueNBC_h_
#define __MIL_OrderConduite_EnleverTenueNBC_h_

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduite_ABC.h"
#include "TER/TER_Localisation.h"

// =============================================================================
/** @class  MIL_OrderConduite_EnleverTenueNBC
    @brief  MIL_OrderConduite_EnleverTenueNBC
*/
// Created: AGR 2005-6-28 - 14:3:4
// =============================================================================
class MIL_OrderConduite_EnleverTenueNBC : public MIL_OrderConduite_ABC
{
    MT_COPYNOTALLOWED( MIL_OrderConduite_EnleverTenueNBC );
public:
    //! @name Constructors/Destructor
    //@{
             MIL_OrderConduite_EnleverTenueNBC( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type );
    virtual ~MIL_OrderConduite_EnleverTenueNBC();
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

};

#endif // __MIL_OrderConduite_EnleverTenueNBC_h_
