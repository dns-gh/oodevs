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
#include "Entity_ABC.h"

namespace dispatcher
{
    class Agent;
    class Model;

// =============================================================================
/** @class  LogConsignMedical
    @brief  LogConsignMedical
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogConsignMedical : public Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LogConsignMedical( Model& model, const ASN1T_MsgLogMedicalHandlingCreation& msg );
    virtual ~LogConsignMedical();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetID() const;
    //@}

    //! @name Operations
    //@{
    using Entity_ABC::Update;
    void Update( const ASN1T_MsgLogMedicalHandlingUpdate& msg );
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogConsignMedical( const LogConsignMedical& );            //!< Copy constructor
    LogConsignMedical& operator=( const LogConsignMedical& ); //!< Assignement operator
    //@}

private:
          Model&        model_;
    const unsigned long nID_;
    const Agent&        agent_;
    const unsigned long nTickCreation_;

    Agent*                           pTreatingAgent_;
    ASN1T_EnumHumanRank              nRank_;
    ASN1T_EnumHumanWound             nWound_;
    bool                             bMentalDiseased_;
    bool                             bContaminated_;
    ASN1T_EnumLogMedicalHandlingStatus nState_;
    bool                             bDiagnosed_;
};

}

#include "LogConsignMedical.inl"

#endif // __LogConsignMedical_h_
