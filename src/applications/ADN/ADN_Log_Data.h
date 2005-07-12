// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-14 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Log_Data.h $
// $Author: Ape $
// $Modtime: 7/04/05 11:21 $
// $Revision: 3 $
// $Workfile: ADN_Log_Data.h $
//
// *****************************************************************************

#ifndef __ADN_Log_Data_h_
#define __ADN_Log_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"

class ADN_XmlInput_Helper;


// =============================================================================
/** @class  ADN_Log_Data
    @brief  ADN_Log_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_Log_Data
: public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_Log_Data )

public:
// *****************************************************************************
    class WorkTimeModifiersInfo
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( WorkTimeModifiersInfo )
    public:
        WorkTimeModifiersInfo( E_TempsBordee nType );

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        E_TempsBordee nType_;

        ADN_Type_Double rRepairModifier_;
        ADN_Type_Double rTreatModifier_;
        ADN_Type_Double rSortModifier_;
    };

    typedef ADN_Type_Vector_ABC< WorkTimeModifiersInfo > T_WorkTimeModifiersInfo_Vector;
    typedef T_WorkTimeModifiersInfo_Vector::iterator     IT_WorkTimeModifiersInfo_Vector;


// *****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
    ADN_Log_Data();
    ~ADN_Log_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( T_StringList& vFiles ) const;
    void Reset();
    //@}

private:
    void ReadArchive( ADN_XmlInput_Helper& input );
    void WriteArchive( MT_OutputArchive_ABC& output );

public:
    T_WorkTimeModifiersInfo_Vector vWorkTimeModifiers_;
};

#endif // __ADN_Log_Data_h_
