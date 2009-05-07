// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreFactory_h_
#define __ScoreFactory_h_

#include "ScoreFactory_ABC.h"

namespace indicators
{
    class GaugeFactory_ABC;
    class Primitives;
}

namespace kernel
{
    class Controller;
}

// =============================================================================
/** @class  ScoreFactory
    @brief  ScoreFactory
*/
// Created: SBO 2009-04-16
// =============================================================================
class ScoreFactory : public ScoreFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ScoreFactory( kernel::Controller& controller, const indicators::Primitives& indicators, const indicators::GaugeFactory_ABC& gaugeFactory );
    virtual ~ScoreFactory();
    //@}

    //! @name Operations
    //@{
    virtual Score_ABC* CreateScore( xml::xistream& xis ) const;
    virtual Score_ABC* CreateScore( const QString& name ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoreFactory( const ScoreFactory& );            //!< Copy constructor
    ScoreFactory& operator=( const ScoreFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const indicators::Primitives& indicators_;
    const indicators::GaugeFactory_ABC& gaugeFactory_;
    //@}
};

#endif // __ScoreFactory_h_
