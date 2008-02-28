// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Attributes_h_
#define __Attributes_h_

#include "game_asn/Simulation.h"
#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Aggregatable_ABC.h"
#include "clients_kernel/Types.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class CoordinateConverter_ABC;
    class PropertiesDictionary;
    class Team_ABC;
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
             Attributes( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter
                       , kernel::PropertiesDictionary& dictionary, const kernel::Resolver_ABC< kernel::Team_ABC >& teamResolver );
    virtual ~Attributes();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;

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

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::Resolver_ABC< kernel::Team_ABC >& teamResolver_;

    geometry::Point2f vPos_;
    unsigned int nSpeed_;
    int	nAltitude_;
    int	nDirection_;

    unsigned              nRawOpState_;
    E_OperationalStatus    nOpState_;
    E_ForceRatioStatus          nFightRateState_;
    E_Roe                 nRulesOfEngagementState_;
    E_PopulationRoe       nRulesOfEngagementPopulationState_;
    E_MeetingEngagementStatus nCloseCombatState_;

    bool     bDead_;
    bool     bNeutralized_;

    E_UnitPosture nOldPosture_;
    E_UnitPosture nCurrentPosture_;
    uint		  nPostureCompletionPourcentage_;
    uint          nInstallationState_;

    E_FireAvailability    nIndirectFireAvailability_;

    bool bLoadingState_;
    bool bHumanTransportersReady_;
    bool bStealthModeEnabled_;

    bool bRadioSilence_;
    bool bCommJammed_;
    bool bRadarEnabled_;

    bool bPrisoner_;
    const kernel::Team_ABC* surrenderedTo_;
    bool bRefugeesManaged_;

    bool aggregated_;
    //@}
};

#endif // __Attributes_h_
