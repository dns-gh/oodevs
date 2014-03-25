// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Natures_Data_h_
#define __ADN_Natures_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "tools/IdManager.h"

// =============================================================================
/** @class  ADN_Natures_Data
    @brief  ADN_Natures_Data
*/
// Created: ABR 2013-07-10
// =============================================================================
class ADN_Natures_Data : public ADN_Data_ABC
{
public:
    //! @name NatureInfos
    //@{
    class NatureInfos : public ADN_RefWithLocalizedName
    {

    public:
                 NatureInfos();
        explicit NatureInfos( int id );
        virtual ~NatureInfos();
        NatureInfos* CreateCopy();

    public:
        ADN_Type_Int nId_;
    };

    typedef ADN_Type_Vector_ABC< NatureInfos > T_NatureInfos_Vector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Natures_Data();
    virtual ~ADN_Natures_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( tools::Path::T_Paths& vFiles ) const;

    ADN_Natures_Data::T_NatureInfos_Vector& GetNaturesInfos();
    ADN_Natures_Data::NatureInfos*          FindNature( const std::string& strName );
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& ) const;
    //@}

private:
    //! @name Serialization
    //@{
    void ReadArchive( xml::xistream& input );
    void ReadNature( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;
    //@}

private:
    //! @name Member data
    //@{
    ADN_Natures_Data::T_NatureInfos_Vector vDotationNatures_;
    static tools::IdManager idManager_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_Natures_Data::GetDotationNaturesInfos
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
inline
ADN_Natures_Data::T_NatureInfos_Vector& ADN_Natures_Data::GetNaturesInfos()
{
    return vDotationNatures_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Natures_Data::FindDotationNature
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
inline
ADN_Natures_Data::NatureInfos* ADN_Natures_Data::FindNature( const std::string& strName )
{
    for( auto it = vDotationNatures_.begin(); it != vDotationNatures_.end(); ++it )
        if( (*it)->strName_ == strName )
            return *it;
    return 0;
}

#endif // __ADN_Natures_Data_h_
