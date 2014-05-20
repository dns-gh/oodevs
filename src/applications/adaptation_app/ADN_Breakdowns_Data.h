// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Breakdowns_Data_h_
#define __ADN_Breakdowns_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_CrossedRef.h"
#include "ADN_Types.h"
#include "ADN_Resources_Data.h"

// =============================================================================
/** @class  ADN_Breakdowns_Data
    @brief  ADN_Breakdowns_Data
*/
// Created: APE 2005-03-16
// =============================================================================
class ADN_Breakdowns_Data : public ADN_Data_ABC
{
public:
// *****************************************************************************
    class RepairPartInfo : public ADN_CrossedRef< ADN_Resources_Data::CategoryInfo >
    {
    public:
                 RepairPartInfo();
        virtual ~RepairPartInfo() {}

        RepairPartInfo* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Int nNbr_;
    };

    typedef ADN_Type_Vector_ABC< RepairPartInfo > T_RepairPartInfoVector;

// *****************************************************************************

    class BreakdownInfo : public ADN_RefWithLocalizedName
    {
    public:
                 BreakdownInfo();
                 BreakdownInfo( unsigned int id );
        virtual ~BreakdownInfo();

        BreakdownInfo* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void ReadPart( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Enum< E_BreakdownNTI, eNbrBreakdownNTI > nNTI_;
        ADN_Type_Enum< E_BreakdownType, eNbrBreakdownType > nType_;
        ADN_Type_Int nId_;
        ADN_Type_Time repairTime_;
        ADN_Type_Time repairTimeVariance_;
        T_RepairPartInfoVector vRepairParts_;
    };

    typedef ADN_Type_Vector_ABC< BreakdownInfo > T_BreakdownInfoVector;

// *****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Breakdowns_Data();
    virtual ~ADN_Breakdowns_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( tools::Path::T_Paths& vFiles ) const;
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

    T_BreakdownInfoVector& GetBreakdowns();
    ADN_Breakdowns_Data::BreakdownInfo* FindBreakdown( const std::string& strName );
    QStringList GetBreakdownsThatUse( ADN_Resources_Data::CategoryInfo& part );
    //@}

private:
    void ReadArchive( xml::xistream& input );
    void ReadCategory( xml::xistream& input );
    void ReadBreakdown( xml::xistream& input, const E_BreakdownNTI& nti );
    void WriteArchive( xml::xostream& output ) const;

public:
    ADN_Type_String strAverageDiagnosticTime_;
    ADN_Type_Bool repairDurationInManHours_;
    T_BreakdownInfoVector vBreakdowns_;

private:
    static tools::IdManager idManager_;
};

#endif // __ADN_Breakdowns_Data_h_
