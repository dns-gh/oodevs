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

#ifdef __GNUG__
#   pragma interface
#endif

#include "Param_ABC.h"


// =============================================================================
/** @class  ParamComboBox
    @brief  ParamComboBox
    @par    Using example
    @code
    ParamComboBox;
    @endcode
*/
// Created: APE 2004-04-21
// =============================================================================
template< class T >
class ParamComboBox : public QHBox, public Param_ABC
{
    MT_COPYNOTALLOWED( ParamComboBox );

public:
    //! @name Constructors/Destructor
    //@{
    ParamComboBox( T& output, const std::string strLabel, QWidget* pParent, OptionalParamFunctor_ABC* pOptional );
    ~ParamComboBox();
    //@}

    //! @name Operations
    //@{
    void AddItem( const std::string strLabel, const T& value );
    void WriteMsg( std::stringstream& strMsg );
    //@}

private:
    //! @name Member data
    //@{
    T& output_;
    QLabel* pLabel_;
    MT_ValuedComboBox<T>* pComboBox_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: ParamComboBox constructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< class T >
ParamComboBox<T>::ParamComboBox( T& output, const std::string strLabel, QWidget* pParent, OptionalParamFunctor_ABC* pOptional )
    : QHBox         ( pParent )
    , Param_ABC ( pOptional )
    , output_       ( output )
{
    pLabel_ = new QLabel( strLabel.c_str(), this );
    pLabel_->setAlignment( AlignVCenter | AlignLeft );
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
void ParamComboBox<T>::WriteMsg( std::stringstream& strMsg )
{
    if( pOptional_ )
        pOptional_->SetOptionalPresent();

    output_ = pComboBox_->GetValue();
    strMsg << pLabel_->text().latin1() << ": " << pComboBox_->GetValue();
}


#endif // __ParamComboBox_h_
