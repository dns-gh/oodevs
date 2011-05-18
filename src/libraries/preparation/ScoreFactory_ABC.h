// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreFactory_ABC_h_
#define __ScoreFactory_ABC_h_

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

class Score_ABC;

// =============================================================================
/** @class  ScoreFactory_ABC
    @brief  ScoreFactory_ABC
*/
// Created: SBO 2009-04-16
// =============================================================================
class ScoreFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ScoreFactory_ABC() {}
    virtual ~ScoreFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual Score_ABC* CreateScore( xml::xistream& xis ) const = 0;
    virtual Score_ABC* CreateScore( const QString& name ) const = 0;
    //@}
};

#endif // __ScoreFactory_ABC_h_
