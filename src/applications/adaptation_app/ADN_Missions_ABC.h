// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Missions_ABC_h_
#define __ADN_Missions_ABC_h_

#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_Missions_Parameter.h"

// =============================================================================
/** @class  ADN_Missions_ABC
    @brief  ADN_Missions_ABC
*/
// Created: NPT 2013-02-14
// =============================================================================
class ADN_Missions_ABC : public ADN_RefWithLocalizedName
    {
    public:
        class ADN_Missions_Attachment : public ADN_RefWithName
        {
        public:
            ADN_Missions_Attachment(){}
            ADN_Missions_Attachment( const tools::Path& name )
            {
                strName_ = name.ToUTF8();
            }
            virtual ~ADN_Missions_Attachment(){}
        };

        typedef ADN_Type_Vector_ABC< ADN_Missions_Attachment > T_MissionAttachment_Vector;
        typedef ADN_Type_Vector_ABC< ADN_Missions_Parameter > T_MissionParameter_Vector;

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Missions_ABC();
    explicit ADN_Missions_ABC( E_MissionType type );
             ADN_Missions_ABC( E_MissionType type, unsigned int id );
    virtual ~ADN_Missions_ABC();
    //@}

    //! @name Operations
    //@{
    virtual ADN_Missions_ABC* CreateCopy() { return 0; }

    void FillContextParameters();

    virtual void ReadArchive( xml::xistream& input, const tools::Path& missionDir );
    virtual void WriteArchive( xml::xostream& output );

    void ReadParameter( xml::xistream& input );
    void CheckMissionDataConsistency( ADN_ConsistencyChecker& checker );
    void CheckFieldDataConsistency( const std::string& fieldData, ADN_ConsistencyChecker& checker );
    void ReadMissionSheetParametersDescriptions( xml::xistream& xis );
    void ReadMissionSheetAttachments( xml::xistream& xis );
    void WriteMissionSheetParametersDescriptions( xml::xostream& xos );
    void WriteMissionSheetAttachments( xml::xostream& xos );
    bool IsFileInAttachmentList( const std::string& fileName );
    void ReadMissionSheet( const tools::Path& missionDir );
    void RenameDifferentNamedMissionSheet( const tools::Path& missionDir );
    bool WriteMissionSheet( const tools::Path& missionDir, const tools::Path& fileName, int type );
    bool IsEmptyMissionSheet();
    bool IsEmptyParameterList();
    bool NeedsSaving();
    void SetNeedsSaving( bool saving );
    void ParseImagesInImageDirectory( const tools::Path& imageDir );

private:
    void AddContextParameter( E_ContextParameters contextType, E_MissionParameterType parameterType, bool optional, int minOccurs = 1, int maxOccurs = 1 );
    //@}

public:
    //! @name Member data
    //@{
    ADN_Type_Int              id_;
    T_MissionParameter_Vector parameters_;
    ADN_Type_String           diaType_;
    E_MissionType             type_;

    //Missions sheets descriptions data
    ADN_Type_String missionSheetPath_;
    ADN_Type_String descriptionContext_;
    ADN_Type_String descriptionBehavior_;
    ADN_Type_String descriptionSpecific_;
    ADN_Type_String descriptionComment_;
    ADN_Type_String descriptionMissionEnd_;
    T_MissionAttachment_Vector attachments_;

private:
    bool needSheetSaving_;
    //@}
};

#endif // __ADN_Missions_ABC_h_
