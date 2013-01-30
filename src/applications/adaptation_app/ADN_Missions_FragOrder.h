// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef ADAPTATION_ADN_MISSIONS_FRAGORDER_H
#define ADAPTATION_ADN_MISSIONS_FRAGORDER_H

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_Missions_ParameterValue.h"
#include "ADN_Missions_Parameter.h"
#include <boost/regex.hpp>

namespace xml
{
    class xistream;
    class xostream;
}

class ADN_Missions_FragOrder : public ADN_RefWithName
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
    typedef T_MissionParameter_Vector::iterator               IT_MissionParameter_Vector;

    typedef std::pair< const boost::regex, const std::string >             T_Regex;

             ADN_Missions_FragOrder();
    explicit ADN_Missions_FragOrder( unsigned int id );
    virtual ~ADN_Missions_FragOrder();

    std::string GetItemName();
    ADN_Missions_FragOrder* CreateCopy();

    void ReadArchive( xml::xistream& input, const std::string& missionDir );
    void ReadParameter( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

    void CheckMissionDataConsistency( ADN_ConsistencyChecker& checker );
    void CheckFieldDataConsistency( std::string fieldData, ADN_ConsistencyChecker& checker );
    void ReadMissionSheetParametersDescriptions( xml::xistream& xis );
    void ReadMissionSheetAttachments( xml::xistream& xis );
    void WriteMissionSheetParametersDescriptions( xml::xostream& xos );
    void WriteMissionSheetAttachments( xml::xostream& xos );
    bool IsFileInAttachmentList( const std::string& fileName );
    void MakeStringXmlItem( xml::xostream& xos, std::size_t length, std::string line );
    void ReadXmlLine( const std::string& tag, xml::xistream& xis, std::string& text );
    void ReadXmlList( const std::string& tag, xml::xistream& xis, std::string& text, int& level );
    void FromXmlToWiki( const std::string& tag, xml::xistream& xis, std::string& text ); 
    void FromWikiToXml( xml::xostream& xos, const std::string& text );
    void ReadMissionSheet( const std::string& missionDir );
    void RemoveDifferentNamedMissionSheet( const std::string& missionDir );
    void WriteMissionSheet( const std::string& missionDir, const tools::Loader_ABC& fileLoader );

public:
    ADN_Type_Int              id_;
    T_MissionParameter_Vector parameters_;
    ADN_Type_String           diaType_;
    ADN_Type_Bool             isAvailableWithoutMission_;

    //Missions sheets descriptions data
    ADN_Type_String missionSheetPath_;
    ADN_Type_String descriptionContext_;
    ADN_Type_String descriptionBehavior_;
    ADN_Type_String descriptionSpecific_;
    ADN_Type_String descriptionComment_;
    ADN_Type_String descriptionMissionEnd_;
    T_MissionAttachment_Vector attachments_; 
};

#endif // ADAPTATION_ADN_MISSIONS_FRAGORDER_H
