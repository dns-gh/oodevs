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

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

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
             MIL_IntegerParameter();
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
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@}

private:
    //! @name Member data
    //@{
    int value_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_IntegerParameter )

#endif // __MIL_IntegerParameter_h_
