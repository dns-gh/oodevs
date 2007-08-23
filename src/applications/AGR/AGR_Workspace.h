// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AGR_Workspace_h_
#define __AGR_Workspace_h_

class AGR_Enumeration;
class AGR_Type_ABC;
class AGR_FragOrder;
class AGR_Class;
class AGR_TypeRC;
class AGR_RC;

// =============================================================================
/** @class  AGR_Workspace
    @brief  AGR_Workspace
*/
// Created: AGN 2004-04-22
// =============================================================================
class AGR_Workspace
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_Workspace();
    virtual ~AGR_Workspace();
    //@}

    //! @name Operations
    //@{
    void Read( const std::string& strFile );
    //@}

    //! @name Accessors
    //@{
    const T_Enumeration_Vector& Enums() const;
    //@}

    //! @name Tools
    //@{
    void ReplaceInString( std::string& strToProcess, const std::string& strToSearch, const std::string& strReplace ) const;
    void ReadStringFile( const std::string& strFileName, std::string& strResult ) const;
    void WriteStringInFile( const std::string& strToWrite, const std::string& strFileName ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_Workspace( const AGR_Workspace& );            //!< Copy constructor
    AGR_Workspace& operator=( const AGR_Workspace& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Read( xml::xistream& xis );
    void ReadElement( const std::string& type, xml::xistream& xis );
    void ReadImport( xml::xistream& xis );
    void ReadSimpleType( xml::xistream& xis );
    void ReadSimpleTypeElement( const std::string& type, xml::xistream& xis, const std::string& strTypeName );
    //@}

private:
    //! @name Member data
    //@{
    T_Enumeration_Vector enumerationList_;
    T_String_Set parsedFileSet_;
    //@}
};

#endif // __AGR_Workspace_h_
