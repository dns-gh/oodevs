// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_UrbanBlockParameter_h_
#define __MIL_UrbanBlockParameter_h_

#include "MIL_BaseParameter.h"

class MIL_EntityManager_ABC;
class MIL_UrbanObject_ABC;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

namespace sword
{
    class Id;
}

// =============================================================================
/** @class  MIL_UrbanBlockParameter
    @brief  MIL_UrbanBlockParameter
*/
// Created: MGD 2009-11-02
// =============================================================================
class MIL_UrbanBlockParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_UrbanBlockParameter();
             MIL_UrbanBlockParameter( const sword::Id& asn, const MIL_EntityManager_ABC& entityManager );
    explicit MIL_UrbanBlockParameter( MIL_UrbanObject_ABC* pUrbanBlock );
    virtual ~MIL_UrbanBlockParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToUrbanBlock( MIL_UrbanObject_ABC*& ) const;
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
    MIL_UrbanObject_ABC* pUrbanBlock_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_UrbanBlockParameter )

#endif // __MIL_UrbanBlockParameter_h_
