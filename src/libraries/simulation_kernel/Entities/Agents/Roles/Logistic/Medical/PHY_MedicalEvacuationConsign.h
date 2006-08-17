// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_MedicalEvacuationConsign.h $
// $Author: Nld $
// $Modtime: 25/04/05 11:10 $
// $Revision: 3 $
// $Workfile: PHY_MedicalEvacuationConsign.h $
//
// *****************************************************************************

#ifndef __PHY_MedicalEvacuationConsign_h_
#define __PHY_MedicalEvacuationConsign_h_

#include "MIL.h"

#include "PHY_MedicalConsign_ABC.h"

class PHY_RolePionLOG_Medical;
class PHY_MedicalHumanState;

// =============================================================================
// @class  PHY_MedicalEvacuationConsign
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MedicalEvacuationConsign : public PHY_MedicalConsign_ABC
{
    MT_COPYNOTALLOWED( PHY_MedicalEvacuationConsign )

    friend class PHY_MedicalEvacuationAmbulance;

public:
             PHY_MedicalEvacuationConsign( PHY_RolePionLOG_Medical& medical, PHY_MedicalHumanState& humanState );
             PHY_MedicalEvacuationConsign();
    virtual ~PHY_MedicalEvacuationConsign();

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
    void EnterStateWaitingForEvacuation();
    void CreateEvacuationAmbulance     ();

    // Appelés par PHY_MedicalEvacuationAmbulance
    void EnterStateEvacuationGoingTo              ();
    void EnterStateEvacuationLoading              ();
    bool EnterStateEvacuationWaitingForFullLoading();
    void EnterStateEvacuationGoingFrom            ();
    void EnterStateEvacuationUnloading            ();
    void ChooseStateAfterEvacuation               ();

    bool DoWaitingForDiagnostic                   ();
    void EnterStateDiagnosing                     ();
    void EnterStateWaitingForCollection           ();
    bool DoWaitingForCollection                   ();
    //@}

private:
    PHY_ComposantePion*             pDoctor_;
    PHY_MedicalEvacuationAmbulance* pEvacuationAmbulance_;
};

#include "PHY_MedicalEvacuationConsign.inl"

#endif // __PHY_MedicalEvacuationConsign_h_
