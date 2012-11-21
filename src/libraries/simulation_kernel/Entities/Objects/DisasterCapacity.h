// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisasterCapacity_h_
#define __DisasterCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

namespace xml
{
    class xistream;
}

class MIL_DisasterType;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  DisasterCapacity
    @brief  Disaster capacity
*/
// Created: LGY 2012-11-20
// =============================================================================
class DisasterCapacity : public ObjectCapacity_ABC
                       , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DisasterCapacity();
    explicit DisasterCapacity( xml::xistream& xis );
    virtual ~DisasterCapacity();
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

private:
    //! @name Copy
    //@{
    DisasterCapacity( const DisasterCapacity& from );
    //@}

private:
    //! @name Member data
    //@{
    std::string type_;
    boost::shared_ptr< const MIL_DisasterType > disasterType_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DisasterCapacity )

#endif
