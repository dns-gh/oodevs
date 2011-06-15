// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_DotationTypeParameter_h_
#define __MIL_DotationTypeParameter_h_

#include "MIL_BaseParameter.h"

class PHY_DotationCategory;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_DotationTypeParameter
    @brief  MIL_DotationTypeParameter
*/
// Created: LDC 2009-06-05
// =============================================================================
class MIL_DotationTypeParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_DotationTypeParameter();
    explicit MIL_DotationTypeParameter( const sword::ResourceType & asn );
    explicit MIL_DotationTypeParameter( const PHY_DotationCategory* pCategory );
    virtual ~MIL_DotationTypeParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToDotationType( const PHY_DotationCategory*& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@}

private:
    //! @name Member data
    //@{
    const PHY_DotationCategory* pCategory_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_DotationTypeParameter )

#endif // __MIL_DotationTypeParameter_h_
