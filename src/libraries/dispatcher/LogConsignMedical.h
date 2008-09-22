 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogConsignMedical_h_
#define __LogConsignMedical_h_

#include "game_asn/Simulation.h"
#include "SimpleEntity.h"

namespace kernel
{
    class Agent_ABC;
}

namespace dispatcher
{
    class Model;
    class ModelVisitor_ABC;
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
             LogConsignMedical( const Model& model, const ASN1T_MsgLogMedicalHandlingCreation& msg );
    virtual ~LogConsignMedical();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const ASN1T_MsgLogMedicalHandlingUpdate& msg );
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    void Accept( ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogConsignMedical( const LogConsignMedical& );            //!< Copy constructor
    LogConsignMedical& operator=( const LogConsignMedical& ); //!< Assignement operator
    //@}

private:
    const Model&             model_;
    const kernel::Agent_ABC& agent_;
    const unsigned long      nTickCreation_;

    const kernel::Agent_ABC*         pTreatingAgent_;
    ASN1T_EnumHumanRank              nRank_;
    ASN1T_EnumHumanWound             nWound_;
    bool                             bMentalDiseased_;
    bool                             bContaminated_;
    ASN1T_EnumLogMedicalHandlingStatus nState_;
    bool                             bDiagnosed_;
};

}

#endif // __LogConsignMedical_h_
