// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __DetectorAttribute_h_
#define __DetectorAttribute_h_

#include "ObjectAttribute_ABC.h"
#include <boost/serialization/export.hpp>

class MIL_Agent_ABC;

// =============================================================================
/** @class  DetectorAttribute
@brief  DetectorAttribute
*/
// Created: SLG 2010-02-16
// =============================================================================
class DetectorAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DetectorAttribute();
    explicit DetectorAttribute( const MIL_Agent_ABC* Agent );
    virtual ~DetectorAttribute();
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name Operations
    //@{
    void AddDetector( const MIL_Agent_ABC& agent );
    const MIL_Agent_ABC* GetDetector() const;
    //@}

private:
    //! @name Serialization
    //@{
    template< typename Archive > friend void save_construct_data( Archive& archive, const DetectorAttribute* attr, const unsigned int /*version*/ );
    template< typename Archive > friend void load_construct_data( Archive& archive, DetectorAttribute* attr, const unsigned int /*version*/ );
    //@}

private:
    //! @name Member data
    //@{
    const MIL_Agent_ABC* detector_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DetectorAttribute )

#endif // __DetectorAttribute_h_
