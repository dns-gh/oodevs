// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Reports_Data_h_
#define __ADN_Reports_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"

// =============================================================================
/** @class  ADN_Reports_Data
    @brief  ADN_Reports_Data
*/
// Created: SBO 2006-12-14
// =============================================================================
class ADN_Reports_Data : public ADN_Data_ABC
{
// =============================================================================
// Report parameters
// =============================================================================
public:
    class ReportParameterValue : public ADN_Ref_ABC
    {
    public:
                 ReportParameterValue();
        virtual ~ReportParameterValue();

        std::string GetItemName();
        ReportParameterValue* CreateCopy();

        void ReadArchive ( xml::xistream& input );
        void WriteArchive( xml::xostream& output, unsigned int id );

    public:
        ADN_Type_String name_;
    };

    typedef ADN_Type_Vector_ABC< ReportParameterValue > T_ReportParameterValue_Vector;
    
public:
    class ReportParameter : public ADN_RefWithName
    {
    public:
                 ReportParameter();
        virtual ~ReportParameter();

        std::string GetItemName();
        ReportParameter* CreateCopy();

        void ReadArchive       ( xml::xistream& input );
        void ReadParameterValue( xml::xistream& input );
        void WriteArchive      ( xml::xostream& output );

    public:
        ADN_Type_Enum< E_MissionParameterType, eNbrMissionParameterType > type_;
        T_ReportParameterValue_Vector values_;
    };

    typedef ADN_Type_Vector_ABC< ReportParameter > T_ReportParameter_Vector;

// *****************************************************************************
// Report info
// *****************************************************************************

public:
    class ReportInfo : public ADN_Ref_ABC
    {
    public:
                 ReportInfo();
        virtual ~ReportInfo();

        std::string GetItemName();
        ReportInfo* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void ReadParameter( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Int id_;
        ADN_Type_String key_;
        ADN_Type_String category_;
        ADN_Type_String message_;
        ADN_Type_String clientFunctionTrigger_;
        T_ReportParameter_Vector parameters_;
    };

    typedef ADN_Type_Vector_ABC< ReportInfo > T_ReportInfo_Vector;

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Reports_Data();
    virtual ~ADN_Reports_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( tools::Path::T_Paths& l ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadArchive( xml::xistream& input );
    void ReadReport( xml::xistream& input );
    void WriteArchive( xml::xostream& output );
    //@}

private:
    //! @name Member data
    //@{
    T_ReportInfo_Vector reports_;
    //@}
};

#endif // __ADN_Reports_Data_h_
