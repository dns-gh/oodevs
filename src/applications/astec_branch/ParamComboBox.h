// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamComboBox.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 3 $
// $Workfile: ParamComboBox.h $
//
// *****************************************************************************

#ifndef __ParamComboBox_h_
#define __ParamComboBox_h_

#include "Param_ABC.h"
#include "MT/MT_Qt/MT_ValuedComboBox.h" // $$$$ AGE 2006-03-15: move

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
    MT_ValuedComboBox<T>* pComboBox_; // $$$$ AGE 2006-03-15: MT_Caca
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
    pComboBox_ = new MT_ValuedComboBox<T>( this );
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
