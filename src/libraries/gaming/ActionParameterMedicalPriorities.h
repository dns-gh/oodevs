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

#include "ActionParameter.h"
#include "game_asn/Simulation.h"

// =============================================================================
/** @class  ActionParameterMedicalPriorities
    @brief  ActionParameterMedicalPriorities
*/
// Created: SBO 2007-06-26
// =============================================================================
class ActionParameterMedicalPriorities : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterMedicalPriorities( const kernel::OrderParameter& parameter );
             ActionParameterMedicalPriorities( const kernel::OrderParameter& parameter, const ASN1T_LogMedicalPriorities& asn );
             ActionParameterMedicalPriorities( const kernel::OrderParameter& parameter, xml::xistream& xis );
    virtual ~ActionParameterMedicalPriorities();
    //@}

    //! @name Operations
    //@{
    void AddMedicalPriority( E_HumanWound value );
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterMedicalPriorities( const ActionParameterMedicalPriorities& );            //!< Copy constructor
    ActionParameterMedicalPriorities& operator=( const ActionParameterMedicalPriorities& ); //!< Assignment operator
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

#endif // __ActionParameterMedicalPriorities_h_
