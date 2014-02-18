// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PathParser_h_
#define __PathParser_h_

#include <sstream>

namespace svg 
{

// =============================================================================
/** @class  PathParser
    @brief  PathParser // $$$$ AGE 2006-08-10: faire throw
*/
// Created: AGE 2006-08-10
// =============================================================================
class PathParser
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PathParser( const std::string& data );
    virtual ~PathParser();
    //@}

    //! @name Operations
    //@{
    bool IsCoordinate();
    bool IsFloat();

    char GetCommand();
    geometry::Point GetCoordinate();
    float GetFloat();
    bool GetFlag();

    bool Eof();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PathParser( const PathParser& );            //!< Copy constructor
    PathParser& operator=( const PathParser& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void SkipSeparators();
    bool IsSeparator( int value );
    void SkipComma();
    PathParser SubParser();
    //@}

private:
    //! @name Member data
    //@{
    std::string base_;
    std::istringstream data_;
    bool crap_;
    //@}
};

}

#endif // __PathParser_h_
