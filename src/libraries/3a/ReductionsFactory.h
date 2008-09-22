// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReductionsFactory_h_
#define __ReductionsFactory_h_

#include "ElementFactory_ABC.h"

// =============================================================================
/** @class  ReductionsFactory
    @brief  ReductionsFactory
*/
// Created: AGE 2008-08-04
// =============================================================================
class ReductionsFactory : public ElementFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ReductionsFactory();
    virtual ~ReductionsFactory();
    //@}

    //! @name Operations
    //@{
    virtual void CreateElement( const std::string& type, xml::xistream& xis, Task& result ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReductionsFactory( const ReductionsFactory& );            //!< Copy constructor
    ReductionsFactory& operator=( const ReductionsFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void Reduce( const std::string& name, xml::xistream& xis, Task& result ) const;
    template< typename F >
    void ReduceFunction( const std::string& name, xml::xistream& xis, Task& result ) const;
    struct ReduceDispatcher;
    static void Error( const std::string& name );
    //@}
};

#endif // __ReductionsFactory_h_
