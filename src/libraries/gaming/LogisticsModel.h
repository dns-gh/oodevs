// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticsModel_h_
#define __LogisticsModel_h_

#include "tools/Resolver.h"

namespace sword
{
    class LogMaintenanceHandlingCreation;
    class LogMedicalHandlingCreation;
    class LogSupplyHandlingCreation;
    class LogFuneralHandlingCreation;
}

class LogMaintenanceConsign;
class LogSupplyConsign;
class LogMedicalConsign;
class LogFuneralConsign;
class LogisticConsignFactory_ABC;

// =============================================================================
/** @class  LogisticsModel
    @brief  LogisticsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class LogisticsModel : public tools::Resolver< LogMaintenanceConsign >
                     , public tools::Resolver< LogMedicalConsign >
                     , public tools::Resolver< LogSupplyConsign >
                     , public tools::Resolver< LogFuneralConsign >
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticsModel( LogisticConsignFactory_ABC& factory );
    virtual ~LogisticsModel();
    //@}

    //! @name Operations
    //@{
    void Purge();

    void CreateMaintenanceConsign( const sword::LogMaintenanceHandlingCreation& message );
    LogMaintenanceConsign& GetMaintenanceConsign( unsigned long id );
    void DeleteMaintenanceConsign( unsigned long id );

    void CreateMedicalConsign( const sword::LogMedicalHandlingCreation& message );
    LogMedicalConsign& GetMedicalConsign( unsigned long id );
    void DeleteMedicalConsign( unsigned long id );

    void CreateSupplyConsign( const sword::LogSupplyHandlingCreation& message );
    LogSupplyConsign& GetSupplyConsign( unsigned long id );
    void DeleteSupplyConsign( unsigned long id );

    void CreateFuneralConsign( const sword::LogFuneralHandlingCreation& message );
    LogFuneralConsign& GetFuneralConsign( unsigned long id );
    void DeleteFuneralConsign( unsigned long id );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticsModel( const LogisticsModel& );            //!< Copy constructor
    LogisticsModel& operator=( const LogisticsModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    template< typename Type >
    void Delete( unsigned long id );
    //@}

private:
    //! @name Member data
    //@{
    LogisticConsignFactory_ABC& factory_;
    //@}
};

#endif // __LogisticsModel_h_
