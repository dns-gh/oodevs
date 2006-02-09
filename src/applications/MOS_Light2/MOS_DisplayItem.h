// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_DisplayItem_h_
#define __MOS_DisplayItem_h_

class QLabel;

// =============================================================================
/** @class  MOS_DisplayItem
    @brief  MOS_DisplayItem
*/
// Created: AGE 2006-02-09
// =============================================================================
class MOS_DisplayItem
{

public:
    //! @name Constructors/Destructor
    //@{
             MOS_DisplayItem( QWidget* parent, const char* name, bool bold );
    virtual ~MOS_DisplayItem();
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
    MOS_DisplayItem( const MOS_DisplayItem& );            //!< Copy constructor
    MOS_DisplayItem& operator=( const MOS_DisplayItem& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    QLabel* valueLabel_;
    //@}
};

#endif // __MOS_DisplayItem_h_
