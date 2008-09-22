// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterMedicalPriorities_h_
#define __ActionParameterMedicalPriorities_h_

#include "Parameter.h"
#include "game_asn/Simulation.h"

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  MedicalPriorities
    @brief  MedicalPriorities
*/
// Created: SBO 2007-06-26
// =============================================================================
class MedicalPriorities : public Parameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MedicalPriorities( const kernel::OrderParameter& parameter );
             MedicalPriorities( const kernel::OrderParameter& parameter, const ASN1T_LogMedicalPriorities& asn );
             MedicalPriorities( const kernel::OrderParameter& parameter, xml::xistream& xis );
    virtual ~MedicalPriorities();
    //@}

    //! @name Operations
    //@{
    void AddMedicalPriority( E_HumanWound value );
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MedicalPriorities( const MedicalPriorities& );            //!< Copy constructor
    MedicalPriorities& operator=( const MedicalPriorities& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< E_HumanWound > T_Priorities;
    //@}

private:
    //! @name Member data
    //@{
    T_Priorities priorities_;
    //@}
};

    }
}

#endif // __ActionParameterMedicalPriorities_h_
