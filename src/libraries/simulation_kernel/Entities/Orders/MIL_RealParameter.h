// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_RealParameter_h_
#define __MIL_RealParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_RealParameter
    @brief  MIL real parameter
*/
// Created: LDC 2009-05-22
// =============================================================================
class MIL_RealParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_RealParameter( float value );
    virtual ~MIL_RealParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToNumeric( float& value ) const;
    virtual bool ToElement( sword::MsgMissionParameter_Value& elem ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_RealParameter( const MIL_RealParameter& );            //!< Copy constructor
    MIL_RealParameter& operator=( const MIL_RealParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    float value_;
    //@}
};

#endif // __MIL_RealParameter_h_
