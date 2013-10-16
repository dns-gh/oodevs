// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorActions_h_
#define __SuccessFactorActions_h_

#include "SuccessFactorAction.h"
#include <tools/Resolver.h>

namespace xml
{
    class xistream;
    class xostream;
}

class SuccessFactorAction;
class SuccessFactorActionTypes;

// =============================================================================
/** @class  SuccessFactorActions
    @brief  SuccessFactorActions
*/
// Created: SBO 2009-06-15
// =============================================================================
class SuccessFactorActions : public tools::Resolver< SuccessFactorAction >
{
public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorActions();
             SuccessFactorActions( xml::xistream& xis, const SuccessFactorActionTypes& actionsTypes );
    virtual ~SuccessFactorActions();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    void AddAction( SuccessFactorAction& action );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorActions( const SuccessFactorActions& );            //!< Copy constructor
    SuccessFactorActions& operator=( const SuccessFactorActions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAction( xml::xistream& xis, const SuccessFactorActionTypes& actionsTypes );
    //@}
};

#endif // __SuccessFactorActions_h_
