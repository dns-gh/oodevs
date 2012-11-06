// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_MenuListView_h_
#define __ADN_MenuListView_h_

// =============================================================================
/** @class  ADN_MenuListView
    @brief  ADN_MenuListView
*/
// Created: ABR 2012-11-06
// =============================================================================
template< typename T >
class ADN_MenuListView : public QWidgetAction
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_MenuListView( ADN_Table3* receiver, ADN_Type_Vector_ABC< T >& vector, QObject* parent );
    virtual ~ADN_MenuListView();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* createWidget( QWidget* parent );
    T* SelectedValue() const;
    //@}

private:
    //! @name Member data
    //@{
    ADN_Table3* receiver_;
    ADN_Type_Vector_ABC< T >& vector_;
    QListWidget* listView_;
    //@}
};

#include "ADN_MenuListView.inl"

#endif // __ADN_MenuListView_h_
