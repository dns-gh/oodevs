// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-05 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ComboBox_Vector.inl $
// $Author: Ape $
// $Modtime: 5/01/05 15:38 $
// $Revision: 1 $
// $Workfile: ADN_ComboBox_Vector.inl $
//
// *****************************************************************************



// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Vector constructor
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
template< typename T >
ADN_ComboBox_Vector<T>::ADN_ComboBox_Vector( QWidget* pParent, const char* szName )
: ADN_ComboBox( pParent, szName )
{
    pConnector_ = new ADN_CBV_Connector<T>( *this );
}


// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Vector destructor
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
template< typename T >
ADN_ComboBox_Vector<T>::~ADN_ComboBox_Vector()
{
}
