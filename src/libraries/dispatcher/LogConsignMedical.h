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

#include "network/Simulation_Asn.h"

namespace dispatcher
{
class Dispatcher;
class Agent;
class Model;
class Publisher_ABC;

// =============================================================================
/** @class  LogConsignMedical
    @brief  LogConsignMedical
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogConsignMedical
{
public:
    //! @name Constructors/Destructor
    //@{
     LogConsignMedical( Model& model, const ASN1T_MsgLogSanteTraitementHumainCreation& msg );
    ~LogConsignMedical();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetID() const;
    //@}

    //! @name Operations
    //@{
    void Update        ( const ASN1T_MsgLogSanteTraitementHumainUpdate& msg );
    void SendCreation  ( Publisher_ABC& publisher ) const;
    void SendFullUpdate( Publisher_ABC& publisher ) const;
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
          
    Agent*                           pTreatingAgent_;
    ASN1T_EnumHumanRank              nRank_;
    ASN1T_EnumHumanWound             nWound_;
    bool                             bMentalDiseased_;
    bool                             bContaminated_;
    ASN1T_EnumLogSanteTraitementEtat nState_;
    bool                             bDiagnosed_;
};

}

#include "LogConsignMedical.inl"

#endif // __LogConsignMedical_h_
