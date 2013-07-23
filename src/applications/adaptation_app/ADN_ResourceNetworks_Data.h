// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ResourceNetworks_Data_h_
#define __ADN_ResourceNetworks_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Resources_Data.h"

// =============================================================================
/** @class  ADN_ResourceNetworks_Data
    @brief  ADN_ResourceNetworks_Data
*/
// Created: JSR 2010-09-13
// =============================================================================
class ADN_ResourceNetworks_Data : public ADN_Data_ABC
{

public:
    class ResourceNetworkInfos : public ADN_RefWithLocalizedName
    {
    public:
        //! @name Constructors/Destructor
        //@{
                 ResourceNetworkInfos();
        virtual ~ResourceNetworkInfos();
        //@}

        //! @name Operations
        //@{
        ResourceNetworkInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );
        //@}

    public:
        //! @name Member data
        //@{
        ADN_Type_String strColor_;
        ADN_Type_Int nProduction_;
        ADN_TypePtr_InVector_ABC< ADN_Resources_Data::CategoryInfo > ptrCategory_;
        //@}
    };

    //! @name Types
    //@{
    typedef ADN_Type_Vector_ABC< ResourceNetworkInfos >  T_ResourceNetworkInfosVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ResourceNetworks_Data();
    virtual ~ADN_ResourceNetworks_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( tools::Path::T_Paths& l ) const;
    void Reset();
    T_ResourceNetworkInfosVector& GetResourceNetworksInfos();
    ResourceNetworkInfos* FindResourceNetwork( const std::string& strName );
    QStringList GetResourceNetworksThatUse( ADN_Resources_Data::CategoryInfo& category );
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadArchive( xml::xistream& input );
    void ReadResourceNetwork( xml::xistream& input );
    void WriteArchive( xml::xostream& output );
    //@}

private:
    //! @name Member data
    //@{
    T_ResourceNetworkInfosVector resourceNetworks_;
    //@}
};

#endif // __ADN_ResourceNetworks_Data_h_
