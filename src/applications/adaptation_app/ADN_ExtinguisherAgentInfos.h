// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ExtinguisherAgentInfos_h_
#define __ADN_ExtinguisherAgentInfos_h_

#include "ADN_Equipement_Data.h"

// =============================================================================
/** @class  ADN_ExtinguisherAgentInfos
    @brief  ADN_ExtinguisherAgentInfos
    @see    4311340-Modèles de feu.doc
*/
// Created: JSR 2010-12-01
// =============================================================================
class ADN_ExtinguisherAgentInfos : public ADN_Ref_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ExtinguisherAgentInfos( ADN_Equipement_Data::CategoryInfo* agent );
    virtual ~ADN_ExtinguisherAgentInfos();
    //@}

    //! @name Operations
    //@{
    void CopyFrom( ADN_ExtinguisherAgentInfos& other );
    void ReadArchive( xml::xistream& );
    void WriteArchive( xml::xostream&, const std::string& tag = "extinguisher-agent" );
    //@}

public:
    //! @name Operators
    //@{
    xml::xostream& operator<<( xml::xostream& xos );
    //@}

public:
    typedef ADN_Equipement_Data::CategoryInfo T_Item;

    class CmpRef : public std::unary_function< ADN_ExtinguisherAgentInfos* , bool >
    {
    public:
        CmpRef( ADN_Equipement_Data::CategoryInfo* val ) : val_( val ){}
        bool operator()( ADN_ExtinguisherAgentInfos* other ) const
        { return other->ptrAgent_.GetData() == val_; }

    private:
        ADN_Equipement_Data::CategoryInfo* val_;
    };

    class Cmp : public std::unary_function< ADN_ExtinguisherAgentInfos* , bool >
    {
    public:
        Cmp( const std::string& name ) : name_( name ) {}
        bool operator()( ADN_ExtinguisherAgentInfos* other ) const
        { return other->ptrAgent_.GetData()->strName_ == name_; }

    private:
        std::string name_;
    };

public:
    //! @name Member data
    //@{
    ADN_TypePtr_InVector_ABC< ADN_Equipement_Data::CategoryInfo > ptrAgent_;
    ADN_Type_Int heatDecreaseRate_;
    //@}
};

typedef ADN_Type_VectorFixed_ABC< ADN_ExtinguisherAgentInfos > T_ExtinguisherAgentInfos_Vector;
typedef T_ExtinguisherAgentInfos_Vector::iterator             IT_ExtinguisherAgentInfos_Vector;

#endif // __ADN_ExtinguisherAgentInfos_h_
