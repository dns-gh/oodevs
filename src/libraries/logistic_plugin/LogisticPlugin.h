// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __LogisticPlugin_h_
#define __LogisticPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "ConsignResolver_ABC.h"
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <vector>

class QApplication;
namespace tools
{
    class SessionConfig;
}

namespace xml
{
    class xistream;
}

namespace boost
{
    namespace gregorian
    {
        class date;
    }
}

namespace plugins
{
namespace logistic
{

class ConsignResolver_ABC;
class NameResolver_ABC;

// =============================================================================
/** @class  LogisticPlugin
    @brief  LogisticPlugin
*/
// Created: MMC 2012-08-06
// =============================================================================
class LogisticPlugin : public dispatcher::Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticPlugin( const boost::shared_ptr<const NameResolver_ABC>& nameResolver,
                 const std::string& maintenanceFile, const std::string& supplyFile,
                 const std::string& funeralFile, const std::string& medicalFile,
                 int maxHistoricFiles, int maxFileLines );
    virtual ~LogisticPlugin();
    //@}

    //! @name Types
    //@{
    enum E_LogisticType
    {
        eLogisticType_Maintenance,
        eLogisticType_Supply,
        eLogisticType_Funeral,
        eLogisticType_Medical,
        eNbrLogisticType
    };
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );
    virtual void Receive( const sword::SimToClient& message, const boost::gregorian::date& today );
    virtual int GetConsignCount( E_LogisticType eLogisticType ) const;
    virtual void SetMaxLinesInFile( int maxLines );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticPlugin( const LogisticPlugin& );            //!< Copy constructor
    LogisticPlugin& operator=( const LogisticPlugin& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{    
    std::vector< ConsignResolver_ABC* >         resolvers_;
    boost::scoped_ptr< QApplication >           localAppli_;
    boost::shared_ptr<const NameResolver_ABC>   nameResolver_;
    //@}
};

LogisticPlugin* CreateLogisticPlugin( const boost::shared_ptr<const NameResolver_ABC>& nameResolver, const tools::SessionConfig& config, int maxHistoricFiles, int maxFileLines );

}
}

#endif // __LogisticPlugin_h_
