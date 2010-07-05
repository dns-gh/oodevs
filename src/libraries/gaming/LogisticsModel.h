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

namespace MsgsSimToClient
{
    class MsgLogMaintenanceHandlingCreation;
    class MsgLogMedicalHandlingCreation;
    class MsgLogSupplyHandlingCreation;
}

class LogMaintenanceConsign;
class LogSupplyConsign;
class LogMedicalConsign;
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

    void CreateMaintenanceConsign( const MsgsSimToClient::MsgLogMaintenanceHandlingCreation& message );
    LogMaintenanceConsign& GetMaintenanceConsign( unsigned long id );
    void DeleteMaintenanceConsign( unsigned long id );

    void CreateMedicalConsign( const MsgsSimToClient::MsgLogMedicalHandlingCreation& message );
    LogMedicalConsign& GetMedicalConsign( unsigned long id );
    void DeleteMedicalConsign( unsigned long id );

    void CreateSupplyConsign( const MsgsSimToClient::MsgLogSupplyHandlingCreation& message );
    LogSupplyConsign& GetSupplyConsign( unsigned long id );
    void DeleteSupplyConsign( unsigned long id );
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
