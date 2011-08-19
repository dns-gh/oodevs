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

#ifndef __PHY_RoleAction_Objects_DataComputerPionData_h_
#define __PHY_RoleAction_Objects_DataComputerPionData_h_

#include "Operation.h"

class MIL_Object_ABC;
class MIL_Agent_ABC;
class PHY_ComposantePion;
class PHY_DotationCategory;

// =============================================================================
// @class  PHY_RoleAction_Objects_DataComputerPionData
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Objects_DataComputerPionData
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleAction_Objects_DataComputerPionData();
             PHY_RoleAction_Objects_DataComputerPionData( MIL_Agent_ABC& pion, E_Operation operation, const MIL_Object_ABC& object );
    virtual ~PHY_RoleAction_Objects_DataComputerPionData() {}
    //@}

    //! @name Operations
    //@{
    void operator() ( const PHY_ComposantePion& composante );
    void RemoveSlowComposantes( double rMinOperationTime );
    double GetMinOperationTime() const;

    void ReserveConsumptions();
    void RollbackConsumptionsReservations();

    unsigned int GetDotationValue( const PHY_DotationCategory& category ) const;
    void ConsumeDotations( const PHY_DotationCategory& category, double& nNbr );
    unsigned int RecoverDotations( const PHY_DotationCategory& category, unsigned int nNbr );
    void GetTotalOperationTime( double& rOperationTime, unsigned int& nNbrComposantes ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< std::pair< const PHY_ComposantePion*, double > > T_ComposanteDataVector;
    typedef T_ComposanteDataVector::iterator                               IT_ComposanteDataVector;
    typedef T_ComposanteDataVector::const_iterator                        CIT_ComposanteDataVector;
    //@}

private:
    //! @name Member data
    //@{
    E_Operation operation_;
    const MIL_Object_ABC* pObject_;
    MIL_Agent_ABC* pPion_;
    bool bConsumptionReserved_;
    T_ComposanteDataVector workingComposantes_;
    //@}
};


#endif // __PHY_RoleAction_Objects_DataComputerPionData_h_
