// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __Stages_h_
#define __Stages_h_

#include <boost/noncopyable.hpp>
#include <string>
#include <map>

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  Stages
    @brief  Stages
*/
// Created: ABR 2013-02-20
// =============================================================================
class Stages : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             Stages();
    virtual ~Stages();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config );
    void Purge();
    std::string FindTranslation( const std::string& key ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void Read( xml::xistream& xis );
    void ReadStage( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::map< const std::string, std::string > stages_;
    //@}
};

#endif // __Stages_h_
