// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ComboBox_Vector_h_
#define __ADN_ComboBox_Vector_h_

#include "ADN_ComboBox.h"
#include "ADN_ComboBoxItem.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_ComboBox_Vector
    @brief  ADN_ComboBox_Vector
    @par    Using example
    @code
    ADN_ComboBox_Vector;
    @endcode
*/
// Created: APE 2005-01-05
// =============================================================================
template< typename VectorType >
class ADN_ComboBox_Vector : public ADN_ComboBox
                          , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ComboBox_Vector( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_ComboBox_Vector();
    //@}
};

template< typename AType >
class ADN_CBV_Connector : public ADN_Connector_ComboBox
{
public:
    explicit ADN_CBV_Connector( ADN_ComboBox_Vector<AType>& combo )
        : ADN_Connector_ComboBox( &combo )
    {}

    virtual ~ADN_CBV_Connector()
    {}

    virtual ADN_ComboBoxItem* CreateItem( void * obj )
    {
        // Create the new combo item.
        ADN_ComboBoxItem* pItem = new ADN_ComboBoxItem(*pCombo_,obj);

        // Connect it with the object's name.
        pItem->GetConnector().Connect( &(static_cast<AType*>(obj))->strName_ );

        return pItem;
    };
};

#include "ADN_ComboBox_Vector.inl"

#endif // __ADN_ComboBox_Vector_h_
