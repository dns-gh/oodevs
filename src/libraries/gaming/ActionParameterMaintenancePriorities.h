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

#include "ActionParameter.h"
#include "game_asn/Asn.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class EquipmentType;
}

// =============================================================================
/** @class  ActionParameterMaintenancePriorities
    @brief  ActionParameterMaintenancePriorities
*/
// Created: SBO 2007-06-26
// =============================================================================
class ActionParameterMaintenancePriorities : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterMaintenancePriorities( const kernel::OrderParameter& parameter );
             ActionParameterMaintenancePriorities( const kernel::OrderParameter& parameter, const kernel::Resolver_ABC< kernel::EquipmentType >& resolver, const ASN1T_LogMaintenancePriorities& asn );
             ActionParameterMaintenancePriorities( const kernel::OrderParameter& parameter, const kernel::Resolver_ABC< kernel::EquipmentType >& resolver, xml::xistream& xis );
    virtual ~ActionParameterMaintenancePriorities();
    //@}

    //! @name Operations
    //@{
    void AddPriority( const kernel::EquipmentType& value );
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterMaintenancePriorities( const ActionParameterMaintenancePriorities& );            //!< Copy constructor
    ActionParameterMaintenancePriorities& operator=( const ActionParameterMaintenancePriorities& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::EquipmentType* > T_Priorities;
    //@}

private:
    //! @name Member data
    //@{
    T_Priorities priorities_;
    //@}
};

#endif // __ActionParameterMaintenancePriorities_h_
