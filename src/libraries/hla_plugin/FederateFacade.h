// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_FederateFacade_h
#define plugins_hla_FederateFacade_h

#include <boost/noncopyable.hpp>

namespace xml
{
    class xisubstream;
}

namespace hla
{
    class TimeFactory_ABC;
    class TimeIntervalFactory_ABC;
    class RtiAmbassador_ABC;
    class Federate;
}

namespace dispatcher
{
    class Model_ABC;
}

namespace plugins
{
namespace hla
{
    class AggregateEntityClass;
    class ExtensionFactory;

// =============================================================================
/** @class  FederateFacade
    @brief  Federate facade
*/
// Created: SBO 2008-02-18
// =============================================================================
class FederateFacade : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FederateFacade( xml::xisubstream xis, dispatcher::Model_ABC& model, unsigned int lookAhead );
    virtual ~FederateFacade();
    //@}

    //! @name Operations
    //@{
    void Step();
    //@}

private:
    //! @name Member data
    //@{
    bool joined_;
    dispatcher::Model_ABC& model_;
    std::auto_ptr< AggregateEntityClass > agentClass_;
    std::auto_ptr< ExtensionFactory > factory_;
    std::auto_ptr< ::hla::TimeFactory_ABC > timeFactory_;
    std::auto_ptr< ::hla::TimeIntervalFactory_ABC > intervalFactory_;
    std::auto_ptr< ::hla::RtiAmbassador_ABC > ambassador_;
    std::auto_ptr< ::hla::Federate > federate_;
    //@}
};

}
}

#endif // plugins_hla_FederateFacade_h
