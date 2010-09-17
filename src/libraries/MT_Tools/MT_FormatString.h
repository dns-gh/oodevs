// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_FormatString_h_
#define __MT_FormatString_h_

#include <string>
#include <ostream>
#include <cstdarg>

//=============================================================================
/** @class  MT_FormatString
    @brief  Format string with a printf-like syntax.

    Usage:

    @code
    cout << MT_FormatString( "%s = %g", "PI", 3.14159f ) << endl;
    @encode

    @note MT_FormatString is reentrant, that is the following scheme is allowed:

    @code
    cout << MT_FormatString( "%s %g", ( const char* )MT_FormatString( "%s = ", "PI" ), 3.14159f ) << endl;    
    @encode

    As for sprintf, take care about casting the optionnal parameters to the type expected by the format
    specifier !
*/
// Created: CBX 2001-06-18
//=============================================================================
class MT_FormatString
{
public:
    //! @name Constructors
    //@{
    MT_FormatString( const char* pFormat, ... );
    //@}

    //! @name Operations
    //@{
    void Format( const char* pFormat, ... );

    void VFormat( const char* pFormat, va_list argList );
    //@}

    //! @name Operators
    //@{
    operator const std::string& () const;
    const char* c_str() const;

    friend std::ostream& operator<<( std::ostream& os, const MT_FormatString& fstr )
    {
        return os << fstr.fstr_;
    }
    //@}

private:
    //! @name Member data
    //@{
    std::string fstr_;
    //@}
};

#endif // __MT_FormatString_h_
