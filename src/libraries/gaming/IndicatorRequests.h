// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IndicatorRequests_h_
#define __IndicatorRequests_h_

#include "tools/Iterator.h"
#include "protocol/AarSenders.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controller;
}

namespace tools
{
    class Loader_ABC;
}

class AfterActionModel;
class IndicatorDefinition_ABC;
class IndicatorRequest;
class Publisher_ABC;

// =============================================================================
/** @class  IndicatorRequests
    @brief  IndicatorRequests
*/
// Created: AGE 2007-09-25
// =============================================================================
class IndicatorRequests : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             IndicatorRequests( kernel::Controller& controller, Publisher_ABC& publisher );
    virtual ~IndicatorRequests();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Update( const sword::PlotResult& message );
    void Save( const tools::Path& path ) const;
    void Load( const tools::Loader_ABC& loader, const tools::Path& path, const AfterActionModel& model );

    IndicatorRequest& CreateRequest( const IndicatorDefinition_ABC& definition, const QString& name = QString() );
    tools::Iterator< const IndicatorRequest& > CreateIterator();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< IndicatorRequest* > T_Requests;
    //@}

    //! @name Helpers
    //@{
    void LoadRequests( xml::xistream& xis, const AfterActionModel& model );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Publisher_ABC& publisher_;
    T_Requests requests_;
    //@}
};

#endif // __IndicatorRequests_h_
