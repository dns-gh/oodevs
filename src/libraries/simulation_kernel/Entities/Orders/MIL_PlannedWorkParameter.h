// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_PlannedWorkParameter_h_
#define __MIL_PlannedWorkParameter_h_

#include "MIL_BaseParameter.h"

class DEC_Gen_Object;
class MIL_EntityManager_ABC;
class ObjectTypeResolver_ABC;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_PlannedWorkParameter
    @brief  MIL_PlannedWorkParameter
*/
// Created: LDC 2009-06-04
// =============================================================================
class MIL_PlannedWorkParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_PlannedWorkParameter();
             MIL_PlannedWorkParameter( const sword::PlannedWork& asn, const MIL_EntityManager_ABC& entityManager,
                                       const ObjectTypeResolver_ABC& resolver );
             MIL_PlannedWorkParameter( const sword::PlannedWork& asn, const MIL_EntityManager_ABC& entityManager, unsigned int identifier,
                                       const ObjectTypeResolver_ABC& resolver );
    explicit MIL_PlannedWorkParameter( boost::shared_ptr< DEC_Gen_Object > param );
    virtual ~MIL_PlannedWorkParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToGenObject( boost::shared_ptr< DEC_Gen_Object >& ) const;
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
    boost::shared_ptr< DEC_Gen_Object > pGenObject_;
    unsigned int identifier_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_PlannedWorkParameter )

#endif // __MIL_PlannedWorkParameter_h_
