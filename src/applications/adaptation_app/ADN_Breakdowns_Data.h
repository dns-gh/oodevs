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
#include "ADN_Types.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Tr.h"

namespace xml { class xistream; }

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
    class RepairPartInfo : public ADN_Ref_ABC
                         , public ADN_DataTreeNode_ABC
    {
    public:
                 RepairPartInfo();
        virtual ~RepairPartInfo() {}

        virtual std::string GetNodeName();
        std::string GetItemName();

        RepairPartInfo* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Equipement_Data::CategoryInfo> ptrPart_;
        ADN_Type_Int nNbr_;
    };

    typedef ADN_Type_Vector_ABC<RepairPartInfo> T_RepairPartInfoVector;
    typedef T_RepairPartInfoVector::iterator    IT_RepairPartInfoVector;


// *****************************************************************************

    class BreakdownInfo : public ADN_Ref_ABC
                        , public ADN_DataTreeNode_ABC
    {
    public:
                 BreakdownInfo();
        virtual ~BreakdownInfo();

        virtual std::string GetNodeName();
        std::string GetItemName();

        BreakdownInfo* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void ReadPart   ( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_String   strName_;
        ADN_Type_Enum<E_BreakdownNTI, eNbrBreakdownNTI>    nNTI_;
        ADN_Type_Enum<E_BreakdownType, eNbrBreakdownType>  nType_;
        ADN_Type_Int      nId_;
        ADN_Type_Time     repairTime_;
        ADN_Type_Time     repairTimeVariance_;

        T_RepairPartInfoVector vRepairParts_;
    };

    typedef ADN_Type_Vector_ABC<BreakdownInfo>      T_BreakdownInfoVector;
    typedef T_BreakdownInfoVector::iterator         IT_BreakdownInfoVector;
    typedef T_BreakdownInfoVector::const_iterator   CIT_BreakdownInfoVector;


// *****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Breakdowns_Data();
    virtual ~ADN_Breakdowns_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( T_StringList& vFiles ) const;
    void Reset();

    T_BreakdownInfoVector& GetBreakdowns();
    ADN_Breakdowns_Data::BreakdownInfo* FindBreakdown( const std::string& strName );

    int GetNextId();
    //@}

private:
    void ReadArchive( xml::xistream& input );
    void ReadCategory( xml::xistream& input );
    void ReadBreakdown( xml::xistream& input, const E_BreakdownNTI& nti );
    void WriteArchive( xml::xostream& output );

public:
    int nNextId_;
    ADN_Type_String strAverageDiagnosticTime_;
    T_BreakdownInfoVector vBreakdowns_;
};

#endif // __ADN_Breakdowns_Data_h_
