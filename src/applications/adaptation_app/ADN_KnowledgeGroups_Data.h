// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_KnowledgeGroups_Data_h_
#define __ADN_KnowledgeGroups_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Types.h"

// =============================================================================
/** @class  ADN_KnowledgeGroups_Data
    @brief  ADN_KnowledgeGroups_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_KnowledgeGroups_Data : public ADN_Data_ABC
{
public:
// *****************************************************************************
    class AgentGroupInfo : public ADN_Ref_ABC
    {
    public:
        AgentGroupInfo();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Time maxLifetime_;
        ADN_Type_Double rMaxDistance_;
        ADN_Type_Bool bInterpolationTime_;
        ADN_Type_Time interpolationTime_;
    };

// *****************************************************************************
    class PopulationGroupInfo : public ADN_Ref_ABC
    {
    public:
        PopulationGroupInfo();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Time maxLifetime_;
    };

// *****************************************************************************
    class GroupInfo : public ADN_RefWithLocalizedName
    {

    public:
        GroupInfo();

        GroupInfo* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Time communicationDelay_; // LTO
        AgentGroupInfo agentInfos_;
        PopulationGroupInfo populationInfos_;
    };

    typedef ADN_Type_Vector_ABC< GroupInfo > T_GroupInfoVector;

// *****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
             ADN_KnowledgeGroups_Data();
    virtual ~ADN_KnowledgeGroups_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( tools::Path::T_Paths& vFiles ) const;
    //@}

    //! @name Accessors
    //@{
    T_GroupInfoVector& GetGroupInfos();
    //@}

private:
    //! @name Helpers
    //@{
    void ReadArchive( xml::xistream& input );
    void ReadKnowledgeGroup( xml::xistream& input );
    void WriteArchive( xml::xostream& output );
    //@}

public:
    //! @name Member data
    //@{
    T_GroupInfoVector vGroups_;
    //@}

};

#endif // __ADN_KnowledgeGroups_Data_h_
