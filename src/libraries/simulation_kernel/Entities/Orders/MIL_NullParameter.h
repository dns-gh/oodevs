// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_NullParameter_h_
#define __MIL_NullParameter_h_

#include "MIL_BaseParameter.h"

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_NullParameter
    @brief  MIL_NullParameter
*/
// Created: LDC 2009-05-20
// =============================================================================
class MIL_NullParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_NullParameter();
    virtual ~MIL_NullParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToElement( sword::MissionParameter_Value& ) const;
    virtual bool ToList( std::vector< sword::MissionParameter_Value >& ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_NullParameter )

#endif // __MIL_NullParameter_h_
