// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AGR_EnumGenerator_h_
#define __AGR_EnumGenerator_h_

// =============================================================================
/** @class  AGR_EnumGenerator
    @brief  AGR_EnumGenerator
*/
// Created: APE 2005-02-08
// =============================================================================
class AGR_EnumGenerator
{
public:
    //! @name Constructors/Destructor
    //@{
             AGR_EnumGenerator();
    virtual ~AGR_EnumGenerator();
    //@}

    //! @name Operations
    //@{
    void Generate( const AGR_Workspace& workspace, const std::string& strOutputPath );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AGR_EnumGenerator( const AGR_EnumGenerator& );            //!< Copy constructor
    AGR_EnumGenerator& operator=( const AGR_EnumGenerator& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void GenerateEnumFile( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateTranslatorHeader( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateTranslatorImplementation( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;

    std::string CreatePrettyEnumName( const std::string strEnumName ) const;
    std::string CreatePrettyValueName( const std::string strEnumName, const std::string strValueName ) const;
    std::string CreateScriptName( const std::string strValue ) const;
    std::string GetCoreEnumName( const std::string strEnumName ) const;
    //@}
};

#endif // __AGR_EnumGenerator_h_
