// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef import_Warnings_h
#define import_Warnings_h

#include <fstream>
#include <string>
#include <set>

// =============================================================================
/** @class  Warnings
    @brief  Warnings
*/
// Created: LDC 2010-07-07
// =============================================================================
class Warnings
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Warnings( const std::string& log );
    virtual ~Warnings();
    //@}

    //! @name Operations
    //@{
    void Add( const std::string& id );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Warnings( const Warnings& );            //!< Copy constructor
    Warnings& operator=( const Warnings& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::ofstream log_;
    std::set< std::string > identifiers_;
    //@}
};

#endif // import_Warnings_h
