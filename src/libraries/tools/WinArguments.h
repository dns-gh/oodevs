// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __tools_WinArguments_h_
#define __tools_WinArguments_h_

#include <vector>

namespace tools
{
// =============================================================================
/** @class  WinArguments
    @brief  WinMain argument parser
*/
// Created: RDS 2008-07-22
// =============================================================================
class WinArguments
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit WinArguments( const std::string& arguments );
    virtual ~WinArguments();
    //@}

    //! @name Accessors
    //@{
    const char* const* Argv() const;
    int                Argc() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    WinArguments( const WinArguments& );            //!< Copy constructor
    WinArguments& operator=( const WinArguments& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< std::string > argv_;
    std::vector< const char* > cArgv_;
    //@}
};

}

#endif // __tools_WinArguments_h_
