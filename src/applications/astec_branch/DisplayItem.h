// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisplayItem_h_
#define __DisplayItem_h_

class QLabel;

// =============================================================================
/** @class  DisplayItem
    @brief  DisplayItem
*/
// Created: AGE 2006-02-09
// =============================================================================
class DisplayItem
{

public:
    //! @name Constructors/Destructor
    //@{
             DisplayItem( QWidget* parent, const char* name, bool bold );
    virtual ~DisplayItem();
    //@}

    //! @name Operations
    //@{
    void Display( const char* value );
    void Display( const std::string& value );
    void Display( const QString& value );
    template< typename T >
    void Display( const T& value ) {
        Display( QString::number( value ) );
    }
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DisplayItem( const DisplayItem& );            //!< Copy constructor
    DisplayItem& operator=( const DisplayItem& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    QLabel* valueLabel_;
    //@}
};

#endif // __DisplayItem_h_
