// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreDefinitions_h_
#define __ScoreDefinitions_h_

#include "protocol/Aar.h"
#include <tools/Resolver.h>

namespace indicators
{
    class Primitives;
    class GaugeFactory_ABC;
}

namespace xml
{
    class xistream;
}

class ScoreDefinition;

// =============================================================================
/** @class  ScoreDefinitions
    @brief  ScoreDefinitions
*/
// Created: SBO 2009-04-29
// =============================================================================
class ScoreDefinitions : public tools::Resolver< ScoreDefinition, QString >
{
public:
    //! @name Constructors/Destructor
    //@{
             ScoreDefinitions( const indicators::Primitives& indicators, const indicators::GaugeFactory_ABC& factory );
    virtual ~ScoreDefinitions();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::AarInformation& asnMsg );
    void Purge();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoreDefinitions( const ScoreDefinitions& );            //!< Copy constructor
    ScoreDefinitions& operator=( const ScoreDefinitions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadDefinition( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const indicators::Primitives& primitives_;
    const indicators::GaugeFactory_ABC& gaugeFactory_;
    //@}
};

#endif // __ScoreDefinitions_h_
