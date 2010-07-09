// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterMaintenancePriorities_h_
#define __ActionParameterMaintenancePriorities_h_

#include "Parameter.h"
#include "tools/Resolver_ABC.h"

namespace Common
{
    class MsgLogMaintenancePriorities;
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
             MaintenancePriorities( const kernel::OrderParameter& parameter, const tools::Resolver_ABC< kernel::EquipmentType >& resolver, const Common::MsgLogMaintenancePriorities& message );
             MaintenancePriorities( const kernel::OrderParameter& parameter, const tools::Resolver_ABC< kernel::EquipmentType >& resolver, xml::xistream& xis );
    virtual ~MaintenancePriorities();
    //@}

    //! @name Operations
    //@{
    void AddPriority( const kernel::EquipmentType& value );
    virtual void CommitTo( Common::MsgMissionParameter& message ) const;
    virtual void CommitTo( std::string& content ) const;
    virtual void Clean( Common::MsgMissionParameter& message ) const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MaintenancePriorities( const MaintenancePriorities& );            //!< Copy constructor
    MaintenancePriorities& operator=( const MaintenancePriorities& ); //!< Assignment operator
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

    }
}

#endif // __ActionParameterMaintenancePriorities_h_
