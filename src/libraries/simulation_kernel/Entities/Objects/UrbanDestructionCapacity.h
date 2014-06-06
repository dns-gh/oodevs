// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanDestructionCapacity_h_
#define __UrbanDestructionCapacity_h_

#include "MIL.h"
#include "ObjectCapacity_ABC.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

class MIL_Object_ABC;
class PHY_UrbanAttritionData;

// =============================================================================
/** @class  UrbanDestructionCapacity
    @brief  UrbanDestructionCapacity
*/
// Created: JCR 2008-05-30
// =============================================================================
class UrbanDestructionCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanDestructionCapacity();
    explicit UrbanDestructionCapacity( const PHY_UrbanAttritionData& data );
    explicit UrbanDestructionCapacity( xml::xistream& xis );
    virtual ~UrbanDestructionCapacity();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void Update( MIL_Object_ABC& object, unsigned int time );
    //@}

    //! @name Copy/Assignment
    //@{
    UrbanDestructionCapacity( const UrbanDestructionCapacity& from );
    UrbanDestructionCapacity& operator=( const UrbanDestructionCapacity& from );
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< PHY_UrbanAttritionData > data_;
    bool appliedOnce_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( UrbanDestructionCapacity )

#endif // __UrbanDestructionCapacity_h_
