// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef MaintenanceFunctions_h
#define MaintenanceFunctions_h

#include "ENT/ENT_Enums.h"

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

namespace kernel
{
    class BreakdownType;
    class ComponentType;
    class EquipmentType;

// =============================================================================
/** @class  MaintenanceFunctions
    @brief  MaintenanceFunctions
*/
// Created: ABR 2014-02-06
// =============================================================================
class MaintenanceFunctions : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MaintenanceFunctions( xml::xistream& xis );
    virtual ~MaintenanceFunctions();
    //@}

    //! @name Operations
    //@{
    bool CanHaul( const EquipmentType& equipment ) const;
    bool CanHaul( const ComponentType& component ) const;
    bool CanRepair( const BreakdownType& breakdown ) const;
    //@}

private:
    //! @name Types
    //@{
    struct TowCapacity
    {
        explicit TowCapacity( xml::xistream& xis );
        float capacity_;
        std::string loadTime_;
        std::string unloadTime_;
    };
    struct RepairCapacity
    {
        explicit RepairCapacity( xml::xistream& xis );
        boost::optional< unsigned int > maxRepairTime_;
        std::vector< E_BreakdownType > supportedTypes_;
    };
    //@}

    //! @name Member data
    //@{
    boost::optional< TowCapacity > towCapacity_;
    std::map< E_BreakdownNTI, const RepairCapacity > repairCapacities_;
    //@}
};

} //! namespace kernel

#endif // MaintenanceFunctions_h
