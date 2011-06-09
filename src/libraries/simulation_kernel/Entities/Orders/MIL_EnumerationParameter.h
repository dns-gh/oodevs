// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_EnumerationParameter_h_
#define __MIL_EnumerationParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_EnumerationParameter
    @brief  MIL_EnumerationParameter
*/
// Created: LDC 2009-05-22
// =============================================================================
class MIL_EnumerationParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_EnumerationParameter();
    explicit MIL_EnumerationParameter( int );
    virtual ~MIL_EnumerationParameter();
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

BOOST_CLASS_EXPORT_KEY( MIL_EnumerationParameter )

#endif // __MIL_EnumerationParameter_h_
