// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_BoolParameter_h_
#define __MIL_BoolParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_BoolParameter
    @brief  MIL_BoolParameter
*/
// Created: LDC 2009-05-22
// =============================================================================
class MIL_BoolParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_BoolParameter();
    explicit MIL_BoolParameter( bool );
    virtual ~MIL_BoolParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToBool( bool& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name Member data
    //@{
    bool value_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_BoolParameter )

#endif // __MIL_BoolParameter_h_
