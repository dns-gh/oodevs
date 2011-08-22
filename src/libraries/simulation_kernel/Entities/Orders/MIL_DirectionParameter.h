// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_DirectionParameter_h_
#define __MIL_DirectionParameter_h_

#include "MIL_BaseParameter.h"

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_DirectionParameter
    @brief  MIL_DirectionParameter
*/
// Created: LDC 2009-05-26
// =============================================================================
class MIL_DirectionParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_DirectionParameter();
    explicit MIL_DirectionParameter( const sword::Heading& heading );
    explicit MIL_DirectionParameter( boost::shared_ptr< MT_Vector2D >& dir );
    virtual ~MIL_DirectionParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    virtual bool ToDirection( boost::shared_ptr< MT_Vector2D >& ) const;
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
    int heading_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_DirectionParameter )

#endif // __MIL_DirectionParameter_h_
