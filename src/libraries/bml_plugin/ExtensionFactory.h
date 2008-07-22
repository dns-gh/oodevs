// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExtensionFactory_h_
#define __ExtensionFactory_h_

#include "dispatcher/ExtensionFactory_ABC.h"

namespace dispatcher
{
    class Agent;
    class Automat;
    class Model;
}

namespace bml
{
    class Publisher_ABC;
    class ReportFactory;
    class Simulation;

// =============================================================================
/** @class  ExtensionFactory
    @brief  ExtensionFactory
*/
// Created: SBO 2008-02-29
// =============================================================================
class ExtensionFactory : public dispatcher::ExtensionFactory_ABC< dispatcher::Agent >
                       , public dispatcher::ExtensionFactory_ABC< dispatcher::Automat >
{

public:
    //! @name Constructors/Destructor
    //@{
             ExtensionFactory( Publisher_ABC& publisher, const ReportFactory& reportFactory, const Simulation& simulation, const dispatcher::Model& model );
    virtual ~ExtensionFactory();
    //@}

    //! @name Operations
    //@{
    virtual void Create( dispatcher::Agent& entity );
    virtual void Create( dispatcher::Automat& entity );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExtensionFactory( const ExtensionFactory& );            //!< Copy constructor
    ExtensionFactory& operator=( const ExtensionFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    const ReportFactory& reportFactory_;
    const Simulation& simulation_;
    const dispatcher::Model& model_;
    //@}
};

}

#endif // __ExtensionFactory_h_
