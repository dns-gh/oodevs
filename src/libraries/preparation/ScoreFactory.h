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

namespace kernel
{
    class Controller;
}

class IndicatorPrimitives;

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
             ScoreFactory( kernel::Controller& controller, const IndicatorPrimitives& indicators );
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

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const IndicatorPrimitives& indicators_;
    //@}
};

#endif // __ScoreFactory_h_
