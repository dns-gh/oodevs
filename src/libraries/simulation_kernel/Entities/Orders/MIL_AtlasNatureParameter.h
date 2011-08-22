// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_AtlasNatureParameter_h_
#define __MIL_AtlasNatureParameter_h_

#include "MIL_BaseParameter.h"

namespace sword
{
    class Nature;
}

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_AtlasNatureParameter
    @brief  MIL_AtlasNatureParameter
*/
// Created: LDC 2009-05-25
// =============================================================================
class MIL_AtlasNatureParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_AtlasNatureParameter();
    explicit MIL_AtlasNatureParameter( const sword::Nature& );
    explicit MIL_AtlasNatureParameter( int nature );
    virtual ~MIL_AtlasNatureParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    virtual bool ToNatureAtlas( int& atlas ) const;
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
    int nature_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_AtlasNatureParameter )

#endif // __MIL_AtlasNatureParameter_h_
