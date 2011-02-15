// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CommandLineConfig_ABC_h_
#define __CommandLineConfig_ABC_h_

#include <string>
#include <boost/noncopyable.hpp>

namespace boost
{
namespace program_options
{
    class options_description;
    class variables_map;
}
}

namespace tools
{

// =============================================================================
/** @class  CommandLineConfig_ABC
    @brief  CommandLineConfig_ABC
*/
// Created: NLD 2007-01-10
// =============================================================================
class CommandLineConfig_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             CommandLineConfig_ABC();
    virtual ~CommandLineConfig_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Parse( int argc, char* argv[] );
    //@}

protected:
    //! @name Protected operations
    //@{
    void AddOptions( boost::program_options::options_description& options );
    bool IsSet( const std::string& option ) const;
    //@}

private:
    //! @name Member data
    //@{
    boost::program_options::options_description* options_;
    boost::program_options::variables_map* values_;
    //@}
};

}

#endif // __CommandLineConfig_ABC_h_
