// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorFactory_ABC_h_
#define __SuccessFactorFactory_ABC_h_

namespace xml
{
    class xistream;
}

class SuccessFactor;

// =============================================================================
/** @class  SuccessFactorFactory_ABC
    @brief  SuccessFactorFactory_ABC
*/
// Created: SBO 2009-06-15
// =============================================================================
class SuccessFactorFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorFactory_ABC() {}
    virtual ~SuccessFactorFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual SuccessFactor* Create( xml::xistream& xis ) const = 0;
    virtual SuccessFactor* Create( const QString& name ) const = 0;
    //@}
};

#endif // __SuccessFactorFactory_ABC_h_
