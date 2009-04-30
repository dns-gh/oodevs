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

#include "clients_kernel/Resolver.h"

namespace indicators
{
    class Primitives;
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
class ScoreDefinitions : public kernel::Resolver< ScoreDefinition, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ScoreDefinitions( const indicators::Primitives& indicators );
    virtual ~ScoreDefinitions();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& file );
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
    //@}
};

#endif // __ScoreDefinitions_h_
