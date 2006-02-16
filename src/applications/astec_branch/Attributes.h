// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Attributes_h_
#define __Attributes_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"

class Controller;

// =============================================================================
/** @class  Attributes
    @brief  Attributes
*/
// Created: AGE 2006-02-13
// =============================================================================
class Attributes : public Extension_ABC
                 , public Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             Attributes( Controller& controller );
    virtual ~Attributes();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Attributes( const Attributes& );            //!< Copy constructor
    Attributes& operator=( const Attributes& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    //@}

public: // $$$$ AGE 2006-02-16: 
    //! @name Member data
    //@{
    Controller& controller_;

    MT_Vector2D	 vPos_;
    unsigned int nSpeed_;
    int	nAltitude_;
    int	nDirection_;

    unsigned           nRawOpState_;
    E_EtatOperationnel nOpState_;
    E_EtatRapFor          nFightRateState_;
    E_Roe                 nRulesOfEngagementState_;
    E_RoePopulation       nRulesOfEngagementPopulationState_;
    E_EtatCombatRencontre nCloseCombatState_;

    bool     bDead_;
    bool     bNeutralized_;
    bool     bEmbraye_;

    E_UnitPosture nOldPosture_;
    E_UnitPosture nCurrentPosture_;
    uint		  nPostureCompletionPourcentage_;

    E_DisponibiliteAuTir    nIndirectFireAvailability_;

    bool bLoadingState_;
    bool bHumanTransportersReady_;
    bool bStealthModeEnabled_;

    bool bRadioSilence_;
    bool bCommJammed_;
    bool bRadarEnabled_;

    bool bPrisoner_;
    bool bSurrendered_;
    bool bRefugeesManaged_;
    //@}
};

#endif // __Attributes_h_
