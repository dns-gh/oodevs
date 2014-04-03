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

namespace sword
{
    class LogMedicalPriorities;
}

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
             MedicalPriorities( const kernel::OrderParameter& parameter, const sword::LogMedicalPriorities& message );
    virtual ~MedicalPriorities();
    //@}

    //! @name Operations
    //@{
    void AddMedicalPriority( E_HumanWound value );
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual bool IsSet() const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
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
