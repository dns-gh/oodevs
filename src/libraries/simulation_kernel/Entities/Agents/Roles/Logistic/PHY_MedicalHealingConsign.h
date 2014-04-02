// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MedicalHealingConsign.h $
// $Author: Jvt $
// $Modtime: 29/04/05 11:13 $
// $Revision: 4 $
// $Workfile: PHY_MedicalHealingConsign.h $
//
// *****************************************************************************

#ifndef __PHY_MedicalHealingConsign_h_
#define __PHY_MedicalHealingConsign_h_

#include "MIL.h"
#include "PHY_MedicalConsign_ABC.h"

class PHY_ComposantePion;
class PHY_MedicalHumanState;
class PHY_RoleInterface_Medical;

// =============================================================================
// @class  PHY_MedicalHealingConsign
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MedicalHealingConsign : public PHY_MedicalConsign_ABC
{
public:
             PHY_MedicalHealingConsign( MIL_Agent_ABC& medical, PHY_MedicalHumanState& humanState );
             PHY_MedicalHealingConsign();
    virtual ~PHY_MedicalHealingConsign();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
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
    void EnterStateWaitingForHealing      ();
    bool DoWaitingForHealing              ();
    void EnterStateHealing                ();
    void EnterStateResting                ();
    void ChooseStateAfterResting          ();
    void EnterStateSearchingForHealingArea();
    void DoSearchForHealingArea           ();
    void EnterStateWaitingForCollection   ();
    bool DoWaitingForCollection           ();
    //@}

private:
    PHY_ComposantePion* pDoctor_;
};

BOOST_CLASS_EXPORT_KEY( PHY_MedicalHealingConsign )

#endif // __PHY_MedicalHealingConsign_h_
