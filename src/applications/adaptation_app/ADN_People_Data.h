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

#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Type_VectorFixed_ABC.h"
#include "ADN_Models_Data.h"
#include "ADN_Population_Data.h"

class xml::xistream;

// =============================================================================
/** @class  ADN_People_Data
*/
// Created: SLG 2010-11-22
// =============================================================================
class ADN_People_Data : public ADN_Data_ABC
{

public:
// *****************************************************************************
    class PeopleInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {

    public:
        PeopleInfos();
        ~PeopleInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        PeopleInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output, int mosId );

    public:
        ADN_Type_String                                       strName_;
        ADN_TypePtr_InVector_ABC<ADN_Population_Data::PopulationInfos> ptrModel_;
        ADN_Type_Int male_; 
        ADN_Type_Int female_; 
        ADN_Type_Int children_;
        ADN_Type_String transfertTime_;
    };

    typedef ADN_Type_Vector_ABC< PeopleInfos >  T_PeopleInfosVector;
    typedef T_PeopleInfosVector::iterator      IT_PeopleInfosVector;

// *****************************************************************************
public:
    ADN_People_Data();
    virtual ~ADN_People_Data();

    void                        Reset();
    void                        FilesNeeded(T_StringList& l) const;

    T_PeopleInfosVector&      GetPeople();
    PeopleInfos*              FindPeople( const std::string& strName );

private:
    void ReadArchive ( xml::xistream& input );
    void ReadPeople  ( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    T_PeopleInfosVector   vPeople_;
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
    IT_PeopleInfosVector it = std::find_if( vPeople_.begin(), vPeople_.end(), ADN_Tools::NameCmp<PeopleInfos>( strName ) );
    if( it == vPeople_.end() )
        return 0;
    return *it;
}


#endif // __ADN_People_Data_h_
