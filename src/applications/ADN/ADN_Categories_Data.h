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

#ifndef __ADN_Categories_Data_h_
#define __ADN_Categories_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Enums.h"
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Tools.h"

class ADN_XmlInput_Helper;


//*****************************************************************************
// Created: JDY 03-08-27
//*****************************************************************************
class ADN_Categories_Data : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED(ADN_Categories_Data)

public:
//*****************************************************************************
    class ArmorInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ArmorInfos )

    public:
        ArmorInfos();

        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& );
        void WriteArchive( MT_OutputArchive_ABC& );

    public:
        ADN_Type_String strName_;
        ADN_Type_Enum< E_ProtectionType, eNbrProtectionType > nType_;
        ADN_Type_Time   neutralizationAverageTime_;
        ADN_Type_Time   neutralizationVariance_;
        ADN_Type_Double rBreakdownEVA_;
        ADN_Type_Double rBreakdownNEVA_;
        ADN_Type_Double rPercentageWoundedHumansEvac_;
        ADN_Type_Double rPercentageWoundedHumansNoEvac_;
    };

    typedef ADN_Type_Vector_ABC<ArmorInfos>        T_ArmorInfos_Vector;
    typedef T_ArmorInfos_Vector::iterator          IT_ArmorInfos_Vector;


//*****************************************************************************
    
    typedef ADN_Type_String SizeInfos;
    typedef ADN_Type_Vector_ABC<SizeInfos>         T_SizeInfos_Vector;
    typedef T_SizeInfos_Vector::iterator           IT_SizeInfos_Vector;


//*****************************************************************************
public:

    ADN_Categories_Data();
    virtual ~ADN_Categories_Data();
    
    void            FilesNeeded(T_StringList& l) const;
    void            Reset();
    void            Load();
    void            Save();

    T_ArmorInfos_Vector&    GetArmorsInfos();
    T_SizeInfos_Vector&     GetSizesInfos();
    ArmorInfos*             FindArmor( const std::string& strName );
    SizeInfos*              FindSize( const std::string& strName );

private:
    void ReadSizes( ADN_XmlInput_Helper& input );
    void ReadArmors( ADN_XmlInput_Helper& input );

    void WriteSizes( MT_OutputArchive_ABC& output );
    void WriteArmors( MT_OutputArchive_ABC& output );


private:
    T_ArmorInfos_Vector vArmors_;
    T_SizeInfos_Vector  vSizes_;
};


//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data::T_ArmorInfos_Vector
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
inline
ADN_Categories_Data::T_ArmorInfos_Vector& ADN_Categories_Data::GetArmorsInfos()
{
    return vArmors_;
}


//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data::T_SizeInfos_Vector
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
inline
ADN_Categories_Data::T_SizeInfos_Vector& ADN_Categories_Data::GetSizesInfos()
{
    return vSizes_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::FindArmor
// Created: APE 2004-12-03
// -----------------------------------------------------------------------------
inline
ADN_Categories_Data::ArmorInfos* ADN_Categories_Data::FindArmor( const std::string& strName )
{
    IT_ArmorInfos_Vector it = std::find_if( vArmors_.begin(), vArmors_.end(), ADN_Tools::NameCmp<ArmorInfos>(strName) );
    if( it == vArmors_.end() )
        return 0;
    return *it;
}


// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::FindSize
// Created: APE 2004-12-03
// -----------------------------------------------------------------------------
inline
ADN_Categories_Data::SizeInfos* ADN_Categories_Data::FindSize( const std::string& strName )
{
    for( IT_SizeInfos_Vector it = vSizes_.begin(); it != vSizes_.end(); ++it )
        if( ADN_Tools::CaselessCompare( (*it)->GetData(), strName ) )
            return *it;
    return 0;
}


// -----------------------------------------------------------------------------
// Name: ArmorInfos::GetItemName
// Created: APE 2004-11-10
// -----------------------------------------------------------------------------
inline
std::string ADN_Categories_Data::ArmorInfos::GetItemName()
{
    return std::string();
}


#endif // __ADN_Categories_Data_h_