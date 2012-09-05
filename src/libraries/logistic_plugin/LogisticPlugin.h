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

class QApplication;

namespace tools
{
    class SessionConfig;
}

namespace xml
{
    class xistream;
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
             LogisticPlugin(const boost::shared_ptr<const NameResolver_ABC>& nameResolver,
                 const tools::SessionConfig& config, xml::xistream& xis );
    virtual ~LogisticPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );
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
    int currentTick_;
    std::string simTime_;
    const tools::SessionConfig& sessionConfig_;
    boost::scoped_ptr< ConsignResolver_ABC >    maintenanceResolver_;
    boost::scoped_ptr< ConsignResolver_ABC >    supplyResolver_;
    boost::scoped_ptr< ConsignResolver_ABC >    funeralResolver_;
    boost::scoped_ptr< ConsignResolver_ABC >    medicalResolver_;
    boost::scoped_ptr< QApplication >           localAppli_;
    boost::shared_ptr<const NameResolver_ABC> nameResolver_;
    //@}
};
}
}

#endif // __LogisticPlugin_h_
