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

#include "ADN_CrossedRef.h"
#include "ADN_Resources_Data.h"

// =============================================================================
/** @class  ADN_ExtinguisherAgentInfos
    @brief  ADN_ExtinguisherAgentInfos
    @see    4311340-Modèles de feu.doc
*/
// Created: JSR 2010-12-01
// =============================================================================
class ADN_ExtinguisherAgentInfos : public ADN_CrossedRef< ADN_Resources_Data::CategoryInfo >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ExtinguisherAgentInfos( ADN_Resources_Data::CategoryInfo* agent );
    virtual ~ADN_ExtinguisherAgentInfos();
    //@}

    //! @name Operations
    //@{
    void CopyFrom( ADN_ExtinguisherAgentInfos& other );
    void ReadArchive( xml::xistream& );
    void WriteArchive( xml::xostream&, const std::string& tag = "extinguisher-agent" ) const;
    //@}

public:
    typedef ADN_Resources_Data::CategoryInfo T_Item;

    class CmpRef : public std::unary_function< ADN_ExtinguisherAgentInfos*, bool >
    {
    public:
        CmpRef( ADN_Resources_Data::CategoryInfo* val ) : val_( val ) {}
        bool operator()( ADN_ExtinguisherAgentInfos* other ) const
        {
            return other->GetCrossedElement() == val_;
        }

    private:
        ADN_Resources_Data::CategoryInfo* val_;
    };

    class Cmp : public std::unary_function< ADN_ExtinguisherAgentInfos*, bool >
    {
    public:
        Cmp( const std::string& name ) : name_( name ) {}
        bool operator()( ADN_ExtinguisherAgentInfos* other ) const
        {
            return other->GetCrossedElement() ? other->GetCrossedElement()->strName_ == name_ : false;
        }

    private:
        std::string name_;
    };

public:
    //! @name Member data
    //@{
    ADN_Type_Int heatDecreaseRate_;
    //@}
};

typedef ADN_Type_VectorFixed_ABC< ADN_ExtinguisherAgentInfos > T_ExtinguisherAgentInfos_Vector;

#endif // __ADN_ExtinguisherAgentInfos_h_
