// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ParamComboBox_h_
#define __ParamComboBox_h_

#include "Param_ABC.h"
#include "actions/Action_ABC.h"
#include "actions/Enumeration.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/OrderParameterValue.h"
#include "clients_gui/ValuedComboBox.h"
#include "InterfaceBuilder_ABC.h"

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamComboBox
    @brief  ParamComboBox
*/
// Created: APE 2004-04-21
// =============================================================================
template< typename T >
class ParamComboBox : public Param_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamComboBox( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamComboBox();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    void AddItem( const QString& name, T value );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    T GetValue() const;
    void Clear();
    //@}

protected:
    //! @name Types
    //@{
    typedef std::vector< std::pair< QString, T > > T_Values;
    //@}

protected:
    //! @name Member data
    //@{
    T_Values                    values_;
    ::gui::ValuedComboBox< T >* comboBox_;
    //@}
};

#include "ParamComboBox.inl"

    }
}

#endif // __ParamComboBox_h_
