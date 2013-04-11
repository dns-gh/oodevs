// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Weapons_Data_PhInfos_h_
#define __ADN_Weapons_Data_PhInfos_h_

#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Categories_Data.h"

namespace xml { class xistream; }

// =============================================================================
/** @class  ADN_Weapons_Data_PhInfos
*/
// Created: APE 2004-11-19
// =============================================================================
class ADN_Weapons_Data_PhInfos : public ADN_Ref_ABC
{
public:
    ADN_Weapons_Data_PhInfos();

    ADN_Weapons_Data_PhInfos* CreateCopy();
    void SetPhModifiers( double phModifier, double distModifier );
    void ApplyPhModifiers();
    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    ADN_Type_Int       nDistance_;
    ADN_Type_Double    rPerc_;
    ADN_Type_Int       nModifiedDistance_;
    ADN_Type_Double    rModifiedPerc_;
    double             phModifier_;
    double             distModifier_;
};

// =============================================================================
/** @class  ADN_Weapons_Data_PhSizeInfos
*/
// Created: APE 2004-11-19
// =============================================================================
class ADN_Weapons_Data_PhSizeInfos : public ADN_Ref_ABC
{
public:
    explicit ADN_Weapons_Data_PhSizeInfos( ADN_Categories_Data::SizeInfos *ptr );
    virtual ~ADN_Weapons_Data_PhSizeInfos();

    void ReadArchive( xml::xistream& input );
    void ReadHp( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    ADN_TypePtr_InVector_ABC< ADN_Categories_Data::SizeInfos > ptrSize_;
    ADN_Type_Vector_ABC< ADN_Weapons_Data_PhInfos > vPhs_;

public:
    typedef ADN_Categories_Data::SizeInfos  T_Item;

    class Cmp : public std::unary_function< ADN_Weapons_Data_PhSizeInfos*, bool >
    {
    public:
        Cmp( const std::string& val ) : val_( val ) {}
        ~Cmp() {}

        bool operator()( ADN_Weapons_Data_PhSizeInfos* tgtnfos ) const
        { return tgtnfos->ptrSize_.GetData() && tgtnfos->ptrSize_.GetData()->strName_.GetData()==val_; }

    private:
        std::string val_;
    };

    class CmpRef : public std::unary_function< ADN_Weapons_Data_PhSizeInfos*, bool >
    {
    public:
        CmpRef( ADN_Categories_Data::SizeInfos* val ) : val_( val ) {}
        ~CmpRef(){}

        bool operator()( ADN_Weapons_Data_PhSizeInfos* tgtnfos ) const
        { return tgtnfos->ptrSize_.GetData() == val_; }

    private:
        ADN_Categories_Data::SizeInfos* val_;
    };
};

#endif // __ADN_Weapons_Data_PhInfos_h_
