// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-04-22 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Workspace.h $
// $Author: Ape $
// $Modtime: 8/02/05 15:12 $
// $Revision: 17 $
// $Workfile: AGR_Workspace.h $
//
// *****************************************************************************

#ifndef __AGR_Workspace_h_
#define __AGR_Workspace_h_

class AGR_Mission;
class AGR_Enumeration;
class AGR_Type_ABC;
class AGR_Member;
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

    //! @name Types
    //@{
    typedef std::vector< AGR_Mission* >           T_Mission_Vector;
    typedef T_Mission_Vector::const_iterator    CIT_Mission_Vector;

    typedef std::vector< AGR_FragOrder* >           T_FragOrder_Vector;
    typedef T_FragOrder_Vector::const_iterator    CIT_FragOrder_Vector;

    typedef std::vector< const AGR_RC* > T_RCVector;
    typedef T_RCVector::const_iterator   CIT_RCVector;
    //@}

    //! @name Accessors
    //@{
    const T_Mission_Vector& Missions() const;
    const T_FragOrder_Vector& FragOrders() const;
    const T_Enumeration_Vector& Enums() const;
    const T_RCVector&           GetRCs() const;
    //@}

    //! @name Operations
    //@{
    AGR_Member* CreateMember( const std::string& strName, const std::string& strASNType, const AGR_Class& ownerClass, bool bOptional ) const;
    AGR_Member* CreateLocalTypeMember( const std::string& strMissionName, const std::string& strMemberName, MT_XXmlInputArchive& input, const AGR_Class& ownerClass, bool bOptional ) const;

    const AGR_Type_ABC* FindType  ( const std::string& strName ) const;
    const AGR_TypeRC*   FindTypeRC( const std::string& strType ) const;
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

    //! @name Types
    //@{
    typedef std::vector< AGR_Type_ABC* >          T_TypeVector;
    typedef T_TypeVector::const_iterator        CIT_TypeVector;

    typedef std::map< std::string, std::string >      T_StringMap;
    typedef T_StringMap::const_iterator             CIT_StringMap;

    typedef std::map< std::string, const AGR_TypeRC* > T_TypeRCMap;
    typedef T_TypeRCMap::const_iterator                CIT_TypeRCMap;
    //@}

    //! @name Helpers
    //@{
    void Read( MT_XXmlInputArchive& input, const std::string& strModuleName );
    AGR_Mission* FindMission( const std::string& strFullName ) const;
    AGR_FragOrder* FindFragOrder( const std::string& strName ) const;
    
    void ReadImport( MT_XXmlInputArchive& input, const std::string& strModuleName );
    void ReadSimpleType( MT_XXmlInputArchive& input, const std::string& strModuleName );
    void ReadComplexType( MT_XXmlInputArchive& input, const std::string& strModuleName );
    //@}

private:
    //! @name Member data
    //@{
    T_FragOrder_Vector fragOrderList_;
    T_Mission_Vector missionList_;
    T_Enumeration_Vector enumerationList_;
    T_TypeVector typeList_;
    T_String_Set parsedFileSet_;

    T_TypeRCMap typeRCs_;
    T_RCVector  rcs_;

    T_StringMap lowNameMap_;
    T_StringMap demandLowNameMap_;

    bool bMsgCRParsed_; //$$$ N'importe quoi
    //@}
};

#include "AGR_Workspace.inl"

#endif // __AGR_Workspace_h_
