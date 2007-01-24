// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalStates_h_
#define __MedicalStates_h_

#include "network/Simulation_Asn.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "Availability.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Controller;
    class EquipmentType;
    class Agent_ABC;
    class Displayer_ABC;
    class PropertiesDictionary;
}

// =============================================================================
/** @class  MedicalStates
    @brief  MedicalStates
*/
// Created: AGE 2006-02-14
// =============================================================================
class MedicalStates : public kernel::Extension_ABC
                    , public kernel::Updatable_ABC< ASN1T_MsgLogSanteEtat >
{
public:
    //! @name Constructors/Destructor
    //@{
             MedicalStates( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::EquipmentType >& resolver, const kernel::Resolver_ABC< kernel::Agent_ABC >& agentResolver, kernel::PropertiesDictionary& dico );
    virtual ~MedicalStates();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DoUpdate( const ASN1T_MsgLogSanteEtat& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MedicalStates( const MedicalStates& );            //!< Copy constructor
    MedicalStates& operator=( const MedicalStates& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< E_HumanWound > T_Priorities;
    typedef std::vector< const kernel::Agent_ABC* > T_Agents;
    typedef std::vector< Availability > T_Availabilities;
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller&      controller_;
    const kernel::Resolver_ABC< kernel::EquipmentType >& resolver_;
    const kernel::Resolver_ABC< kernel::Agent_ABC >&         agentResolver_;

    bool             bChainEnabled_;
    T_Availabilities dispoReleveAmbulances_;
    T_Availabilities dispoRamassageAmbulances_;  
    T_Availabilities dispoDoctors_;
    T_Priorities     priorities_;
    T_Agents         tacticalPriorities_;
    //@}
};

#endif // __MedicalStates_h_
