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
    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    //@}

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
    : Param_ABC( parameter.GetName().c_str() )
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
    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    QLabel* label = new QLabel( GetName(), box );
    label->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    comboBox_ = new ::gui::ValuedComboBox<T>( box );
    comboBox_->setSorting( true );
    for( T_Values::const_iterator it = values_.begin(); it != values_.end(); ++it )
        comboBox_->AddItem( it->first, it->second );
    box->setStretchFactor( comboBox_, 1 );
    return box;
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
    action.AddParameter( *new actions::parameters::Enumeration( parameter_, GetValue() ) );
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

// -----------------------------------------------------------------------------
// Name: ParamComboBox::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
template< typename T >
bool ParamComboBox<T>::IsOptional() const
{
    return parameter_.IsOptional();
}

    }
}

#endif // __ParamComboBox_h_
