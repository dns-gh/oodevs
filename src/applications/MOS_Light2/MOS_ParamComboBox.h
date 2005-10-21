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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamComboBox.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 3 $
// $Workfile: MOS_ParamComboBox.h $
//
// *****************************************************************************

#ifndef __MOS_ParamComboBox_h_
#define __MOS_ParamComboBox_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Param_ABC.h"


// =============================================================================
/** @class  MOS_ParamComboBox
    @brief  MOS_ParamComboBox
    @par    Using example
    @code
    MOS_ParamComboBox;
    @endcode
*/
// Created: APE 2004-04-21
// =============================================================================
template< class T >
class MOS_ParamComboBox : public QHBox, public MOS_Param_ABC
{
    MT_COPYNOTALLOWED( MOS_ParamComboBox );

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamComboBox( T& output, const std::string strLabel, QWidget* pParent );
    ~MOS_ParamComboBox();
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
// Name: MOS_ParamComboBox constructor
/** @param  output 
    @param  strLabel 
    @param  pParent 
*/
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< class T >
MOS_ParamComboBox<T>::MOS_ParamComboBox( T& output, const std::string strLabel, QWidget* pParent )
    : QHBox         ( pParent )
    , MOS_Param_ABC ()
    , output_       ( output )
{
    pLabel_ = new QLabel( strLabel.c_str(), this );
    pLabel_->setAlignment( AlignVCenter | AlignLeft );
    pComboBox_ = new MT_ValuedComboBox<T>( this );
    pComboBox_->setSorting( true );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamComboBox destructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< class T >
MOS_ParamComboBox<T>::~MOS_ParamComboBox()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamComboBox::AddItem
/** @param  strLabel 
    @param  value 
*/
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< class T >
void MOS_ParamComboBox<T>::AddItem( const std::string strLabel, const T& value )
{
    pComboBox_->AddItem( strLabel.c_str(), value );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamComboBox::WriteMsg
/** @param  sParam 
*/
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< class T >
void MOS_ParamComboBox<T>::WriteMsg( std::stringstream& strMsg )
{
    output_ = pComboBox_->GetValue();
    strMsg << pLabel_->text().latin1() << ": " << pComboBox_->GetValue();
}


#endif // __MOS_ParamComboBox_h_
