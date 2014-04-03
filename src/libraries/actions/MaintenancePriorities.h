// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterMaintenancePriorities_h_
#define __ActionParameterMaintenancePriorities_h_

#include "Parameter.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    class IdList;
}

namespace kernel
{
    class EquipmentType;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  MaintenancePriorities
    @brief  MaintenancePriorities
*/
// Created: SBO 2007-06-26
// =============================================================================
class MaintenancePriorities : public Parameter< std::string >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MaintenancePriorities( const kernel::OrderParameter& parameter );
             MaintenancePriorities( const kernel::OrderParameter& parameter, const tools::Resolver_ABC< kernel::EquipmentType >& resolver, const sword::IdList& message );
    virtual ~MaintenancePriorities();
    //@}

    //! @name Operations
    //@{
    void AddPriority( const kernel::EquipmentType& value );
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual void CommitTo( std::string& content ) const;
    virtual bool IsSet() const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

    //! @name Helpers
    //@{
    typedef std::vector< const kernel::EquipmentType* > T_Priorities;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    T_Priorities priorities_;
    //@}
};

}
}

#endif // __ActionParameterMaintenancePriorities_h_
