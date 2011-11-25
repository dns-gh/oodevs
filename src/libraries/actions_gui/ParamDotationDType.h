// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamDotationDType_h_
#define __ParamDotationDType_h_

#include "ParamComboBox.h"
#include "tools/Resolver.h"

namespace sword
{
    class DotationType;
}

namespace kernel
{
    class DotationType;
    class OrderParameter;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamDotationDType
    @brief  ParamDotationDType
    // $$$$ SBO 2007-03-21: ParamComboBox< const DotationType* > ?
    // $$$$ AGE 2007-10-23: pas utilisé
*/
// Created: SBO 2006-08-09
// =============================================================================
class ParamDotationDType : public ParamComboBox< int /*sword::DotationType*/ >
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamDotationDType( const kernel::OrderParameter& parameter, const tools::Resolver_ABC< kernel::DotationType >& resolver, const kernel::Entity_ABC& agent );
    virtual ~ParamDotationDType();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool InternalCheckValidity() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    const tools::Resolver_ABC< kernel::DotationType >& resolver_;
    const kernel::Entity_ABC& agent_;
    //@}
};

    }
}

#endif // __ParamDotationDType_h_
