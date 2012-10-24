// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Missions_Data_h_
#define __ADN_Missions_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_Drawings_Data.h"
#include "ADN_MissionGenObjectTypes_Infos.h"
#include "ADN_Missions_ParameterValue.h"
#include "ADN_Missions_Type.h"
#include "tools/IdManager.h"

enum E_EntityType;

namespace xml { class xistream; }

// =============================================================================
/** @class  ADN_Missions_Data
    @brief  ADN_Missions_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_Missions_Data : public ADN_Data_ABC
{
// =============================================================================
// Mission parameters
// =============================================================================
public:
    typedef ADN_Type_Vector_ABC<ADN_Missions_ParameterValue>  T_MissionParameterValue_Vector;
    typedef T_MissionParameterValue_Vector::iterator          IT_MissionParameterValue_Vector;
    typedef ADN_Type_Vector_ABC<ADN_Missions_Type>            T_Choice_Vector;
    typedef T_Choice_Vector::iterator                         IT_Choice_Vector;

public:
    class MissionParameter : public ADN_Ref_ABC
                           , public ADN_DataTreeNode_ABC
    {
    public:
                 MissionParameter();
        virtual ~MissionParameter();

        std::string GetItemName();
        MissionParameter* CreateCopy();

        void ReadArchive ( xml::xistream& input );
        void ReadValue   ( xml::xistream& input );
        template< typename T >
        void ReadChoice  ( xml::xistream& input, T& data );
        void WriteArchive( xml::xostream& output );

    private:
        void FillChoices();
        void FillGenObjects();
        bool HasGenObjects() const;

    public:
        ADN_Type_String                                                   strMission_;
        ADN_Type_String                                                   strName_;
        ADN_Type_Enum< E_MissionParameterType, eNbrMissionParameterType > type_;
        ADN_Type_Bool                                                     isOptional_;
        ADN_Type_Int                                                      minOccurs_;
        ADN_Type_Int                                                      maxOccurs_;
        ADN_Type_Int                                                      minValue_;
        ADN_Type_Int                                                      maxValue_;
        T_MissionParameterValue_Vector                                    values_;
        T_Choice_Vector                                                   choices_;
        helpers::T_MissionGenObjectTypes_Infos_Vector                     genObjects_;
        ADN_Type_String                                                   diaName_;
    };

    typedef ADN_Type_Vector_ABC<MissionParameter>  T_MissionParameter_Vector;
    typedef T_MissionParameter_Vector::iterator   IT_MissionParameter_Vector;

// =============================================================================
// Missions
// =============================================================================
public:
    class Mission : public ADN_Ref_ABC
                  , public ADN_DataTreeNode_ABC
    {
    public:
                 Mission();
        explicit Mission( unsigned int id );
        virtual ~Mission();

        std::string GetItemName();
        Mission* CreateCopy();

        void ReadArchive ( xml::xistream& input, std::size_t contextLength, E_EntityType modelType );
        void ReadParameter( xml::xistream& input, std::size_t& index, std::size_t contextLength );
        void WriteArchive( xml::xostream& output, const std::string& type, const T_MissionParameter_Vector& context );

        void ReadMissionSheet ( E_EntityType type );
        void RemoveDifferentNamedMissionSheet ( E_EntityType type );
        void WriteMissionSheet ( E_EntityType type );
        std::string FromEntityTypeToRepository( E_EntityType type);

    public:
        ADN_Type_Int id_;
        ADN_Type_String strName_;
        T_MissionParameter_Vector parameters_;
        ADN_Type_String diaType_;
        ADN_Type_String diaBehavior_;
        ADN_Type_String cdtDiaBehavior_;
        ADN_Type_String mrtDiaBehavior_;
        ADN_Type_String missionSheetContent_;
        ADN_Type_String missionSheetPath_;
        ADN_Type_String strPackage_;
        ADN_TypePtr_InVector_ABC< ADN_Drawings_Data::DrawingInfo > symbol_;
    };

    typedef ADN_Type_Vector_ABC<Mission>         T_Mission_Vector;
    typedef T_Mission_Vector::iterator          IT_Mission_Vector;
    typedef T_Mission_Vector::const_iterator   CIT_Mission_Vector;

// =============================================================================
// Frag orders
// =============================================================================
public:
    class FragOrder : public ADN_Ref_ABC
                    , public ADN_DataTreeNode_ABC
    {
    public:
                 FragOrder();
        explicit FragOrder( unsigned int id );
        virtual ~FragOrder();

        std::string GetItemName();
        FragOrder* CreateCopy();

        void ReadArchive ( xml::xistream& input );
        void ReadParameter( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

        void ReadMissionSheet ();
        void RemoveDifferentNamedMissionSheet ();
        void WriteMissionSheet ();

    public:
        ADN_Type_Int              id_;
        ADN_Type_String           strName_;
        T_MissionParameter_Vector parameters_;
        ADN_Type_String           diaType_;
        ADN_Type_Bool             isAvailableWithoutMission_;
        ADN_Type_String           missionSheetContent_;
        ADN_Type_String           missionSheetPath_;
    };

    typedef ADN_Type_Vector_ABC<FragOrder>  T_FragOrder_Vector;
    typedef T_FragOrder_Vector::iterator   IT_FragOrder_Vector;

// =============================================================================
// Main data
// =============================================================================
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Missions_Data();
    virtual ~ADN_Missions_Data();
    //@}

    //! @name Operations
    //@{
    virtual void FilesNeeded( T_StringList& vFiles ) const;
    virtual void Reset();

    T_FragOrder_Vector& GetFragOrders();
    T_Mission_Vector&   GetUnitMissions();
    T_Mission_Vector&   GetAutomatMissions();
    T_Mission_Vector&   GetPopulationMissions();
    FragOrder*          FindFragOrder( const std::string& strName );
    Mission*            FindMission( T_Mission_Vector& missions, const std::string& strName );
    virtual void Load( const tools::Loader_ABC& fileLoader );
    virtual void Initialize();

    QStringList         GetAllMissionsThatUse( ADN_Objects_Data_ObjectInfos& object ); // $$$$ ABR 2012-08-03: Warning, return not only the name, but concatenation of tab name and mission name

    QStringList         GetUnitMissionsThatUse( ADN_Objects_Data_ObjectInfos& object );
    QStringList         GetAutomatMissionsThatUse( ADN_Objects_Data_ObjectInfos& object );
    QStringList         GetPopulationMissionsThatUse( ADN_Objects_Data_ObjectInfos& object );
    QStringList         GetFragOrdersThatUse( ADN_Objects_Data_ObjectInfos& object );

    void NotifyElementDeleted( std::string elementName, E_EntityType elementType );
    //@}

private:
    void ReadArchive( xml::xistream& input );
    void ReadFragOrder( xml::xistream& input );
    void ReadMission( xml::xistream& input, T_Mission_Vector& missions, std::size_t contextLength, E_EntityType modelType );
    void ReadContext( xml::xistream& input, T_MissionParameter_Vector& context );
    void ReadContextParameter( xml::xistream& input, T_MissionParameter_Vector& context );
    void WriteArchive( xml::xostream& output );
    void MoveMissionSheetsToObsolete( std::string file );

public:
    T_MissionParameter_Vector   unitContext_;
    T_Mission_Vector            unitMissions_;
    T_MissionParameter_Vector   automatContext_;
    T_Mission_Vector            automatMissions_;
    T_MissionParameter_Vector   populationContext_;
    T_Mission_Vector            populationMissions_;
    T_FragOrder_Vector          fragOrders_;
    T_StringList toDeleteMissionSheets_;

private:
    static tools::IdManager idManager_;
};

#endif // __ADN_Missions_Data_h_
