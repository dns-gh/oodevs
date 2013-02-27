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
#include "ADN_Drawings_Data.h"
#include "ADN_Missions_Parameter.h"

#include <boost/noncopyable.hpp>
// =============================================================================
/** @class  ADN_Missions_ABC
    @brief  ADN_Missions_ABC
*/
// Created: NPT 2013-02-14
// =============================================================================
class ADN_Missions_ABC : private boost::noncopyable
                       , public ADN_RefWithName
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

        typedef ADN_Type_Vector_ABC<ADN_Missions_Attachment>      T_MissionAttachment_Vector;
        typedef T_MissionAttachment_Vector::iterator             IT_MissionAttachment_Vector;

        typedef ADN_Type_Vector_ABC<ADN_Missions_Parameter>       T_MissionParameter_Vector;
        typedef T_MissionParameter_Vector::iterator              IT_MissionParameter_Vector;

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Missions_ABC();
             ADN_Missions_ABC( unsigned int id );
    virtual ~ADN_Missions_ABC();
    //@}

    //! @name Operations
    //@{
    std::string GetItemName();
    virtual ADN_Missions_ABC* CreateCopy() { return 0; }

    void FillContextParameters( E_EntityType entityType );

    virtual void ReadArchive( xml::xistream& input, const std::string& missionDir );
    virtual void WriteArchive( xml::xostream& output, const std::string& name );

    void ReadParameter( xml::xistream& input );
    void CheckMissionDataConsistency( ADN_ConsistencyChecker& checker, E_MissionType type );
    void CheckFieldDataConsistency( std::string fieldData, ADN_ConsistencyChecker& checker, E_MissionType type );
    void ReadMissionSheetParametersDescriptions( xml::xistream& xis );
    void ReadMissionSheetAttachments( xml::xistream& xis );
    void WriteMissionSheetParametersDescriptions( xml::xostream& xos );
    void WriteMissionSheetAttachments( xml::xostream& xos );
    bool IsFileInAttachmentList( const std::string& fileName );
    void ReadMissionSheet( const std::string& missionDir );
    void RenameDifferentNamedMissionSheet( const std::string& missionDir );
    bool WriteMissionSheet( const std::string& missionDir, std::string fileName, int type );
    bool IsEmptyMissionSheet();
    bool IsEmptyParameterList();

private:
    void AddContextParameter( E_ContextParameters contextType, E_MissionParameterType parameterType, bool optional, int minOccurs = 1, int maxOccurs = 1 );
    //@}

public:
    //! @name Member data
    //@{
    ADN_Type_Int              id_;
    T_MissionParameter_Vector parameters_;
    ADN_Type_String           diaType_;

    //Missions sheets descriptions data
    ADN_Type_String missionSheetPath_;
    ADN_Type_String descriptionContext_;
    ADN_Type_String descriptionBehavior_;
    ADN_Type_String descriptionSpecific_;
    ADN_Type_String descriptionComment_;
    ADN_Type_String descriptionMissionEnd_;
    T_MissionAttachment_Vector attachments_;
    bool needSheetSaving_;
    //@}
};

#endif // __ADN_Missions_ABC_h_
