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

#ifndef __MIL_OrderConduite_Automate_TC2_GererMaterielAvantDeplacement_h_
#define __MIL_OrderConduite_Automate_TC2_GererMaterielAvantDeplacement_h_

#include "simulation_kernel/Entities/Orders/Conduite/MIL_OrderConduite_ABC.h"
#include "simulation_terrain/TER_Localisation.h"

// =============================================================================
/** @class  MIL_OrderConduite_Automate_TC2_GererMaterielAvantDeplacement
    @brief  MIL_OrderConduite_Automate_TC2_GererMaterielAvantDeplacement
*/
// Created: AGR 
// =============================================================================
class MIL_OrderConduite_Automate_TC2_GererMaterielAvantDeplacement : public MIL_OrderConduite_ABC
{
    MT_COPYNOTALLOWED( MIL_OrderConduite_Automate_TC2_GererMaterielAvantDeplacement );
public:
             MIL_OrderConduite_Automate_TC2_GererMaterielAvantDeplacement( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup );
    virtual ~MIL_OrderConduite_Automate_TC2_GererMaterielAvantDeplacement();
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
    static int nDIAOrderConduiteAutomateTc2GererMaterielAvantDeplacementIdx_;

};

#endif // __MIL_OrderConduite_Automate_TC2_GererMaterielAvantDeplacement_h_
