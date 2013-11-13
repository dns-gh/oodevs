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
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#pragma warning( push, 0 )
#include <boost/ptr_container/ptr_map.hpp>
#pragma warning( pop )
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

namespace tools
{
    class Path;
}

namespace plugins
{
namespace logistic
{

class ConsignData_ABC;
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
             LogisticPlugin(const boost::shared_ptr<const NameResolver_ABC>& nameResolver,
                 const tools::Path& maintenanceFile, const tools::Path& supplyFile,
                 const tools::Path& funeralFile, const tools::Path& medicalFile );
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
    virtual int DebugGetConsignCount( E_LogisticType eLogisticType ) const;
    virtual void SetMaxLinesInFile( int maxLines );
    //@}

private:
    //! @name Member data
    //@{
    boost::ptr_map< E_LogisticType, ConsignResolver_ABC > resolvers_;
    boost::scoped_ptr< QApplication >           localAppli_;
    boost::shared_ptr<const NameResolver_ABC>   nameResolver_;
    boost::ptr_map< int, ConsignData_ABC >      consigns_;
    int currentTick_;
    std::string simTime_;
    //@}
};

LogisticPlugin* CreateLogisticPlugin(
    const boost::shared_ptr<const NameResolver_ABC>& nameResolver,
    const tools::SessionConfig& config, xml::xistream& xis );

}
}

#endif // __LogisticPlugin_h_
