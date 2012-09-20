// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ConsignResolver_ABC_h_
#define __ConsignResolver_ABC_h_

#include "ConsignData_ABC.h"
#include "protocol/Protocol.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/noncopyable.hpp>


namespace sword
{
    class SimToClient;
}

namespace dispatcher
{
    class Model_ABC;
}

namespace kernel
{
    class StaticModel;
}

namespace plugins
{
namespace logistic
{

// =============================================================================
/** @class  ConsignResolver_ABC
    @brief  ConsignResolver_ABC
*/
// Treats specialized logistic messages and traces and keeps consigns data
// Created: MMC 2012-08-06
// =============================================================================
class ConsignResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
    ConsignResolver_ABC( const std::string& name, const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel );
    virtual ~ConsignResolver_ABC();
    //@}

    //! @name Operations
    //@{
    void Receive( const sword::SimToClient& message );
    void SetTime( int tick, const std::string& simTime );
    void GetSimTime( std::string& simTime, std::string& tick ) const;
    virtual void InitHeader() = 0;
    virtual void AddToLineIndex( int number ) { curLineIndex_ += number; }
    //@}

    //! @name Helpers
    //@{
    void GetAgentName( int id, std::string& name ) const;
    void GetAutomatName( int id, std::string& name ) const;
    void GetFormationName( int id, std::string& name ) const;
    void GetSupplykName( const sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus& eSupply, std::string& name ) const;
    void GetRankName( const sword::EnumHumanRank& eRank, std::string& name ) const;
    void GetWoundName( const sword::EnumHumanWound& eWound, std::string& name ) const;
    void GetMedicalName( const sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus& eMedical, std::string& name ) const;
    void GetMaintenanceName( const sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus& eMaintenance, std::string& name ) const;
    void GetFuneralName( const sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus& eFuneral, std::string& name ) const;
    void GetEquipmentName( const sword::EquipmentType& equipmentType, std::string& name ) const;
    void GetBreakdownName( const sword::BreakdownType& breakdownType, std::string& name ) const;
    void GetResourceName( const sword::ResourceType& resourceType, std::string& name ) const;
    //@}

protected:

    //! @name Operations
    //@{
    virtual bool IsManageable( const sword::SimToClient& ) { return false; }
    virtual void ManageMessage( const sword::SimToClient& ) {}
    ConsignData_ABC& GetConsign( int requestId );
    virtual ConsignData_ABC* CreateConsignData( int requestId ) = 0;
    virtual void DestroyConsignData( int requestId );

    template < typename M, typename T >
    void TraceConsign( const M& msg, std::ofstream& output )
    {
        if( msg.has_request() )
        {
            static_cast< T& >( GetConsign( static_cast< int >( msg.request().id() ) ) ).ManageMessage( msg, *this ) >> output;
            output.flush();
        }
    }
    //@}

    //! @name Helpers
    //@{
    void AppendDateWithExtension( std::string& fileName, const boost::gregorian::date& d, int fileIndex );
    void CheckOutputFile();
    void SetNewFile();
    void RemoveOldFiles();
    void OpenFile();
    void SetHeader( const ConsignData_ABC& consign );
    //@}

    //! @name Member data
    //@{
    const dispatcher::Model_ABC& model_;
    const kernel::StaticModel& staticModel_;
    std::string header_;
    boost::gregorian::date fileDate_;
    int curTick_;
    std::string simTime_;
    const std::string name_;
    std::string fileName_;
    std::ofstream output_;
    boost::gregorian::date fileDateForMedical_;
    std::map< int, ConsignData_ABC* > consignsData_;
    int curLineIndex_;
    //@}
};
}
}

#endif // __ConsignResolver_ABC_h_
