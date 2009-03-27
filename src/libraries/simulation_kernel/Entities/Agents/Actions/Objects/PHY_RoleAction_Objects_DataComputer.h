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

#include "PHY_RoleAction_Objects_DataComputerPionData.h"

class MIL_Object_ABC;
class MIL_AgentPion;
class PHY_ComposantePion;

// =============================================================================
// @class  PHY_RoleAction_Objects_DataComputer
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Objects_DataComputer
{
    MT_COPYNOTALLOWED( PHY_RoleAction_Objects_DataComputer )

public:
     PHY_RoleAction_Objects_DataComputer( MIL_AgentPion& pion, PHY_RoleAction_Objects_DataComputerPionData::E_Operation nOperation, const MIL_Object_ABC& object );
    ~PHY_RoleAction_Objects_DataComputer();


    //! @name Operations
    //@{  
    bool     HasDotations                    ( const PHY_DotationCategory& category, uint nNbr ) const;
    void     ConsumeDotations                ( const PHY_DotationCategory& category, uint nNbr );
    void     RecoverDotations                ( const PHY_DotationCategory& category, uint nNbr );

    MT_Float ComputeDeltaPercentage          ();

    void     operator()                      ( const PHY_ComposantePion& composante );
    //@}

private:
    //! @name Tools
    //@{
    void CollectData                     ( MIL_AgentPion& pion );
    void FilterData                      ();

    void ReserveConsumptions             ();
    void RollbackConsumptionsReservations();   
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_RoleAction_Objects_DataComputerPionData >  T_PionDataVector;
    typedef T_PionDataVector::iterator                                  IT_PionDataVector;
    typedef T_PionDataVector::reverse_iterator                          RIT_PionDataVector;
    typedef T_PionDataVector::const_reverse_iterator                    CRIT_PionDataVector;
    typedef T_PionDataVector::const_iterator                            CIT_PionDataVector;
    //@}

private:
          MIL_AgentPion&                                            pion_;
    const PHY_RoleAction_Objects_DataComputerPionData::E_Operation  operation_;
    const MIL_Object_ABC&                                       object_;
          T_PionDataVector                                          pionsData_;

};

#endif // __PHY_RoleAction_Objects_DataComputer_h_
