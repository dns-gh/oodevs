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

#include "IndicatorDefinition_ABC.h"
#include "tools/Resolver.h"

namespace xml
{
    class xistream;
}

class AfterActionParameter;

// =============================================================================
/** @class  AfterActionFunction
    @brief  AfterActionFunction
*/
// Created: AGE 2007-09-20
// =============================================================================
class AfterActionFunction : public IndicatorDefinition_ABC
                          , public tools::Resolver< AfterActionParameter, std::string >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AfterActionFunction( xml::xistream& xis );
    virtual ~AfterActionFunction();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    QString GetComments() const;
    virtual std::string Commit( const T_Parameters& parameters ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    void ReadDescription( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    QString name_;
    const std::string base_;
    QString comments_;
    //@}
};

#endif // __AfterActionFunction_h_
