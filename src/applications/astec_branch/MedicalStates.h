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

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Availability.h"
#include "Resolver_ABC.h"

class Controller;
class EquipmentType;
class Agent;
class Displayer_ABC;
class DataDictionary;

// =============================================================================
/** @class  MedicalStates
    @brief  MedicalStates
*/
// Created: AGE 2006-02-14
// =============================================================================
class MedicalStates : public Extension_ABC
                    , public Updatable_ABC< ASN1T_MsgLogSanteEtat >
{
public:
    //! @name Constructors/Destructor
    //@{
             MedicalStates( Controller& controller, const Resolver_ABC< EquipmentType >& resolver, const Resolver_ABC< Agent >& agentResolver, DataDictionary& dico );
    virtual ~MedicalStates();
    //@}

    //! @name Operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
    virtual void DoUpdate( const ASN1T_MsgLogSanteEtat& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MedicalStates( const MedicalStates& );            //!< Copy constructor
    MedicalStates& operator=( const MedicalStates& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< E_HumanWound > T_Priorities;
    typedef std::vector< const Agent* > T_Agents;
    typedef std::vector< Availability > T_Availabilities;
    //@}

public:
    //! @name Member data
    //@{
    Controller&      controller_;
    const Resolver_ABC< EquipmentType >& resolver_;
    const Resolver_ABC< Agent >&         agentResolver_;

    bool             bChainEnabled_;
    T_Availabilities dispoReleveAmbulances_;
    T_Availabilities dispoRamassageAmbulances_;  
    T_Availabilities dispoDoctors_;
    T_Priorities     priorities_;
    T_Agents         tacticalPriorities_;
    //@}
};

#endif // __MedicalStates_h_
