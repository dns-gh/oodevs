// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalStates_h_
#define __MedicalStates_h_


#include "Availability.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Types.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/Protocol.h"
#include "tools/Resolver_ABC.h"

using namespace Common;

namespace kernel
{
    class Controller;
    class EquipmentType;
    class Automat_ABC;
    class Displayer_ABC;
    class PropertiesDictionary;
}

namespace MsgsSimToClient
{
    class MsgLogMedicalState;
}

// =============================================================================
/** @class  MedicalStates
    @brief  MedicalStates
*/
// Created: AGE 2006-02-14
// =============================================================================
class MedicalStates : public kernel::Extension_ABC
                    , public kernel::Updatable_ABC< MsgsSimToClient::MsgLogMedicalState >
{
public:
    //! @name Constructors/Destructor
    //@{
             MedicalStates( kernel::Controller& controller, const tools::Resolver_ABC< kernel::EquipmentType >& resolver, const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, kernel::PropertiesDictionary& dico );
    virtual ~MedicalStates();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DoUpdate( const MsgsSimToClient::MsgLogMedicalState& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MedicalStates( const MedicalStates& );            //!< Copy constructor
    MedicalStates& operator=( const MedicalStates& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< E_HumanWound > T_Priorities;
    typedef std::vector< const kernel::Automat_ABC* > T_Automats;
    typedef std::vector< Availability > T_Availabilities;
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller&      controller_;
    const tools::Resolver_ABC< kernel::EquipmentType >& resolver_;
    const tools::Resolver_ABC< kernel::Automat_ABC >&   automatResolver_;

    bool             bChainEnabled_;
    T_Availabilities dispoReleveAmbulances_;
    T_Availabilities dispoRamassageAmbulances_;
    T_Availabilities dispoDoctors_;
    T_Priorities     priorities_;
    T_Automats       tacticalPriorities_;
    //@}
};

#endif // __MedicalStates_h_
