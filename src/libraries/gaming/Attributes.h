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

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/SubTypes.h"
#include "clients_kernel/Types.h"
#include "clients_kernel/OptionalValue.h"
#include "protocol/SimulationSenders.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    class UnitAttributes;
}

namespace kernel
{
    class Controller;
    class CoordinateConverter_ABC;
    class DetectionMap;
    class Displayer_ABC;
    class Entity_ABC;
    class SensorType;
    class Team_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

// =============================================================================
/** @class  Attributes
    @brief  Attributes
*/
// Created: AGE 2006-02-13
// =============================================================================
class Attributes : public kernel::Extension_ABC
                 , public kernel::Displayable_ABC
                 , public kernel::Updatable_ABC< sword::UnitAttributes >
                 , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Attributes( kernel::Entity_ABC& entity, kernel::Controller& controller, const kernel::DetectionMap& elevation,
                         const kernel::CoordinateConverter_ABC& converter, gui::PropertiesDictionary& dictionary,
                         const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver );
    virtual ~Attributes();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    float GetDistanceModificator( const kernel::SensorType& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Attributes( const Attributes& );            //!< Copy constructor
    Attributes& operator=( const Attributes& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( gui::PropertiesDictionary& dico ) const;
    void UpdateHierarchies();
    virtual void DoUpdate( const sword::UnitAttributes& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC& entity_;
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::DetectionMap& elevation_;
    const kernel::CoordinateConverter_ABC& converter_;
    const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver_;
    geometry::Point2f vPos_;
    int nSpeed_;
    int nAltitude_;
    int nDirection_;
    int nSlope_;
    int nRawOpState_;
    E_OperationalStatus nOpState_;
    E_ForceRatioStatus nFightRateState_;
    E_Roe nRulesOfEngagementState_;
    E_PopulationRoe nRulesOfEngagementPopulationState_;
    E_MeetingEngagementStatus nCloseCombatState_;
    E_UnitPosture nOldPosture_;
    E_UnitPosture nCurrentPosture_;
    uint nPostureCompletionPourcentage_;
    int nInstallationState_;
    E_FireAvailability nIndirectFireAvailability_;
    bool bDead_;
    bool bNeutralized_;
    bool bLoadingState_;
    bool bHumanTransportersReady_;
    bool bStealthModeEnabled_;
    bool bRadioEmitterSilence_;
    bool bRadioReceiverSilence_;
    bool bCommJammed_;
    bool bRadarEnabled_;
    bool bPrisoner_;
    bool bUnderground_;
    bool bRefugeesManaged_;
    bool isPC_;
    const kernel::Team_ABC* surrenderedTo_;
    unsigned int knowledgeGroupJammed_;
    std::map< std::string, std::string > extensions_;
    float fLodgingSatisfactionPercent_;
    float fSecuritySatisfactionPercent_;
    float fHealthSatisfactionPercent_;
    int crowdTransported_;
    const float cellDistance_;
    bool bAmbianceSafety_;
    //@}
};

#endif // __Attributes_h_
