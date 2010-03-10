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

#ifndef __ADN_Zurb_Data_h_
#define __ADN_Zurb_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Enums.h"
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Tools.h"

class xml::xistream;

//*****************************************************************************
// Created: JDY 03-08-27
//*****************************************************************************
class ADN_Zurb_Data : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED(ADN_Zurb_Data)

public:
    
    typedef ADN_Type_String ZurbInfos;
    typedef ADN_Type_Vector_ABC<ZurbInfos>         T_ZurbInfos_Vector;
    typedef T_ZurbInfos_Vector::iterator           IT_ZurbInfos_Vector;
    
public:

             ADN_Zurb_Data();
    virtual ~ADN_Zurb_Data();
    
    void            FilesNeeded(T_StringList& l) const;
    void            Reset();
    void            Load();
    void            Save();

    T_ZurbInfos_Vector&           GetMaterialsInfos();  
    ZurbInfos*                    FindMaterial( const std::string& strName );
    T_ZurbInfos_Vector&           GetFacadesInfos();  
    ZurbInfos*                    FindFacade( const std::string& strName );
    T_ZurbInfos_Vector&           GetRoofShapesInfos();  
    ZurbInfos*                    FindRoofShape( const std::string& strName );


private:
    void ReadZurb( xml::xistream& input );
    void WriteZurb( xml::xostream& output );
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
    T_ZurbInfos_Vector  vMaterials_;
    T_ZurbInfos_Vector  vRoofShapes_;
    T_ZurbInfos_Vector  vFacades_;
};



//-----------------------------------------------------------------------------
// Name: ADN_Zurb_Data::GetMaterialsInfos
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
inline
ADN_Zurb_Data::T_ZurbInfos_Vector& ADN_Zurb_Data::GetMaterialsInfos()
{
    return vMaterials_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Zurb_Data::FindMaterial
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
inline
ADN_Zurb_Data::ZurbInfos* ADN_Zurb_Data::FindMaterial( const std::string& strName )
{
    for( IT_ZurbInfos_Vector it = vMaterials_.begin(); it != vMaterials_.end(); ++it )
        if( ADN_Tools::CaselessCompare( (*it)->GetData(), strName ) )
            return *it;
    return 0;
}

//-----------------------------------------------------------------------------
// Name: ADN_Zurb_Data::GetFacadesInfos
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
inline
ADN_Zurb_Data::T_ZurbInfos_Vector& ADN_Zurb_Data::GetFacadesInfos()
{
    return vFacades_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Zurb_Data::FindFacade
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
inline
ADN_Zurb_Data::ZurbInfos* ADN_Zurb_Data::FindFacade( const std::string& strName )
{
    for( IT_ZurbInfos_Vector it = vFacades_.begin(); it != vFacades_.end(); ++it )
        if( ADN_Tools::CaselessCompare( (*it)->GetData(), strName ) )
            return *it;
    return 0;
}

//-----------------------------------------------------------------------------
// Name: ADN_Zurb_Data::GetRoofShapesInfos
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
inline
ADN_Zurb_Data::T_ZurbInfos_Vector& ADN_Zurb_Data::GetRoofShapesInfos()
{
    return vRoofShapes_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Zurb_Data::FindRoofShape
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
inline
ADN_Zurb_Data::ZurbInfos* ADN_Zurb_Data::FindRoofShape( const std::string& strName )
{
    for( IT_ZurbInfos_Vector it = vRoofShapes_.begin(); it != vRoofShapes_.end(); ++it )
        if( ADN_Tools::CaselessCompare( (*it)->GetData(), strName ) )
            return *it;
    return 0;
}

#endif // __ADN_Zurb_Data_h_