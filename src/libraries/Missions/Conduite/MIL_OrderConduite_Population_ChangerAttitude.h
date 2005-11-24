// *****************************************************************************
//
// $Created: AGR $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_OrderConduite_Population_Skeleton.h $
// $Author: Nld $
// $Modtime: 19/10/04 14:30 $
// $Revision: 3 $
// $Workfile: AGR_OrderConduite_Population_Skeleton.h $
//
// *****************************************************************************

#ifndef __MIL_OrderConduite_Population_ChangerAttitude_h_
#define __MIL_OrderConduite_Population_ChangerAttitude_h_

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduite_ABC.h"
#include "TER/TER_Localisation.h"

// =============================================================================
/** @class  MIL_OrderConduite_Population_ChangerAttitude
    @brief  MIL_OrderConduite_Population_ChangerAttitude
*/
// Created: AGR 
// =============================================================================
class MIL_OrderConduite_Population_ChangerAttitude : public MIL_OrderConduite_ABC
{
    MT_COPYNOTALLOWED( MIL_OrderConduite_Population_ChangerAttitude );
public:
    //! @name Constructors/Destructor
    //@{
             MIL_OrderConduite_Population_ChangerAttitude( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup );
    virtual ~MIL_OrderConduite_Population_ChangerAttitude();
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
    static int nDIAOrderConduitePopulationChangerAttitudeIdx_;

};

#endif // __MIL_OrderConduite_Population_ChangerAttitude_h_
