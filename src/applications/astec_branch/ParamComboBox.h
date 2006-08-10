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
#include "astec_gui/ValuedComboBox.h"

// =============================================================================
/** @class  ParamComboBox
    @brief  ParamComboBox
*/
// Created: APE 2004-04-21
// =============================================================================
template< class T >
class ParamComboBox : public QHBox, public Param_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamComboBox( QWidget* parent, T& asn, const std::string label );
    virtual ~ParamComboBox();
    //@}

    //! @name Operations
    //@{
    void AddItem( const std::string strLabel, const T& value );
    virtual void Commit();
    //@}

private:
    //! @name Copy/Assignment
    //@{    
    ParamComboBox( const ParamComboBox& );
    ParamComboBox& operator=( const ParamComboBox& );
    //@}

private:
    //! @name Member data
    //@{
    T& asn_;
    ValuedComboBox<T>* pComboBox_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ParamComboBox constructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< class T >
ParamComboBox<T>::ParamComboBox( QWidget* parent, T& asn, const std::string label )
    : QHBox( parent )
    , asn_ ( asn )
{
    QLabel* pLabel = new QLabel( label.c_str(), this );
    pLabel->setAlignment( AlignVCenter | AlignLeft );
    pComboBox_ = new ValuedComboBox<T>( this );
    pComboBox_->setSorting( true );
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
// Name: ParamComboBox::AddItem
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< class T >
void ParamComboBox<T>::AddItem( const std::string strLabel, const T& value )
{
    pComboBox_->AddItem( strLabel.c_str(), value );
}

// -----------------------------------------------------------------------------
// Name: ParamComboBox::WriteMsg
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< class T >
void ParamComboBox<T>::Commit()
{
    asn_ = pComboBox_->GetValue();
}

#endif // __ParamComboBox_h_
