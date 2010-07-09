// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ExportFilter_h_
#define __ExportFilter_h_

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ExportFilter
    @brief  ExportFilter
*/
// Created: SBO 2010-04-22
// =============================================================================
class ExportFilter
{
public:
    //! @name Constructors/Destructor
    //@{
             ExportFilter( xml::xistream& xis, const tools::ExerciseConfig& config );
    virtual ~ExportFilter();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    QString GetDescription() const;
    void Execute( const std::string& outputDirectory ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExportFilter( const ExportFilter& );            //!< Copy constructor
    ExportFilter& operator=( const ExportFilter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadDescription( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, std::pair< QString, QString > > T_Descriptions;
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    const std::string target_;
    const std::string xsl_;
    const std::string output_;
    const std::string currentLanguage_;
    T_Descriptions descriptions_;
    //@}
};

#endif // __ExportFilter_h_
