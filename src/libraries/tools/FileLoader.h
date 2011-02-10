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

namespace xml
{
    class xistream;
}

namespace tools
{
    class ExerciseConfig;

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
             FileLoader( const tools::ExerciseConfig& config, const std::string& file );
             FileLoader( const tools::ExerciseConfig& config, std::string& invalidSignatureFiles, std::string& missingSignatureFiles );
             FileLoader( const tools::ExerciseConfig& config, std::string& invalidSignatureFiles, std::string& missingSignatureFiles, std::string& malformedFiles );
    virtual ~FileLoader();
    //@}

    //! @name Operations
    //@{
    FileLoader& Load( const std::string& rootTag, T_Loader loader );
    void AddToCRC();
    virtual FileLoader& LoadAndUpdate( const std::string& rootTag, T_Loader loader, const std::string& xslTransform );
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
    std::string* malformedFiles_;
    bool addToCRC_;
    //@}
};

}

#endif // __FileLoader_h_
