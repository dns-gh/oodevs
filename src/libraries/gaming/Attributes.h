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
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Aggregatable_ABC.h"

class Controller;
class Displayer_ABC;
class CoordinateConverter_ABC;
class DataDictionary;

// =============================================================================
/** @class  Attributes
    @brief  Attributes
*/
// Created: AGE 2006-02-13
// =============================================================================
class Attributes : public Extension_ABC
                 , public Updatable_ABC< ASN1T_MsgUnitAttributes >
                 , public Drawable_ABC
                 , public Aggregatable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Attributes( Controller& controller, const CoordinateConverter_ABC& converter, DataDictionary& dictionary );
    virtual ~Attributes();
    //@}

    //! @name Operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
    void DisplayInTooltip( Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;

    float ComputePostureFactor( const std::vector< float >& factors ) const; // $$$$ AGE 2006-04-19: move in Postures ?
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
    virtual void Aggregate( const bool& );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const CoordinateConverter_ABC& converter_;

    geometry::Point2f vPos_;
    unsigned int nSpeed_;
    int	nAltitude_;
    int	nDirection_;

    unsigned              nRawOpState_;
    E_EtatOperationnel    nOpState_;
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

    bool aggregated_;
    //@}
};

#endif // __Attributes_h_
