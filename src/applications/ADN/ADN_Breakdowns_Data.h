// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-16 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Breakdowns_Data.h $
// $Author: Nld $
// $Modtime: 27/04/05 10:57 $
// $Revision: 4 $
// $Workfile: ADN_Breakdowns_Data.h $
//
// *****************************************************************************

#ifndef __ADN_Breakdowns_Data_h_
#define __ADN_Breakdowns_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Equipement_Data.h"

class ADN_XmlInput_Helper;


// =============================================================================
/** @class  ADN_Breakdowns_Data
    @brief  ADN_Breakdowns_Data
*/
// Created: APE 2005-03-16
// =============================================================================
class ADN_Breakdowns_Data
: public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_Breakdowns_Data )

public:
// *****************************************************************************
    class RepairPartInfo
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( RepairPartInfo )
    public:
        RepairPartInfo();

        virtual std::string GetNodeName();
        std::string GetItemName();

        RepairPartInfo* CreateCopy();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Equipement_Data::CategoryInfo> ptrPart_;
        ADN_Type_Int nNbr_;
    };

    typedef ADN_Type_Vector_ABC<RepairPartInfo> T_RepairPartInfoVector;
    typedef T_RepairPartInfoVector::iterator    IT_RepairPartInfoVector;


// *****************************************************************************

    class BreakdownInfo
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( BreakdownInfo )
    public:
         BreakdownInfo();
        ~BreakdownInfo();

        virtual std::string GetNodeName();
        std::string GetItemName();

        BreakdownInfo* CreateCopy();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

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
    ~ADN_Breakdowns_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( T_StringList& vFiles ) const;
    void Reset();

    ADN_Breakdowns_Data::BreakdownInfo* FindBreakdown( const std::string& strName );

    int GetNextId();
    //@}

private:
    void ReadArchive( ADN_XmlInput_Helper& input );
    void WriteArchive( MT_OutputArchive_ABC& output );

public:
    int nNextId_;
    ADN_Type_String       strAverageDiagnosticTime_;
    T_BreakdownInfoVector vBreakdowns_;
};

#endif // __ADN_Breakdowns_Data_h_
