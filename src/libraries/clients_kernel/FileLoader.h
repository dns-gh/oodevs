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
             FileLoader( const tools::ExerciseConfig& config, std::string& invalidSignatureFiles, std::string& missingSignatureFiles );
    virtual ~FileLoader();
    //@}

    //! @name Operations
    //@{
    FileLoader& Load( const std::string& rootTag, T_Loader loader );
    void AddToCRC();
    virtual FileLoader& LoadAndUpdate( const std::string& rootTag, T_Loader loader, const std::string& xslTransform ) = 0;
    //@}

protected:
    //! @name Helpers
    //@{
    void CheckSignatures( const std::string& file, std::string& invalidSignatureFiles, std::string& missingSignatureFiles ) const;
    void GetFile( const std::string& rootTag, xml::xistream& xis, std::string& file ) const;
    void Check( const std::string& file, T_Loader loader, const std::string& xslTransform );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FileLoader( const FileLoader& );            //!< Copy constructor
    FileLoader& operator=( const FileLoader& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    std::string* invalidSignatureFiles_;
    std::string* missingSignatureFiles_;
    bool addToCRC_;
    //@}
};

}

#endif // __FileLoader_h_
