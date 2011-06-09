// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_IntegerParameter_h_
#define __MIL_IntegerParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_IntegerParameter
    @brief  MIL_IntegerParameter
*/
// Created: LDC 2011-05-09
// =============================================================================
class MIL_IntegerParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_IntegerParameter( int );
    virtual ~MIL_IntegerParameter();
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
    //@}

private:
    //! @name Member data
    //@{
    int value_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_IntegerParameter )

#endif // __MIL_IntegerParameter_h_
