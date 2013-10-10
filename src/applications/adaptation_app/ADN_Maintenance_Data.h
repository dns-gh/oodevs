// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Maintenance_Data_h_
#define __ADN_Maintenance_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_AvailabilityWarning.h"

// =============================================================================
/** @class  ADN_Maintenance_Data
    @brief  ADN_Maintenance_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_Maintenance_Data : public ADN_Data_ABC
{
public:
    class WorkingSchemeInfo : public ADN_Ref_ABC
    {
    public:
        explicit WorkingSchemeInfo( unsigned int nIdx );
        virtual ~WorkingSchemeInfo() {}

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Int nIdx_;
        ADN_Type_Int nWorkTime_;
        ADN_Type_Time warningDelay_;
    };

    typedef ADN_Type_Vector_ABC< WorkingSchemeInfo > T_WorkingSchemeInfo_Vector;

// *****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Maintenance_Data();
    virtual ~ADN_Maintenance_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( tools::Path::T_Paths& vFiles ) const;
    //@}

private:
    void ReadArchive( xml::xistream& input );
    void ReadWorkingScheme( xml::xistream& input );
    void ReadAvailabilityAlert( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;

public:
    T_WorkingSchemeInfo_Vector vWorkingSchemes_;
    T_AvailabilityWarning_Vector vRepairerWarnings_;
    T_AvailabilityWarning_Vector vHaulerWarnings_;
};

#endif // __ADN_Maintenance_Data_h_
