// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ParamFireClass_h_
#define __ParamFireClass_h_

#include "ParamComboBox.h"

namespace kernel
{
    class FireClass;
    class OrderParameter;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamFireClass
    @brief  ParamFireClass
*/
// Created: LGY 2013-11-26
// =============================================================================
class ParamFireClass : public ParamComboBox< int >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamFireClass( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamFireClass();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void Visit( const actions::parameters::FireClass& param );
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool InternalCheckValidity() const;
    //@}

private:
    //! @name Member data
    //@{
    const tools::StringResolver< kernel::FireClass >& resolver_;
    std::map< int, std::string > internalId_;
    //@}
};

}
}

#endif // __ParamFireClass_h_
