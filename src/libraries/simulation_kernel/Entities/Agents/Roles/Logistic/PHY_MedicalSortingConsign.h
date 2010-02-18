// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_MedicalSortingConsign.h $
// $Author: Nld $
// $Modtime: 25/04/05 11:10 $
// $Revision: 3 $
// $Workfile: PHY_MedicalSortingConsign.h $
//
// *****************************************************************************

#ifndef __PHY_MedicalSortingConsign_h_
#define __PHY_MedicalSortingConsign_h_

#include "MIL.h"
#include "PHY_MedicalConsign_ABC.h"

class PHY_ComposantePion;
class PHY_MedicalHumanState;
class PHY_RoleInterface_Medical;

// =============================================================================
// @class  PHY_MedicalSortingConsign
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MedicalSortingConsign : public PHY_MedicalConsign_ABC
{

public:
             PHY_MedicalSortingConsign( PHY_RoleInterface_Medical& medical, PHY_MedicalHumanState& humanState );
             PHY_MedicalSortingConsign();
    virtual ~PHY_MedicalSortingConsign();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    virtual void Cancel();
    virtual bool Update(); 
    //@}

    //! @name Accessors
    //@{
    virtual bool CouldNeedCollectionAmbulance() const;
    virtual bool IsATransportConsign         () const;
    //@}

private:
    //! @name States
    //@{
    void EnterStateWaitingForSorting      ();
    bool DoWaitingForSorting              ();
    void EnterStateSorting                ();
    void EnterStateSearchingForHealingArea();
    void DoSearchForHealingArea           ();
    void EnterStateWaitingForCollection   ();
    bool DoWaitingForCollection           ();
    //@}

private:
    PHY_ComposantePion* pDoctor_;
};

#endif // __PHY_MedicalSortingConsign_h_
