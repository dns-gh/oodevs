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

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

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
             MIL_RealParameter();
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
    virtual bool ToId( int& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@

private:
    //! @name Member data
    //@{
    float value_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_RealParameter )

#endif // __MIL_RealParameter_h_
