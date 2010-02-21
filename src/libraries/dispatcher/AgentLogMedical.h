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

namespace Common
{
    enum EnumHumanWound;
}

namespace MsgsSimToClient
{
    class MsgLogMedicalState;
    class MsgLogMedicalEquipmentAvailability;
}

namespace kernel
{
    class Automat_ABC;
    class Agent_ABC;
}

namespace dispatcher
{
    class Model;
    class ClientPublisher_ABC;

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
             AgentLogMedical( const Model& model, const kernel::Agent_ABC& agent, const MsgsSimToClient::MsgLogMedicalState& asnMsg );
    virtual ~AgentLogMedical();
    //@}

    //! @name Main
    //@{
    void Update(  const MsgsSimToClient::MsgLogMedicalState& asnMsg );
    void Send  ( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentLogMedical( const AgentLogMedical& );            //!< Copy constructor
    AgentLogMedical& operator=( const AgentLogMedical& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef EquipmentAvailability< MsgsSimToClient::MsgLogMedicalEquipmentAvailability > T_Availability;
    //@}

private:
    //! @name Member data
    //@{
    const Model&             model_;
    const kernel::Agent_ABC& agent_;
    bool                     bSystemEnabled_;

    std::vector< Common::EnumHumanWound >       priorities_;
    std::vector< const kernel::Automat_ABC* > tacticalPriorities_;

    std::vector< T_Availability > evacuationAmbulancesAvailability_;
    std::vector< T_Availability > collectionAmbulancesAvailability_;
    std::vector< T_Availability > doctorsAvailability_;
    //@}
};

}

#endif // __AgentLogMedical_h_
