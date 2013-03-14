 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LogConsignMedical_h_
#define __LogConsignMedical_h_

#include "SimpleEntity.h"
#include "protocol/ClientSenders.h"

namespace kernel
{
    class Agent_ABC;
    class ModelVisitor_ABC;
}

namespace dispatcher
{
    class Model;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  LogConsignMedical
    @brief  LogConsignMedical
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogConsignMedical : public SimpleEntity< >
{
public:
    //! @name Constructors/Destructor
    //@{
             LogConsignMedical( const Model& model, const sword::LogMedicalHandlingCreation& msg );
    virtual ~LogConsignMedical();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const sword::LogMedicalHandlingUpdate& msg );
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    bool IsObsoleteForUnit( unsigned int id ) const;
    //@}

private:
    //! @name Member data
    //@{
    const Model& model_;
    const kernel::Agent_ABC& agent_;
    const unsigned long nTickCreation_;
    const kernel::Agent_ABC* pTreatingAgent_;
    sword::EnumHumanRank nRank_;
    sword::EnumHumanWound nWound_;
    bool bMentalDiseased_;
    bool bContaminated_;
    sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus nState_;
    unsigned long currentStateEndTick_;
    bool bDiagnosed_;
    //@}
};

}

#endif // __LogConsignMedical_h_
