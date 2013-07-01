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
#include "ADN_Activities_Data.h"
#include "ADN_MissionGenObjectTypes_Infos.h"
#include "tools/IdManager.h"

class ADN_Objects_Data_ObjectInfos;

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
    class MissionParameterValue : public ADN_Ref_ABC
                                , public ADN_DataTreeNode_ABC
    {
    public:
                 MissionParameterValue();
        virtual ~MissionParameterValue();

        std::string GetItemName();
        MissionParameterValue* CreateCopy();

        void ReadArchive ( xml::xistream& input );
        void WriteArchive( xml::xostream& output, unsigned int id );

    public:
        ADN_Type_String name_;
    };
    class MissionType : public ADN_Ref_ABC
                      , public ADN_DataTreeNode_ABC
    {
    public:
                 MissionType();
        explicit MissionType( const std::string& name );
        virtual ~MissionType();

        std::string GetItemName();
        const std::string& GetInputName() const;
        MissionType* CreateCopy();

        void WriteArchive( xml::xostream& output );
    public:
        std::string name_;
        std::string displayName_;
        ADN_Type_Bool isAllowed_;
    };

    typedef ADN_Type_Vector_ABC<MissionParameterValue>  T_MissionParameterValue_Vector;
    typedef T_MissionParameterValue_Vector::iterator   IT_MissionParameterValue_Vector;
    typedef ADN_Type_Vector_ABC<MissionType>            T_Choice_Vector;
    typedef T_Choice_Vector::iterator                  IT_Choice_Vector;

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

    public:
        ADN_Type_String                                                   strName_;
        ADN_Type_Enum< E_MissionParameterType, eNbrMissionParameterType > type_;
        ADN_Type_Enum< E_AnchorType, eNbrAnchorType >                     anchor_;
        ADN_Type_Bool                                                     isOptional_;
        ADN_Type_Int                                                      minOccurs_;
        ADN_Type_Int                                                      maxOccurs_;
        ADN_Type_Int                                                      minValue_;
        ADN_Type_Int                                                      maxValue_;
        T_MissionParameterValue_Vector                                    values_;
        T_Choice_Vector                                                   choices_;
        helpers::T_MissionGenObjectTypes_Infos_Vector                     knowledgeObjects_;
        ADN_Type_String                                                   diaName_;
    };

    typedef ADN_Type_Vector_ABC<MissionParameter>        T_MissionParameter_Vector;
    typedef T_MissionParameter_Vector::iterator         IT_MissionParameter_Vector;
    typedef T_MissionParameter_Vector::const_iterator  CIT_MissionParameter_Vector;

// =============================================================================
// ADN_Missions_ABC
// =============================================================================
public:
    class ADN_Missions_ABC : public ADN_Ref_ABC
                      , public ADN_DataTreeNode_ABC
    {
    public:
        ADN_Missions_ABC();
        ADN_Missions_ABC( unsigned int id );
        ~ADN_Missions_ABC();

        std::string GetItemName();
        virtual void ReadArchive ( xml::xistream&, std::size_t ) { assert( 0 ); };
        virtual void WriteArchive( xml::xostream&, E_MissionType, const T_MissionParameter_Vector* ) { assert( 0 ); };
        virtual ADN_Missions_ABC* CreateCopy() { return 0; };
        // $$$$ ABR 2013-05-21: TODO: Fill theses methods with common Mission and FragOrder implementation

    public:
        ADN_Type_Int              id_;
        ADN_Type_String           strName_;
        T_MissionParameter_Vector parameters_;
        ADN_Type_String           diaType_;
        ADN_Type_String           doctrineDescription_;
        ADN_Type_String           usageDescription_;
    };
    typedef ADN_Type_Vector_ABC<ADN_Missions_ABC>    T_Mission_ABC_Vector;
    typedef T_Mission_ABC_Vector::iterator          IT_Mission_ABC_Vector;
    typedef T_Mission_ABC_Vector::const_iterator   CIT_Mission_ABC_Vector;

// =============================================================================
// Missions
// =============================================================================
public:
    class Mission : public ADN_Missions_ABC
    {
    public:
                 Mission();
        explicit Mission( unsigned int id );
        virtual ~Mission();

        virtual ADN_Missions_ABC* CreateCopy();

        void ReadParameter( xml::xistream& input, std::size_t& index, std::size_t contextLength );
        virtual void ReadArchive ( xml::xistream& input, std::size_t contextLength );
        virtual void WriteArchive( xml::xostream& output, E_MissionType type, const T_MissionParameter_Vector* context );

    public:
        ADN_Type_String diaBehavior_;
        ADN_Type_String cdtDiaBehavior_;
        ADN_Type_String mrtDiaBehavior_;
        ADN_TypePtr_InVector_ABC< ADN_Activities_Data::PackageInfos > strPackage_;
        ADN_TypePtr_InVector_ABC< ADN_Drawings_Data::DrawingInfo > symbol_;
    };

    typedef ADN_Type_Vector_ABC<Mission>         T_Mission_Vector;
    typedef T_Mission_Vector::iterator          IT_Mission_Vector;
    typedef T_Mission_Vector::const_iterator   CIT_Mission_Vector;

// =============================================================================
// Frag orders
// =============================================================================
public:
    class FragOrder : public ADN_Missions_ABC
    {
    public:
                 FragOrder();
        explicit FragOrder( unsigned int id );
        virtual ~FragOrder();

        virtual ADN_Missions_ABC* CreateCopy();

        void ReadParameter( xml::xistream& input );
        virtual void ReadArchive ( xml::xistream& input, std::size_t contextLength );
        virtual void WriteArchive( xml::xostream& output, E_MissionType type, const T_MissionParameter_Vector* context );

    public:
        ADN_Type_Bool             isAvailableWithoutMission_;
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

    T_Mission_ABC_Vector& GetFragOrders();
    T_Mission_ABC_Vector& GetUnitMissions();
    T_Mission_ABC_Vector& GetAutomatMissions();
    T_Mission_ABC_Vector& GetPopulationMissions();
    FragOrder*          FindFragOrder( const std::string& strName );
    Mission*            FindMission( T_Mission_ABC_Vector& missions, const std::string& strName );
    virtual void Load( const tools::Loader_ABC& fileLoader );
    QStringList GetUnitMissionsThatUse( ADN_Activities_Data::PackageInfos& package );
    QStringList GetAutomataMissionsThatUse( ADN_Activities_Data::PackageInfos& package );
    //@}

private:
    void ReadArchive( xml::xistream& input );
    void ReadActivity( xml::xistream& input );
    void ReadFragOrder( xml::xistream& input );
    void ReadMission( xml::xistream& input, T_Mission_ABC_Vector& missions, std::size_t contextLength );
    void ReadContext( xml::xistream& input, T_MissionParameter_Vector& context );
    void ReadContextParameter( xml::xistream& input, T_MissionParameter_Vector& context );
    void WriteArchive( xml::xostream& output );
    void WriteActivityArchive();
    void CheckAndFixLoadingErrors() const;

public:
    std::vector< T_Mission_ABC_Vector > missionsVector_;
    T_MissionParameter_Vector           unitContext_;
    T_MissionParameter_Vector           automatContext_;
    T_MissionParameter_Vector           populationContext_;
    std::auto_ptr< ADN_Activities_Data > activitiesData_;

private:
    static tools::IdManager idManager_;
};

#endif // __ADN_Missions_Data_h_
