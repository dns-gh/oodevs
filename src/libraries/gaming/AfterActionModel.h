// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionModel_h_
#define __AfterActionModel_h_

#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

namespace sword
{
    class AarInformation;
    class PlotResult;
}

namespace kernel
{
    class Controller;
}

namespace tools
{
    class Loader_ABC;
}

namespace xml
{
    class xistream;
}

class AfterActionFunction;
class IndicatorDefinition_ABC;
class IndicatorRequest;
class IndicatorRequests;
class Publisher_ABC;

// =============================================================================
/** @class  AfterActionModel
    @brief  AfterActionModel
*/
// Created: AGE 2007-09-17
// =============================================================================
class AfterActionModel : private tools::Resolver< AfterActionFunction, QString >
                       , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AfterActionModel( kernel::Controller& controller, Publisher_ABC& publisher );
    virtual ~AfterActionModel();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::AarInformation& message );
    void Update( const sword::PlotResult& message );
    void Purge();
    IndicatorRequest& CreateRequest( const AfterActionFunction& function, const QString& name );
    const IndicatorDefinition_ABC* FindDefinition( const std::string& definition ) const;
    tools::Iterator< const AfterActionFunction& > CreateIterator() const;
    void SaveRequests( const tools::Path& path ) const;
    void LoadRequests( const tools::Loader_ABC& loader, const tools::Path& path );
    //@}

private:
    //! @name Helpers
    //@{
    void Load( const tools::Path& functions );
    void ReadFunction( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    std::unique_ptr< IndicatorRequests > requests_;
    //@}
};

#endif // __AfterActionModel_h_
