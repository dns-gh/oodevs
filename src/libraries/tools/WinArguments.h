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
#include <boost/noncopyable.hpp>

namespace tools
{
// =============================================================================
/** @class  WinArguments
    @brief  WinMain argument parser
*/
// Created: RDS 2008-07-22
// =============================================================================
class WinArguments : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit WinArguments( const std::wstring& arguments );
    virtual ~WinArguments();
    //@}

    //! @name Accessors
    //@{
    char** Argv() const;
    int Argc() const;

    bool HasOption( const std::string& name ) const;
    std::string GetOption( const std::string& name, const std::string& defaultValue = "" ) const;

    std::string GetCommandLine() const;
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
