// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticConsigns_h_
#define __LogisticConsigns_h_

#include "ASN_Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Controller;
}

class LogSupplyConsign;
class LogMaintenanceConsign;
class LogMedicalConsign;

// =============================================================================
/** @class  LogisticConsigns
    @brief  LogisticConsigns
*/
// Created: AGE 2006-02-28
// =============================================================================
class LogisticConsigns : public kernel::Extension_ABC
                       , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LogisticConsigns( kernel::Controller& controller );
    virtual ~LogisticConsigns();
    //@}

    //! @name Operations
    //@{
    void AddConsign   ( LogSupplyConsign& consign );
    void RemoveConsign( LogSupplyConsign& consign );

    void AddConsign   ( LogMaintenanceConsign& consign );
    void RemoveConsign( LogMaintenanceConsign& consign );

    void AddConsign   ( LogMedicalConsign& consign );
    void RemoveConsign( LogMedicalConsign& consign );

    void HandleConsign   ( LogSupplyConsign& consign );
    void TerminateConsign( LogSupplyConsign& consign );

    void HandleConsign   ( LogMaintenanceConsign& consign );
    void TerminateConsign( LogMaintenanceConsign& consign );

    void HandleConsign   ( LogMedicalConsign& consign );
    void TerminateConsign( LogMedicalConsign& consign );

    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticConsigns( const LogisticConsigns& );            //!< Copy constructor
    LogisticConsigns& operator=( const LogisticConsigns& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::set< const LogSupplyConsign* >      T_SupplyConsigns;
    typedef std::set< const LogMaintenanceConsign* > T_MaintenanceConsigns;
    typedef std::set< const LogMedicalConsign* >     T_MedicalConsigns;
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;

    T_SupplyConsigns      requestedSupplies_;
    T_SupplyConsigns      handledSupplies_;
    T_MaintenanceConsigns requestedMaintenances_;
    T_MaintenanceConsigns handledMaintenances_;
    T_MedicalConsigns     requestedMedical_;
    T_MedicalConsigns     handledMedical_;
    //@}
};

#endif // __LogisticConsigns_h_
