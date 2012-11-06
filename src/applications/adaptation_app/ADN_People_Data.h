// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_People_Data_h_
#define __ADN_People_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Type_Repartition.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Population_Data.h"
#include "ADN_ResourceNetworks_Data.h"
#include <map>
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ADN_People_Data
*/
// Created: SLG 2010-11-22
// =============================================================================
class ADN_People_Data : public ADN_Data_ABC
{
public:
// *****************************************************************************
    class EventInfos : public ADN_Ref_ABC
    {
    public:
         EventInfos();
        ~EventInfos();

        EventInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Enum< E_Days, eNbrDays > day_;
        ADN_Type_String from_;
        ADN_Type_String to_;
        ADN_TypePtr_InVector_ABC< ADN_Urban_Data::AccommodationInfos > ptrAccommodation_;
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC< EventInfos >, EventInfosVector )

    class PeopleInfosConsumption
        : public ADN_Ref_ABC
    {

    public:
                 PeopleInfosConsumption();
        virtual ~PeopleInfosConsumption() {}
        std::string GetItemName();

        PeopleInfosConsumption* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_TypePtr_InVector_ABC< ADN_ResourceNetworks_Data::ResourceNetworkInfos > ptrResource_;
        ADN_Type_Int consumption_;
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC< PeopleInfosConsumption >, PeopleInfosConsumptionVector )

// *****************************************************************************
    class PeopleInfos : public ADN_RefWithName
    {
    public:
        PeopleInfos();
        PeopleInfos( unsigned int );
        ~PeopleInfos();

        PeopleInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    private:
        void ReadEvent( xml::xistream& input );
        void ReadConsumption( xml::xistream& input );
        const std::string CheckErrors() const;

    public:
        ADN_Type_Int nId_;
        ADN_TypePtr_InVector_ABC<ADN_Population_Data::PopulationInfos> ptrModel_;
        ADN_Type_String strAngryCrowdMission_;
        ADN_Type_Repartition repartition_;
        ADN_Type_Time transferTime_;
        ADN_Type_Double securityLossOnFire_;
        ADN_Type_Double securityGainPerHour_;
        ADN_Type_Int healthNeed_;
        T_EventInfosVector schedule_;
        T_PeopleInfosConsumptionVector consumptions_;
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC< PeopleInfos >, PeopleInfosVector )

// *****************************************************************************
public:
             ADN_People_Data();
    virtual ~ADN_People_Data();

    void Reset();
    void FilesNeeded(T_StringList& l) const;

    T_PeopleInfosVector& GetPeople();
    PeopleInfos* FindPeople( const std::string& strName );

    QStringList GetPeopleThatUse( ADN_Population_Data::PopulationInfos& population );
    QStringList GetPeopleThatUse( ADN_ResourceNetworks_Data::ResourceNetworkInfos& network );

private:
    void ReadArchive( xml::xistream& input );
    void ReadPeople( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    T_PeopleInfosVector vPeople_;

private:
    static tools::IdManager idManager_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::GetPopulation
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
inline
ADN_People_Data::T_PeopleInfosVector& ADN_People_Data::GetPeople()
{
    return vPeople_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FindPopulation
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
inline
ADN_People_Data::PeopleInfos* ADN_People_Data::FindPeople( const std::string& strName )
{
    IT_PeopleInfosVector it = std::find_if( vPeople_.begin(), vPeople_.end(), ADN_Tools::NameCmp< PeopleInfos >( strName ) );
    if( it == vPeople_.end() )
        return 0;
    return *it;
}

#endif // __ADN_People_Data_h_
