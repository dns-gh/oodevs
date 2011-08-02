// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ValuedComboBox_h_
#define __ValuedComboBox_h_

#pragma warning( push, 0 )
#include <QtGui/qcombobox.h>
#pragma warning( pop )

namespace gui
{

// =============================================================================
/** @class  ValuedComboBox
    @brief  Valued ComboBox
*/
// Created: SBO 2006-04-18
// =============================================================================
template< typename T >
class ValuedComboBox : public QComboBox
{

private:
    //! @name Types
    //@{
    typedef std::vector< T >                  T_ValueVector;
    typedef typename T_ValueVector::iterator IT_ValueVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit ValuedComboBox( QWidget* parent = 0, const char* name = 0 );
    virtual ~ValuedComboBox();
    //@}

    //! @name Accessors
    //@{
    T    GetValue    ();
    bool sorting     ();
    //@}

    //! @name Operations
    //@{
    void AddItem( const QString& label, const T& value );
    void AddItem( const std::string& label, const T& value );
    void ChangeItem( const QString& label, const T& value );
    void RemoveItem( const T& value );

    int GetItemIndex( const T& value );
    void SetCurrentItem( const T& value );
    void Clear();
    unsigned int Count() const;

    void setSorting( bool sorting );
    //@}

private:
    //! @name Copy constructor/Assignment
    //@{
    ValuedComboBox( const ValuedComboBox& );
    ValuedComboBox& operator=( const ValuedComboBox& );
    //@}

private:
    //! @name Member data
    //@{
    T_ValueVector values_;
    bool          sorting_;
    //@}
};

}

#include "ValuedComboBox.inl"

#endif // __ValuedComboBox_h_
