// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-21 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_KnowledgeGroups_Data.h $
// $Author: Ape $
// $Modtime: 7/04/05 11:21 $
// $Revision: 2 $
// $Workfile: ADN_KnowledgeGroups_Data.h $
//
// *****************************************************************************

#ifndef __ADN_KnowledgeGroups_Data_h_
#define __ADN_KnowledgeGroups_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
//#include "ADN_Enums.h"

class ADN_XmlInput_Helper;


// =============================================================================
/** @class  ADN_KnowledgeGroups_Data
    @brief  ADN_KnowledgeGroups_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_KnowledgeGroups_Data
    : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_KnowledgeGroups_Data )

public:
// *****************************************************************************
    class GroupInfo
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( GroupInfo )

    public:
        GroupInfo();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_String strName_;

        ADN_Type_Double rMaxLifetime_;
        ADN_Type_Double rMaxDistance_;
        ADN_Type_Bool   bInterpolationTime_;
        ADN_Type_Double rInterpolationTime_;
    };

    typedef ADN_Type_Vector_ABC< GroupInfo > T_GroupInfoVector;
    typedef T_GroupInfoVector::iterator     IT_GroupInfoVector;


// *****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
     ADN_KnowledgeGroups_Data();
    ~ADN_KnowledgeGroups_Data();
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
    T_GroupInfoVector vGroups_;
};


#endif // __ADN_KnowledgeGroups_Data_h_
