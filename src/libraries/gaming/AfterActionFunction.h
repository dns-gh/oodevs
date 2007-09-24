// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionFunction_h_
#define __AfterActionFunction_h_

#include "clients_kernel/Resolver.h"
class AfterActionFactory;
class AfterActionItem_ABC;
class AfterActionParameter;

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  AfterActionFunction
    @brief  AfterActionFunction
*/
// Created: AGE 2007-09-20
// =============================================================================
class AfterActionFunction : public kernel::Resolver< AfterActionItem_ABC, std::string >
                          , public kernel::Resolver< AfterActionParameter, std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionFunction( kernel::Resolver_ABC< AfterActionFactory, QString >& factories, xml::xistream& xis );
    virtual ~AfterActionFunction();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    std::string Commit() const;

    kernel::Iterator< const AfterActionParameter& > CreateParameterIterator() const;
    kernel::Iterator< const AfterActionItem_ABC& >  CreateItemIterator() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionFunction( const AfterActionFunction& );            //!< Copy constructor
    AfterActionFunction& operator=( const AfterActionFunction& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    void CreateItem( const std::string& type, xml::xistream& xis, kernel::Resolver_ABC< AfterActionFactory, QString >& factories );
    void LoadItem  ( const std::string& type, xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef kernel::Resolver< AfterActionItem_ABC,  std::string > ItemResolver;
    typedef kernel::Resolver< AfterActionParameter, std::string > ParameterResolver;
    //@}

private:
    //! @name Member data
    //@{
    const std::string name_;
    //@}
};

#endif // __AfterActionFunction_h_
