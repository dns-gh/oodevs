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

#include "game_asn/Asn.h"
#include "ModelRefsContainer.h"
#include "ModelsContainer.h"
#include "EquipmentAvailability.h"

namespace dispatcher
{
class Model;
class Agent;
class Automat;
class Publisher_ABC;

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
     AgentLogMedical( Model& model, const Agent& agent, const ASN1T_MsgLogMedicalState& asnMsg );
    ~AgentLogMedical();
    //@}

    //! @name Main
    //@{
    void Update(  const ASN1T_MsgLogMedicalState& asnMsg );
    void Send  ( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< ASN1T_EnumHumanWound > T_HumanWoundVector;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentLogMedical( const AgentLogMedical& );            //!< Copy constructor
    AgentLogMedical& operator=( const AgentLogMedical& ); //!< Assignement operator
    //@}

private:
    const Agent&                          agent_;
          Model&                          model_;
          bool                            bSystemEnabled_;
          T_HumanWoundVector              priorities_;
          ModelRefsContainer< Automat >   tacticalPriorities_;

    ModelsContainer< EquipmentAvailability< ASN1T_LogMedicalEquipmentAvailability > > evacuationAmbulancesAvailability_;
    ModelsContainer< EquipmentAvailability< ASN1T_LogMedicalEquipmentAvailability > > collectionAmbulancesAvailability_;
    ModelsContainer< EquipmentAvailability< ASN1T_LogMedicalEquipmentAvailability > > doctorsAvailability_;
};

}

#endif // __AgentLogMedical_h_
