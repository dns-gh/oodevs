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
class AfterActionFunction : public kernel::Resolver< AfterActionParameter, std::string >
{

public:
    //! @name Types
    //@{
    typedef std::map< std::string, std::string > T_Parameters;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit AfterActionFunction( xml::xistream& xis );
    virtual ~AfterActionFunction();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    QString GetComments() const;
    std::string Commit( const T_Parameters& parameters ) const;
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
    //@}

private:
    //! @name Member data
    //@{
    const QString name_;
    const std::string base_;
    QString comments_;
    //@}
};

#endif // __AfterActionFunction_h_
