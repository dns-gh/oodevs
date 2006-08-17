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

#include "ASN_Types.h"
#include "clients_kernel/Resolver.h"

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
class LogisticsModel : public Resolver< LogMaintenanceConsign >
                     , public Resolver< LogMedicalConsign >
                     , public Resolver< LogSupplyConsign >
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

    void CreateMaintenanceConsign( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& asnMsg );
    LogMaintenanceConsign& GetMaintenanceConsign( unsigned long id );
    void DeleteMaintenanceConsign( unsigned long id );

    void CreateMedicalConsign( const ASN1T_MsgLogSanteTraitementHumainCreation& asnMsg );
    LogMedicalConsign& GetMedicalConsign( unsigned long id );
    void DeleteMedicalConsign( unsigned long id );

    void CreateSupplyConsign( const ASN1T_MsgLogRavitaillementTraitementCreation& asnMsg );
    LogSupplyConsign& GetSupplyConsign( unsigned long id );
    void DeleteSupplyConsign( unsigned long id );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticsModel( const LogisticsModel& );            //!< Copy constructor
    LogisticsModel& operator=( const LogisticsModel& ); //!< Assignement operator
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
