// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Objects/PHY_RoleAction_Objects_DataComputer.h $
// $Author: Nld $
// $Modtime: 11/05/05 18:15 $
// $Revision: 4 $
// $Workfile: PHY_RoleAction_Objects_DataComputer.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_Objects_DataComputer_h_
#define __PHY_RoleAction_Objects_DataComputer_h_

#include "MIL.h"

class MIL_RealObject_ABC;
class MIL_AgentPion;
class PHY_ComposantePion;
class PHY_ConsumptionType;
class PHY_DotationCategory;

// =============================================================================
// @class  PHY_RoleAction_Objects_DataComputer
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Objects_DataComputer
{
    MT_COPYNOTALLOWED( PHY_RoleAction_Objects_DataComputer )

public:
    //! @name Types
    //@{
    enum E_Operation
    {
        eConstruct,
        eDestroy,
        eMine,
        eDemine,
        eBypass,
        ePrepare
    };
    //@}

public:
     PHY_RoleAction_Objects_DataComputer( E_Operation nOperation, const MIL_RealObject_ABC& object );
    ~PHY_RoleAction_Objects_DataComputer();

    //! @name Operations
    //@{
    void     ReserveConsumptions             ();
    void     RollbackConsumptionsReservations();
    bool     HasDotations                    ( uint nNbr, const PHY_DotationCategory& category ) const;
    void     ConsumeDotations                ( uint nNbr, const PHY_DotationCategory& category );
    void     RecoverDotations                ( uint nNbr, const PHY_DotationCategory& category );
    MT_Float GetDeltaPercentage              () const;

    void     SetActivePion( MIL_AgentPion& pion );
    void     operator()   ( const PHY_ComposantePion& composante ) const;
    //@}

private:
    //! @name Types
    //@{
    struct sPionData
    {
        sPionData();
        sPionData( MIL_AgentPion& pion );
        MIL_AgentPion*             pPion_;
        MT_Float                   rTotalTime_;
        uint                       nNbrComposantes_;
        const PHY_ConsumptionType* pConsumptionMode_;
        bool                       bConsumptionReserved_;
    };
    typedef std::vector< sPionData >                 T_PionDataVector;
    typedef T_PionDataVector::iterator               IT_PionDataVector;
    typedef T_PionDataVector::reverse_iterator       RIT_PionDataVector;
    typedef T_PionDataVector::const_iterator         CIT_PionDataVector;
    typedef T_PionDataVector::const_reverse_iterator CRIT_PionDataVector;
    //@}

private:
    const E_Operation         nOperation_;
    const MIL_RealObject_ABC& object_;
          T_PionDataVector    pionsData_;
          sPionData*          pCurrentPionData_;
};

#include "PHY_RoleAction_Objects_DataComputer.inl"

#endif // __PHY_RoleAction_Objects_DataComputer_h_
