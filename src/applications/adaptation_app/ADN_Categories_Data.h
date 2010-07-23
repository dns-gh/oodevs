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
#include "ADN_ArmorInfos.h"

class xml::xistream;


//*****************************************************************************
// Created: JDY 03-08-27
//*****************************************************************************
class ADN_Categories_Data : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED(ADN_Categories_Data)

public:
    typedef ADN_Type_String SizeInfos;
    typedef ADN_Type_Vector_ABC< SizeInfos >         T_SizeInfos_Vector;
    typedef T_SizeInfos_Vector::iterator            IT_SizeInfos_Vector;
    typedef ADN_Type_String DotationNatureInfos;
    typedef ADN_Type_Vector_ABC< DotationNatureInfos > T_DotationNatureInfos_Vector;
    typedef T_DotationNatureInfos_Vector::iterator    IT_DotationNatureInfos_Vector;

public:
             ADN_Categories_Data();
    virtual ~ADN_Categories_Data();

    void FilesNeeded(T_StringList& l) const;
    void Reset();
    void Load();
    void Save();

    helpers::T_ArmorInfos_Vector& GetArmorsInfos();
    T_SizeInfos_Vector&           GetSizesInfos();
    T_DotationNatureInfos_Vector& GetDotationNaturesInfos();
    helpers::ArmorInfos*          FindArmor( const std::string& strName );
    SizeInfos*                    FindSize( const std::string& strName );
    DotationNatureInfos*          FindDotationNature( const std::string& strName );

private:
    void ReadVolume( xml::xistream& input );
    void ReadSizes( xml::xistream& input );
    void ReadProtection( xml::xistream& input );
    void ReadArmors( xml::xistream& input );
    void ReadNature( xml::xistream& input );
    void ReadDotationNatures( xml::xistream& input );

    void WriteSizes( xml::xostream& output );
    void WriteArmors( xml::xostream& output );
    void WriteDotationNatures( xml::xostream& output );

private:
    helpers::T_ArmorInfos_Vector vArmors_;
    T_SizeInfos_Vector  vSizes_;
    T_DotationNatureInfos_Vector vDotationNatures_;
};


//-----------------------------------------------------------------------------
// Name: helpers::T_ArmorInfos_Vector
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
inline
helpers::T_ArmorInfos_Vector& ADN_Categories_Data::GetArmorsInfos()
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
// Name: ADN_Categories_Data::GetDotationNaturesInfos
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
inline
ADN_Categories_Data::T_DotationNatureInfos_Vector& ADN_Categories_Data::GetDotationNaturesInfos()
{
    return vDotationNatures_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::FindArmor
// Created: APE 2004-12-03
// -----------------------------------------------------------------------------
inline
helpers::ArmorInfos* ADN_Categories_Data::FindArmor( const std::string& strName )
{
    helpers::IT_ArmorInfos_Vector it = std::find_if( vArmors_.begin(), vArmors_.end(), ADN_Tools::NameCmp< helpers::ArmorInfos >( strName ) );
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
// Name: ADN_Categories_Data::FindDotationNature
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
inline
ADN_Categories_Data::DotationNatureInfos* ADN_Categories_Data::FindDotationNature( const std::string& strName )
{
    for( IT_DotationNatureInfos_Vector it = vDotationNatures_.begin(); it != vDotationNatures_.end(); ++it )
        if( ADN_Tools::CaselessCompare( (*it)->GetData(), strName ) )
            return *it;
    return 0;
}

#endif // __ADN_Categories_Data_h_