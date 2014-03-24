// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Volumes_Data_h_
#define __ADN_Volumes_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"

// =============================================================================
/** @class  ADN_Volumes_Data
    @brief  ADN_Volumes_Data
*/
// Created: ABR 2013-07-10
// =============================================================================
class ADN_Volumes_Data : public ADN_Data_ABC
{
public:
    //! @name Types
    //@{
    class VolumeInfos : public ADN_RefWithLocalizedName
    {
    public:
        VolumeInfos();
        VolumeInfos* CreateCopy();
    };

    typedef ADN_Type_Vector_ABC< VolumeInfos > T_VolumeInfos_Vector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Volumes_Data();
    virtual ~ADN_Volumes_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( tools::Path::T_Paths& vFiles ) const;

    T_VolumeInfos_Vector& GetVolumesInfos();
    VolumeInfos* FindSize( const std::string& strName );
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& ) const;
    //@}

private:
    //! @name Serialization
    //@{
    void ReadArchive( xml::xistream& input );
    void ReadVolume( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;
    //@}

private:
    //! @name Member data
    //@{
    T_VolumeInfos_Vector vSizes_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_Volumes_Data::GetSizesInfos
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
inline
ADN_Volumes_Data::T_VolumeInfos_Vector& ADN_Volumes_Data::GetVolumesInfos()
{
    return vSizes_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Volumes_Data::FindSize
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
inline
ADN_Volumes_Data::VolumeInfos* ADN_Volumes_Data::FindSize( const std::string& strName )
{
    for( auto it = vSizes_.begin(); it != vSizes_.end(); ++it )
        if( (*it)->strName_ == strName )
            return *it;
    return 0;
}

#endif // __ADN_Volumes_Data_h_
