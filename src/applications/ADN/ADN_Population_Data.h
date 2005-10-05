// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-02 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Population_Data.h $
// $Author: Ape $
// $Modtime: 7/06/05 10:08 $
// $Revision: 9 $
// $Workfile: ADN_Population_Data.h $
//
// *****************************************************************************

#ifndef __ADN_Population_Data_h_
#define __ADN_Population_Data_h_

#include "ADN_Data_ABC.h"

#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Models_Data.h"

class ADN_XmlInput_Helper;


// =============================================================================
/** @class  ADN_Population_Data
*/
// Created: APE 2004-12-02
// =============================================================================
class ADN_Population_Data : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_Population_Data )

public:
// *****************************************************************************
    class PopulationInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( PopulationInfos )

    public:
        PopulationInfos();
        ~PopulationInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        PopulationInfos* CreateCopy();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output, int nMosId );

    public:
        ADN_Type_String                                       strName_;
        ADN_TypePtr_InVector_ABC<ADN_Models_Data::ModelInfos> ptrModel_;
        ADN_Type_Double                                       rConcentrationDensity_;
        ADN_Type_Double                                       rMoveDensity_;
        ADN_Type_Double                                       rMoveSpeed_;
    };

    typedef ADN_Type_Vector_ABC<PopulationInfos>  T_PopulationInfosVector;
    typedef T_PopulationInfosVector::iterator      IT_PopulationInfosVector;


// *****************************************************************************
public:
    ADN_Population_Data();
    virtual ~ADN_Population_Data();

    void                        FilesNeeded(T_StringList& l) const;
    void                        Reset();

    T_PopulationInfosVector&  GetPopulation();
    PopulationInfos*          FindPopulation( const std::string& strName );

private:
    void ReadArchive( ADN_XmlInput_Helper& input );
    void WriteArchive( MT_OutputArchive_ABC& output );

public:
    T_PopulationInfosVector  vPopulation_;
};


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::GetPopulation
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
inline
ADN_Population_Data::T_PopulationInfosVector& ADN_Population_Data::GetPopulation()
{
    return vPopulation_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FindPopulation
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
inline
ADN_Population_Data::PopulationInfos* ADN_Population_Data::FindPopulation( const std::string& strName )
{
    IT_PopulationInfosVector it = std::find_if( vPopulation_.begin(), vPopulation_.end(), ADN_Tools::NameCmp<PopulationInfos>( strName ) );
    if( it == vPopulation_.end() )
        return 0;
    return *it;
}


#endif // __ADN_Population_Data_h_
