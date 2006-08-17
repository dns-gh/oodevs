// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DataDictionary_h_
#define __DataDictionary_h_

#include "Extension_ABC.h"
#include "Value_ABC.h"
#include <qstring.h>
#include <qstringlist.h>

// =============================================================================
/** @class  DataDictionary
    @brief  DataDictionary
*/
// Created: AGE 2006-06-22
// =============================================================================
class DataDictionary : public Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DataDictionary();
    virtual ~DataDictionary();
    //@}

    //! @name Operations
    //@{
    QStringList FindKey( QString name ) const;

    template< typename T >
    void Register( const QString& name, const T& value )
    {
        Value_ABC*& data = data_[ name ];
        delete data;
        data = new Value< T >( value );
    }

    void Display( const QString& name, Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DataDictionary( const DataDictionary& );            //!< Copy constructor
    DataDictionary& operator=( const DataDictionary& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, Value_ABC* >               T_Data;
    typedef T_Data::iterator                             IT_Data;
    typedef T_Data::const_iterator                      CIT_Data;
    //@}

private:
    //! @name Member data
    //@{
    T_Data      data_;
    //@}
};

#endif // __DataDictionary_h_
