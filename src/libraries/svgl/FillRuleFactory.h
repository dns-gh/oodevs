// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FillRuleFactory_h_
#define __FillRuleFactory_h_

#include "PropertyFactory_ABC.h"
#include "FillRule.h"

namespace svg
{

// =============================================================================
/** @class  FillRuleFactory
    @brief  FillRuleFactory
*/
// Created: AGE 2006-08-23
// =============================================================================
class FillRuleFactory : public PropertyFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             FillRuleFactory();
    virtual ~FillRuleFactory();
    //@}

    //! @name Operations
    //@{
    virtual FillRule* CreateProperty( const std::string& color ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FillRuleFactory( const FillRuleFactory& );            //!< Copy constructor
    FillRuleFactory& operator=( const FillRuleFactory& ); //!< Assignement operator
    //@}
};

}

#endif // __FillRuleFactory_h_
