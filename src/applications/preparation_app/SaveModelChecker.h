// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SaveModelChecker_h_
#define __SaveModelChecker_h_

#include "preparation/ModelChecker_ABC.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  SaveModelChecker
    @brief  SaveModelChecker
*/
// Created: JSR 2011-07-27
// =============================================================================
class SaveModelChecker : public ModelChecker_ABC
                       , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SaveModelChecker( QMainWindow* window );
    virtual ~SaveModelChecker();
    //@}

    //! @name Operations
    //@{
    virtual bool Validate();
    virtual bool Reject( const QString& reason );
    virtual bool Prompt( const QString& question );
    //@}
private:
    //! @name Member data
    //@{
    QMainWindow* window_;
    //@}
};

#endif // __SaveModelChecker_h_
