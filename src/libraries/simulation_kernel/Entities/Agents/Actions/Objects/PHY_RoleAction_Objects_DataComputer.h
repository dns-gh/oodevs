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

#include "simulation_kernel/OnComponentFunctor_ABC.h"
#include "PHY_RoleAction_Objects_DataComputerPionData.h"
#include "Operation.h"

class MIL_Object_ABC;
class MIL_Agent_ABC;
class PHY_ComposantePion;

// =============================================================================
// @class  PHY_RoleAction_Objects_DataComputer
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Objects_DataComputer : public OnComponentFunctor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleAction_Objects_DataComputer( MIL_Agent_ABC& pion, E_Operation nOperation, const MIL_Object_ABC& object );
    virtual ~PHY_RoleAction_Objects_DataComputer();
    //@}

    //! @name Operations
    //@{
    bool HasDotations( const PHY_DotationCategory& category, unsigned int nNbr ) const;
    void ConsumeDotations( const PHY_DotationCategory& category, double& nbr );
    void RecoverDotations( const PHY_DotationCategory& category, unsigned int nNbr );

    double ComputeDeltaPercentage();
    double ComputeWorkTime();

    void operator()( PHY_ComposantePion& composante );
    //@}

private:
    //! @name Tools
    //@{
    void CollectData( MIL_Agent_ABC& pion );
    void FilterData();

    void ReserveConsumptions();
    void RollbackConsumptionsReservations();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_RoleAction_Objects_DataComputerPionData > T_PionDataVector;
    typedef T_PionDataVector::iterator                                IT_PionDataVector;
    typedef T_PionDataVector::reverse_iterator                       RIT_PionDataVector;
    typedef T_PionDataVector::const_reverse_iterator                CRIT_PionDataVector;
    typedef T_PionDataVector::const_iterator                         CIT_PionDataVector;
    //@}

private:
    //! @name Member data
    //@{
    const E_Operation operation_;
    const MIL_Object_ABC& object_;
    T_PionDataVector pionsData_;
    //@}
};

#endif // __PHY_RoleAction_Objects_DataComputer_h_
