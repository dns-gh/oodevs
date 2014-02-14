// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HistoryLogisticsModel_h_
#define __HistoryLogisticsModel_h_

#include "LogisticsModel.h"

namespace sword
{
    class ListLogisticRequestsAck;
}

// =============================================================================
/** @class  HistoryLogisticsModel
    @brief  HistoryLogisticsModel
*/
// Created: LGY 2013-12-13
// =============================================================================
class HistoryLogisticsModel : public LogisticsModel
{
public:
    //! @name Constructors/Destructor
    //@{
             HistoryLogisticsModel( LogisticConsignFactory_ABC& factory, const kernel::EntityResolver_ABC& resolver,
                                    const tools::Resolver_ABC< kernel::DotationType >& dotationResolver, kernel::Controller& controller );
    virtual ~HistoryLogisticsModel();
    //@}

    //! @name Operations
    //@{
    void Fill( const sword::ListLogisticRequestsAck& message );
    void Fill( const sword::LogisticHistoryAck& message );

    virtual void CreateMaintenanceConsign( const sword::LogMaintenanceHandlingCreation& message );
    virtual void UpdateMaintenanceConsign( const sword::LogMaintenanceHandlingUpdate& message );
    virtual void CreateMedicalConsign( const sword::LogMedicalHandlingCreation& message );
    virtual void UpdateMedicalConsign( const sword::LogMedicalHandlingUpdate& message );
    virtual void CreateSupplyConsign( const sword::LogSupplyHandlingCreation& message );
    virtual void UpdateSupplyConsign( const sword::LogSupplyHandlingUpdate& message );
    virtual void CreateFuneralConsign( const sword::LogFuneralHandlingCreation& message );
    virtual void UpdateFuneralConsign( const sword::LogFuneralHandlingUpdate& message );
    //@}

private:
    //! @name Helpers
    //@{
    template< typename Type >
    void DeleteConsigns( const std::set< unsigned int >& newRequests );
    //@}
};

#endif // __HistoryLogisticsModel_h_
