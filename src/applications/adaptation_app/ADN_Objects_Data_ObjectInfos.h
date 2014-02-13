// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Objects_Data_ObjectInfos_h_
#define __ADN_Objects_Data_ObjectInfos_h_

#include "ADN_CapacityInfos.h"
#include "ADN_Drawings_Data.h"
#include "ADN_RefWithName.h"

// =============================================================================
/** @class  ADN_Objects_Data_ObjectInfos
    @brief  ADN_Objects_Data_ObjectInfos Here to avoid circular include
*/
// Created: ABR 2011-12-06
// =============================================================================
class ADN_Objects_Data_ObjectInfos : public ADN_RefWithLocalizedName
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Objects_Data_ObjectInfos();
    virtual ~ADN_Objects_Data_ObjectInfos();
    //@}

    //! @name Operations
    //@{
    ADN_Objects_Data_ObjectInfos* CreateCopy();
    void ReadArchive( xml::xistream& input );
    void FixConsistency();
    void WriteArchive( xml::xostream& output ) const;
    virtual void CheckValidity();
    void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;
    std::string GetAllGeometries() const;
    //@}

    //! @name Static method
    //@{
    static std::string GenerateNextType();
    //@}

private:
    //! @name Helpers
    //@{
    void ReadCapacityArchive( const std::string& type, xml::xistream& xis );
    void ReadGeometry( xml::xistream& xis );
    void InitializeCapacities();
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< helpers::ADN_TypeCapacity_Infos > > T_CapacityMap;
    //@}

    //! @name Member data
    //@{
    ADN_Type_String strType_;
    ADN_Type_String description_;
    ADN_Type_Int pointSize_;
    ADN_Type_Bool geometries_[ 4 ];
    ADN_TypePtr_InVector_ABC< ADN_Drawings_Data::DrawingInfo > symbols_[ 4 ];
    T_CapacityMap capacities_;
    //@}
};

#endif // __ADN_Objects_Data_ObjectInfos_h_
