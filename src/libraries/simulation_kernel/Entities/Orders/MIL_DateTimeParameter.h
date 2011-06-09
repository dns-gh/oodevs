// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_DateTimeParameter_h_
#define __MIL_DateTimeParameter_h_

#include "MIL_BaseParameter.h"

class PHY_DotationCategory;

// =============================================================================
/** @class  MIL_DateTimeParameter
    @brief  MIL_DateTimeParameter
*/
// Created: LDC 2009-06-05
// =============================================================================
class MIL_DateTimeParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_DateTimeParameter();
    explicit MIL_DateTimeParameter( const sword::DateTime& asn );
    virtual ~MIL_DateTimeParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToGDH( float& value ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int dateTime_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_DateTimeParameter )

#endif // __MIL_DateTimeParameter_h_
