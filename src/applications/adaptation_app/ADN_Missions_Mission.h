// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef ADAPTATION_ADN_MISSIONS_MISSION_H
#define ADAPTATION_ADN_MISSIONS_MISSION_H

#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_Drawings_Data.h"
#include "ADN_Missions_Parameter.h"
#include "clients_gui/WikiXmlConverter.h"
#include <boost/regex.hpp>

enum E_EntityType;

namespace xml { class xistream; }

class ADN_Missions_Mission : public ADN_RefWithName
{
public:
    class ADN_Missions_Attachment : public ADN_RefWithName
    {
    public:
        ADN_Missions_Attachment(){}
        ADN_Missions_Attachment( const std::string& name )
        {
            strName_ = name;
        }
        virtual ~ADN_Missions_Attachment(){}
    };

    typedef ADN_Type_Vector_ABC<ADN_Missions_Attachment>       T_MissionAttachment_Vector;
    typedef T_MissionAttachment_Vector::iterator              IT_MissionAttachment_Vector;

    typedef ADN_Type_Vector_ABC<ADN_Missions_Parameter>       T_MissionParameter_Vector;
    typedef T_MissionParameter_Vector::iterator              IT_MissionParameter_Vector;

             ADN_Missions_Mission();
    explicit ADN_Missions_Mission( unsigned int id );
    virtual ~ADN_Missions_Mission();

    std::string GetItemName();
    ADN_Missions_Mission* CreateCopy();

    void FillContextParameters( E_EntityType entityType );

    void ReadArchive( xml::xistream& input, ADN_Drawings_Data& drawings, const std::string& missionDir );
    void ReadParameter( xml::xistream& input );
    void WriteArchive( xml::xostream& output, const std::string& type );

    void CheckMissionDataConsistency( ADN_ConsistencyChecker& checker, E_EntityType type );
    void CheckFieldDataConsistency( std::string fieldData, ADN_ConsistencyChecker& checker, E_EntityType type );
    void ReadMissionSheetParametersDescriptions( xml::xistream& xis );
    void ReadMissionSheetAttachments( xml::xistream& xis );
    void WriteMissionSheetParametersDescriptions( xml::xostream& xos );
    void WriteMissionSheetAttachments( xml::xostream& xos );
    bool IsFileInAttachmentList( const std::string& fileName );
    void ReadMissionSheet( const std::string& missionDir );
    void RemoveDifferentNamedMissionSheet( const std::string& missionDir );
    void WriteMissionSheet( const std::string& missionDir );
    bool IsEmptyMissionSheet();
    bool IsEmptyParameterList();

private:
    void AddContextParameter( E_ContextParameters contextType, E_MissionParameterType parameterType, bool optional, int minOccurs = 1, int maxOccurs = 1 );

public:
    ADN_Type_Int id_;
    T_MissionParameter_Vector parameters_;
    ADN_Type_String diaType_;
    ADN_Type_String diaBehavior_;
    ADN_Type_String cdtDiaBehavior_;
    ADN_Type_String mrtDiaBehavior_;
    ADN_Type_String strPackage_;
    ADN_TypePtr_InVector_ABC< ADN_Drawings_Data::DrawingInfo > symbol_;
    std::string doctrine_;
    std::string usage_;

    //Missions sheets descriptions data
    ADN_Type_String missionSheetPath_;
    ADN_Type_String descriptionContext_;
    ADN_Type_String descriptionBehavior_;
    ADN_Type_String descriptionSpecific_;
    ADN_Type_String descriptionComment_;
    ADN_Type_String descriptionMissionEnd_;
    T_MissionAttachment_Vector attachments_; 
};

#endif // ADAPTATION_ADN_MISSIONS_MISSION_H
