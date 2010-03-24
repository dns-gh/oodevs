//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Categories_Data.h $
// $Author: Ape $
// $Modtime: 13/04/05 15:10 $
// $Revision: 8 $
// $Workfile: ADN_Categories_Data.h $
//
//*****************************************************************************

#ifndef __ADN_Urban_Data_h_
#define __ADN_Urban_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Enums.h"
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Tools.h"

class xml::xistream;

//*****************************************************************************
// Created: JDY 03-08-27
//*****************************************************************************
class ADN_Urban_Data : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED(ADN_Urban_Data)

public:
    
    typedef ADN_Type_String UrbanInfos;
    typedef ADN_Type_Vector_ABC<UrbanInfos>         T_UrbanInfos_Vector;
    typedef T_UrbanInfos_Vector::iterator           IT_UrbanInfos_Vector;
    
public:

             ADN_Urban_Data();
    virtual ~ADN_Urban_Data();
    
    void            FilesNeeded(T_StringList& l) const;
    void            Reset();
    void            Load();
    void            Save();

    T_UrbanInfos_Vector&           GetMaterialsInfos();  
    UrbanInfos*                    FindMaterial( const std::string& strName );
    T_UrbanInfos_Vector&           GetFacadesInfos();  
    UrbanInfos*                    FindFacade( const std::string& strName );
    T_UrbanInfos_Vector&           GetRoofShapesInfos();  
    UrbanInfos*                    FindRoofShape( const std::string& strName );


private:
    void ReadUrban( xml::xistream& input );
    void WriteUrban( xml::xostream& output );
    void ReadMaterial( xml::xistream& input );
    void ReadMaterials( xml::xistream& input );
    void WriteMaterials( xml::xostream& output );
    void ReadFacade( xml::xistream& input );
    void ReadFacades( xml::xistream& input );
    void WriteFacades( xml::xostream& output );
    void ReadRoofShape( xml::xistream& input );
    void ReadRoofShapes( xml::xistream& input );
    void WriteRoofShapes( xml::xostream& output );

private:
    T_UrbanInfos_Vector  vMaterials_;
    T_UrbanInfos_Vector  vRoofShapes_;
    T_UrbanInfos_Vector  vFacades_;
};



//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetMaterialsInfos
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
inline
ADN_Urban_Data::T_UrbanInfos_Vector& ADN_Urban_Data::GetMaterialsInfos()
{
    return vMaterials_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::FindMaterial
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
inline
ADN_Urban_Data::UrbanInfos* ADN_Urban_Data::FindMaterial( const std::string& strName )
{
    for( IT_UrbanInfos_Vector it = vMaterials_.begin(); it != vMaterials_.end(); ++it )
        if( ADN_Tools::CaselessCompare( (*it)->GetData(), strName ) )
            return *it;
    return 0;
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetFacadesInfos
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
inline
ADN_Urban_Data::T_UrbanInfos_Vector& ADN_Urban_Data::GetFacadesInfos()
{
    return vFacades_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::FindFacade
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
inline
ADN_Urban_Data::UrbanInfos* ADN_Urban_Data::FindFacade( const std::string& strName )
{
    for( IT_UrbanInfos_Vector it = vFacades_.begin(); it != vFacades_.end(); ++it )
        if( ADN_Tools::CaselessCompare( (*it)->GetData(), strName ) )
            return *it;
    return 0;
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetRoofShapesInfos
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
inline
ADN_Urban_Data::T_UrbanInfos_Vector& ADN_Urban_Data::GetRoofShapesInfos()
{
    return vRoofShapes_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::FindRoofShape
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
inline
ADN_Urban_Data::UrbanInfos* ADN_Urban_Data::FindRoofShape( const std::string& strName )
{
    for( IT_UrbanInfos_Vector it = vRoofShapes_.begin(); it != vRoofShapes_.end(); ++it )
        if( ADN_Tools::CaselessCompare( (*it)->GetData(), strName ) )
            return *it;
    return 0;
}

#endif // __ADN_Urban_Data_h_