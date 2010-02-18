// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MedicalCollectionConsign.h $
// $Author: Nld $
// $Modtime: 25/04/05 11:10 $
// $Revision: 3 $
// $Workfile: PHY_MedicalCollectionConsign.h $
//
// *****************************************************************************

#ifndef __PHY_MedicalCollectionConsign_h_
#define __PHY_MedicalCollectionConsign_h_

#include "MIL.h"
#include "PHY_MedicalConsign_ABC.h"

class PHY_RoleInterface_Medical;
class PHY_MedicalHumanState;

// =============================================================================
// @class  PHY_MedicalCollectionConsign
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MedicalCollectionConsign : public PHY_MedicalConsign_ABC
{
    friend class PHY_MedicalCollectionAmbulance;

public:
             PHY_MedicalCollectionConsign( PHY_RoleInterface_Medical& medical, PHY_MedicalHumanState& humanState );
             PHY_MedicalCollectionConsign();
    virtual ~PHY_MedicalCollectionConsign();

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
    void EnterStateWaitingForCollection();
    void CreateCollectionAmbulance     ();

    // Appel�s par PHY_MedicalCollectionAmbulance
    void EnterStateCollectionLoading              ();
    bool EnterStateCollectionWaitingForFullLoading();
    void EnterStateSearchingForSortingArea        ();
    void EnterStateCollectionGoingTo              ();
    void EnterStateCollectionUnloading            ();
    void TransferToSortingArea                    ( PHY_RoleInterface_Medical& sortingArea );
    void NotifyOutOfMedicalSystem                 ();
    //@}

private:
    PHY_MedicalCollectionAmbulance* pCollectionAmbulance_;
};

BOOST_CLASS_EXPORT_KEY( PHY_MedicalCollectionConsign )

#endif // __PHY_MedicalCollectionConsign_h_
