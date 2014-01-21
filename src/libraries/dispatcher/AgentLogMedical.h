 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentLogMedical_h_
#define __AgentLogMedical_h_

#include "EquipmentAvailability.h"

namespace sword
{
    enum EnumHumanWound;
    class Id;
    class LogMedicalState;
    class LogMedicalEquipmentAvailability;
}

namespace kernel
{
    class Automat_ABC;
    class Agent_ABC;
}

namespace dispatcher
{
    class Model_ABC;
    class ClientPublisher_ABC;
    class MedicalEquipmentAvailability;

// =============================================================================
/** @class  AgentLogMedical
    @brief  AgentLogMedical
*/
// Created: NLD 2006-09-19
// =============================================================================
class AgentLogMedical
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentLogMedical( const Model_ABC& model, const kernel::Agent_ABC& agent, const sword::LogMedicalState& asnMsg );
    virtual ~AgentLogMedical();
    //@}

    //! @name Main
    //@{
    void Update(  const sword::LogMedicalState& asnMsg );
    void Send  ( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentLogMedical( const AgentLogMedical& );            //!< Copy constructor
    AgentLogMedical& operator=( const AgentLogMedical& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Model_ABC&         model_;
    const kernel::Agent_ABC& agent_;
    bool                     bSystemEnabled_;

    std::vector< sword::EnumHumanWound >       priorities_;
    std::vector< sword::Id > tacticalPriorities_;

    std::vector< MedicalEquipmentAvailability > evacuationAmbulancesAvailability_;
    std::vector< MedicalEquipmentAvailability > collectionAmbulancesAvailability_;
    std::vector< MedicalEquipmentAvailability > doctorsAvailability_;
    //@}
};

}

#endif // __AgentLogMedical_h_
