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

#include <tools/Resolver.h>

namespace sword
{
    class LogMaintenanceHandlingCreation;
    class LogMedicalHandlingCreation;
    class LogSupplyHandlingCreation;
    class LogFuneralHandlingCreation;
    class LogMaintenanceHandlingUpdate;
    class LogMedicalHandlingUpdate;
    class LogFuneralHandlingUpdate;
    class LogSupplyHandlingUpdate;
    class SupplyRecipientResourceRequests;
    class ParentEntity;
    class LogisticHistoryAck;
    class LogHistoryEntry;
}

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Formation_ABC;
    class DotationType;
    class Controller;
    class EntityResolver_ABC;
}

namespace google
{
    namespace protobuf
    {
        template< typename T > class RepeatedPtrField;
    }
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
             LogisticsModel( LogisticConsignFactory_ABC& factory, const kernel::EntityResolver_ABC& resolver,
                             const tools::Resolver_ABC< kernel::DotationType >& dotationResolver,
                             kernel::Controller& controller );
    virtual ~LogisticsModel();
    //@}

    //! @name Operations
    //@{
    virtual void Purge();

    virtual void CreateMaintenanceConsign( const sword::LogMaintenanceHandlingCreation& message );
    virtual void UpdateMaintenanceConsign( const sword::LogMaintenanceHandlingUpdate& message );
    void DeleteMaintenanceConsign( unsigned long id );

    virtual void CreateMedicalConsign( const sword::LogMedicalHandlingCreation& message );
    virtual void UpdateMedicalConsign( const sword::LogMedicalHandlingUpdate& message );
    void DeleteMedicalConsign( unsigned long id );

    virtual void CreateSupplyConsign( const sword::LogSupplyHandlingCreation& message );
    virtual void UpdateSupplyConsign( const sword::LogSupplyHandlingUpdate& message );
    void DeleteSupplyConsign( unsigned long id );

    virtual void CreateFuneralConsign( const sword::LogFuneralHandlingCreation& message );
    virtual void UpdateFuneralConsign( const sword::LogFuneralHandlingUpdate& message );
    void DeleteFuneralConsign( unsigned long id );

    virtual kernel::Entity_ABC* FindLogEntity( const sword::ParentEntity& msg );
    virtual void UpdateLogisticHistory( const sword::LogisticHistoryAck& message, unsigned int currentTick );
    void DestroyAgent( unsigned long id );
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
    template< typename M, typename C, typename E >
    void CreateConsign( const M& message, const std::function< C*( const M&, kernel::Agent_ABC& ) >& create );
    template< typename C, typename E >
    void DeleteConsign( unsigned long id );
    template< typename M, typename C, typename E >
    void UpdateConsign( const M& message );
    template< typename Type >
    void UpdateConsign( unsigned long id, int start, int end,
        const google::protobuf::RepeatedPtrField< sword::LogHistoryEntry >& states,
        unsigned int currentTick );
    void UpdateLogisticHistory( int start, int end,
        const google::protobuf::RepeatedPtrField< sword::LogHistoryEntry >& states,
        unsigned int currentTick );
    //@}

protected:
    //! @name Member data
    //@{
    LogisticConsignFactory_ABC& factory_;
    const kernel::EntityResolver_ABC& resolver_;
    const tools::Resolver_ABC< kernel::DotationType >& dotationResolver_;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

#endif // __LogisticsModel_h_
