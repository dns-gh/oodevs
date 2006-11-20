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
#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Aggregatable_ABC.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class CoordinateConverter_ABC;
    class PropertiesDictionary;
}

// =============================================================================
/** @class  Attributes
    @brief  Attributes
*/
// Created: AGE 2006-02-13
// =============================================================================
class Attributes : public kernel::Attributes_ABC
                 , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
                 , public kernel::Drawable_ABC
                 , public kernel::Aggregatable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Attributes( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, kernel::PropertiesDictionary& dictionary );
    virtual ~Attributes();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;

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
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    virtual void Aggregate( const bool& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;

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

    E_UnitPosture nOldPosture_;
    E_UnitPosture nCurrentPosture_;
    uint		  nPostureCompletionPourcentage_;
    uint          nInstallationState_;

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
