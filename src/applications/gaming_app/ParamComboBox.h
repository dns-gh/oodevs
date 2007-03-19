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
#include "clients_gui/ValuedComboBox.h"
#include "gaming/ActionParameter.h"
#include "gaming/Action_ABC.h"

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
    explicit ParamComboBox( const QString& name );
    virtual ~ParamComboBox();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    void AddItem( const QString& name, const T& value );
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void CommitTo( Action_ABC& action ) const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_OID& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{    
    ParamComboBox( const ParamComboBox& );
    ParamComboBox& operator=( const ParamComboBox& );
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::pair< QString, const T* > > T_Values;
    //@}

private:
    //! @name Member data
    //@{
    T_Values values_;
    gui::ValuedComboBox<T>* comboBox_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ParamComboBox constructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< typename T >
ParamComboBox<T>::ParamComboBox( const QString& name )
    : Param_ABC( name )
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
void ParamComboBox<T>::BuildInterface( QWidget* parent )
{
    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    QLabel* label = new QLabel( GetName(), box );
    label->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    comboBox_ = new gui::ValuedComboBox<T>( box );
    comboBox_->setSorting( true );
    for( T_Values::const_iterator it = values_.begin(); it != values_.end(); ++it )
        comboBox_->AddItem( it->first, *it->second );
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::AddItem
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< typename T >
void ParamComboBox<T>::AddItem( const QString& name, const T& value )
{
    values_.push_back( std::make_pair( name, &value ) );
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
template< typename T >
void ParamComboBox<T>::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = 0;
    asn.value.t = T_MissionParameter_value_enumeration;
    asn.value.u.enumeration = comboBox_->GetValue();
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
template< typename T >
void ParamComboBox<T>::CommitTo( ASN1T_OID& asn ) const
{
    if( !comboBox_ )
        InterfaceNotInitialized();
    if( comboBox_->count() )
        asn = comboBox_->GetValue();
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename T >
void ParamComboBox<T>::CommitTo( Action_ABC& action ) const
{
    std::auto_ptr< ActionParameter< T > > param( new ActionParameter< T >( GetName() ) );
    param->SetValue( comboBox_->GetValue() );
    action.AddParameter( *param.release() );
}

#endif // __ParamComboBox_h_
