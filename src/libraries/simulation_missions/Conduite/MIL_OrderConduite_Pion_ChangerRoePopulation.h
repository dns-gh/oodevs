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

#ifndef __MIL_OrderConduite_Pion_ChangerRoePopulation_h_
#define __MIL_OrderConduite_Pion_ChangerRoePopulation_h_

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduite_ABC.h"
#include "TER/TER_Localisation.h"

// =============================================================================
/** @class  MIL_OrderConduite_Pion_ChangerRoePopulation
    @brief  MIL_OrderConduite_Pion_ChangerRoePopulation
*/
// Created: AGR 
// =============================================================================
class MIL_OrderConduite_Pion_ChangerRoePopulation : public MIL_OrderConduite_ABC
{
    MT_COPYNOTALLOWED( MIL_OrderConduite_Pion_ChangerRoePopulation );
public:
    //! @name Constructors/Destructor
    //@{
             MIL_OrderConduite_Pion_ChangerRoePopulation( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type );
    virtual ~MIL_OrderConduite_Pion_ChangerRoePopulation();
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
    static int nDIAOrderConduitePionChangerRoePopulationIdx_;

};

#endif // __MIL_OrderConduite_Pion_ChangerRoePopulation_h_
