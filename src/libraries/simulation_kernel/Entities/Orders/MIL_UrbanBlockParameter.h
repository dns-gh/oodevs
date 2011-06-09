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
class UrbanObjectWrapper;

namespace sword
{
    class UrbanObjectKnowledgeId;
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
             MIL_UrbanBlockParameter( const sword::UrbanObjectKnowledgeId& asn, MIL_EntityManager_ABC& entityManager );
    explicit MIL_UrbanBlockParameter( UrbanObjectWrapper* pUrbanBlock );
    virtual ~MIL_UrbanBlockParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToUrbanBlock( UrbanObjectWrapper*& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@

private:
    //! @name Member data
    //@{
    UrbanObjectWrapper* pUrbanBlock_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_UrbanBlockParameter )

#endif // __MIL_UrbanBlockParameter_h_
