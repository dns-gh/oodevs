// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Activities_Data_h_
#define __ADN_Activities_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"

// =============================================================================
/** @class  ADN_Activities_Data
    @brief  ADN_Activities_Data
*/
// Created: NPT 2013-04-16
// =============================================================================
class ADN_Activities_Data : public ADN_Data_ABC
{
// =============================================================================
// packages
// =============================================================================
public:
    class PackageInfos : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
    public:
        PackageInfos();
        explicit PackageInfos( const std::string name );
        virtual ~PackageInfos();

        std::string GetItemName();
        PackageInfos* CreateCopy();
        void WriteArchive( xml::xostream& output );
        void ReadArchive( xml::xistream& input );

    public:
        ADN_Type_String strName_;
    };
    typedef ADN_Type_Vector_ABC< PackageInfos > T_Package_Vector;

// =============================================================================
// Activities
// =============================================================================
public:
    class ActivityInfos : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
    public:
        ActivityInfos();
        explicit ActivityInfos( unsigned int id, const std::string& name, const std::string& description, bool meleeActivity );
        virtual ~ActivityInfos();

        std::string GetItemName();
        ActivityInfos* CreateCopy();
        void WriteArchive( xml::xostream& output );
        void ReadArchive( xml::xistream& input );

    public:
        ADN_Type_Int id_;
        ADN_Type_String strName_;
        ADN_Type_String description_;
        ADN_Type_Bool meleeActivity_;
        ADN_TypePtr_InVector_ABC< PackageInfos > package_;
    };
    typedef ADN_Type_Vector_ABC< ActivityInfos > T_Activity_Vector;

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Activities_Data();
    virtual ~ADN_Activities_Data();
    //@}

    //! @name Operations
    //@{
    void Reset();
    void WriteArchive( xml::xostream& output );
    void ReadArchive( xml::xistream& input );
    void ReadActivity( xml::xistream& input );
    void ReadPackage( xml::xistream& input );
    T_Activity_Vector& GetActivities();
    T_Package_Vector& GetPackages();

    virtual void FilesNeeded( T_StringList& vFiles ) const;
    PackageInfos* FindPackage( const std::string& strName );
    QStringList GetActivitiesThatUse( ADN_Activities_Data::PackageInfos& package );
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    T_Activity_Vector activities_;
    T_Package_Vector packages_;
    //@}
};

#endif // __ADN_Activities_Data_h_
