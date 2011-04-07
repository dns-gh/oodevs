// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Config_h_
#define __Config_h_

#include "tools/ExerciseConfig.h"
#include <boost/utility/base_from_member.hpp>

namespace tools
{
    class RealFileLoaderObserver_ABC;
}

namespace mission_tester
{
    class Facade;
// =============================================================================
/** @class  Config
    @brief  Config
*/
// Created: PHC 2011-04-07
// =============================================================================
class Config : private boost::base_from_member< std::auto_ptr< tools::RealFileLoaderObserver_ABC > >
             , public tools::ExerciseConfig
{

public:
    //! @name Constructors/Destructor
    //@{
             Config( int argc, char** argv );
    virtual ~Config();
    //@}

    //! @name Operations
    //@{
    void ConfigureLogging( Facade& facade ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef boost::base_from_member< std::auto_ptr< tools::RealFileLoaderObserver_ABC > > T_FileLoader;
    //@}

private:
    //! @name Member data
    //@{
    std::string logFile_;
    //@}
};
}

#endif // __Config_h_
