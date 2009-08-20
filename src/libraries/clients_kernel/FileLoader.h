// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __FileLoader_h_
#define __FileLoader_h_

#include <boost/function.hpp>

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

namespace kernel
{

// =============================================================================
/** @class  FileLoader
    @brief  FileLoader
*/
// Created: SBO 2009-08-20
// =============================================================================
class FileLoader
{
public:
    //! @name Types
    //@{
    typedef boost::function< void ( xml::xistream& ) > T_Loader;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit FileLoader( const tools::ExerciseConfig& config );
    virtual ~FileLoader();
    //@}

    //! @name Operations
    //@{
    FileLoader& Load( const std::string& rootTag, T_Loader loader );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FileLoader( const FileLoader& );            //!< Copy constructor
    FileLoader& operator=( const FileLoader& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    std::auto_ptr< xml::xistream > xis_;
    //@}
};

}

#endif // __FileLoader_h_
