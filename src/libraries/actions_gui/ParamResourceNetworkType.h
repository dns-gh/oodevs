// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ParamResourceNetworkType_h_
#define __ParamResourceNetworkType_h_

#include "ParamComboBox.h"

namespace kernel
{
    class ResourceNetworkType;
    class OrderParameter;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamResourceNetworkType
    @brief  ParamResourceNetworkType
*/
// Created: ABR 2012-02-15
// =============================================================================
class ParamResourceNetworkType : public ParamComboBox< int >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamResourceNetworkType( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamResourceNetworkType();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void Visit( const actions::parameters::ResourceNetworkType& param );
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool InternalCheckValidity() const;
    //@}

private:
    //! @name Member data
    //@{
    const tools::StringResolver< kernel::ResourceNetworkType >& resolver_;
    std::map< int, std::string > internalId_;
    //@}
};

}
}

#endif // __ParamResourceNetworkType_h_
