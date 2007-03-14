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

// =============================================================================
/** @class  ParamComboBox
    @brief  ParamComboBox
*/
// Created: APE 2004-04-21
// =============================================================================
template< class T >
class ParamComboBox : public Param_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamComboBox( T& asn, const QString& name );
    virtual ~ParamComboBox();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    void AddItem( const QString& name, const T& value );
    virtual void Commit();
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
    T& asn_;
    gui::ValuedComboBox<T>* comboBox_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ParamComboBox constructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< class T >
ParamComboBox<T>::ParamComboBox( T& asn, const QString& name )
    : Param_ABC( name )
    , asn_ ( asn )
    , comboBox_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox destructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< class T >
ParamComboBox<T>::~ParamComboBox()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
template< class T >
void ParamComboBox<T>::BuildInterface( QWidget* parent )
{
    QHBox* box = new QHBox( parent );
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
template< class T >
void ParamComboBox<T>::AddItem( const QString& name, const T& value )
{
    values_.push_back( std::make_pair( name, &value ) );
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::WriteMsg
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< class T >
void ParamComboBox<T>::Commit()
{
    if( !comboBox_ )
        InterfaceNotInitialized();
    asn_ = comboBox_->GetValue();
}

#endif // __ParamComboBox_h_
