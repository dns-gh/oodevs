// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_TirIndirectParameter_h_
#define __MIL_TirIndirectParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_TirIndirectParameter
    @brief  MIL_TirIndirectParameter
*/
// Created: LDC 2009-06-05
// =============================================================================
class MIL_TirIndirectParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_TirIndirectParameter( const sword::FireId& fire );
    virtual ~MIL_TirIndirectParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToId( int& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@

private:
    //! @name Member data
    //@{
    int data_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_TirIndirectParameter )

#endif // __MIL_TirIndirectParameter_h_
