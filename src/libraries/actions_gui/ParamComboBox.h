// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamComboBox_h_
#define __ParamComboBox_h_

#include "Param_ABC.h"
#include "actions/Enumeration.h"
#include "actions/Action_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/ValuedComboBox.h"

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
    explicit ParamComboBox( const kernel::OrderParameter& parameter );
    virtual ~ParamComboBox();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( QWidget* parent );
    void AddItem( const QString& name, T value );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    T GetValue() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< std::pair< QString, T > > T_Values;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    T_Values values_;
    ::gui::ValuedComboBox<T>* comboBox_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ParamComboBox constructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< typename T >
ParamComboBox<T>::ParamComboBox( const kernel::OrderParameter& parameter )
    : Param_ABC( parameter.GetName().c_str(), parameter.IsOptional() )
    , parameter_( parameter )
    , comboBox_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox destructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< typename T >
ParamComboBox<T>::~ParamComboBox()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
template< typename T >
QWidget* ParamComboBox<T>::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );

    comboBox_ = new ::gui::ValuedComboBox<T>( parent );
    comboBox_->setSorting( true );
    for( T_Values::const_iterator it = values_.begin(); it != values_.end(); ++it )
        comboBox_->AddItem( it->first, it->second );
    layout->addWidget( comboBox_ );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::AddItem
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< typename T >
void ParamComboBox<T>::AddItem( const QString& name, T value )
{
    values_.push_back( std::make_pair( name, value ) );
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename T >
void ParamComboBox<T>::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() )
        action.AddParameter( *new actions::parameters::Enumeration( parameter_, GetValue() ) );
    else
        action.AddParameter( *new actions::parameters::Enumeration( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::GetValue
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
template< typename T >
T ParamComboBox<T>::GetValue() const
{
    return ( comboBox_ && comboBox_->count() ) ? comboBox_->GetValue() : 0;
}

    }
}

#endif // __ParamComboBox_h_
