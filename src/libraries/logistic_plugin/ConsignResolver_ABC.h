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
#include <boost/date_time/gregorian/gregorian.hpp>

namespace sword
{
    class SimToClient;
}

namespace dispatcher
{
    class Model_ABC;
}

namespace plugins
{
namespace logistic
{

// =============================================================================
/** @class  ConsignResolver_ABC
    @brief  ConsignResolver_ABC
*/
// Created: MMC 2012-08-06
// =============================================================================
class ConsignResolver_ABC
{
protected:
    //! @name Constructors
    //@{
    ConsignResolver_ABC( const std::string& name, const std::string& header, const dispatcher::Model_ABC& model );
    //@}

public:
    //! @name Constructors
    //@{
    virtual ~ConsignResolver_ABC();
    //@}

    //! @name Operations
    //@{
    void Receive( const sword::SimToClient& message );
    void SetTime( int tick, const std::string& simTime );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ConsignResolver_ABC( const ConsignResolver_ABC& );            //!< Copy constructor
    ConsignResolver_ABC& operator=( const ConsignResolver_ABC& ); //!< Assignment operator
    //@}

protected:

    //! @name Operations
    //@{
    virtual bool IsManageable( const sword::SimToClient& ) { return false; }
    virtual void ManageMessage( const sword::SimToClient& ) {}
    
    template < typename T > T* GetConsign( int requestId )
    {
        std::map< int, ConsignData_ABC* >::iterator it = consignsData_.find( requestId );
        if( it == consignsData_.end() )
        {
            T* pConsign = new T;
            consignsData_[ requestId ] = static_cast< ConsignData_ABC* >( pConsign );
            return pConsign;
        }
        return static_cast< T* >( it->second );
    }
    //@}

    //! @name Helpers
    //@{
    void AppendDateWithExtension( std::string& fileName, const boost::gregorian::date& d );
    void CheckOutputFile();
    void GetAgentName( int id, std::string& name );
    void GetAutomatName( int id, std::string& name );
    void GetFormationName( int id, std::string& name );
    //@}

    //! @name Member data
    //@{
    const dispatcher::Model_ABC& model_;
    const std::string header_;
    boost::gregorian::date fileDate_;
    int curTick_;
    std::string simTime_;
    std::string name_;
    std::string fileName_;
    std::ofstream output_;
    boost::gregorian::date fileDateForMedical_;
    std::map< int, ConsignData_ABC* > consignsData_;
    //@}
};
}
}

#endif // __ConsignResolver_ABC_h_
