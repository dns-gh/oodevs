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
    explicit DetectorAttribute( const MIL_Agent_ABC& Agent );
    virtual ~DetectorAttribute();
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( Object& object ) const;
    //@}

    //! @name Operations
    //@{
    void AddDetector( const MIL_Agent_ABC& agent );
    const MIL_Agent_ABC* GetDetector() const;
    //@}

private:
    //! @name Member data
    //@{
    const MIL_Agent_ABC*    detector_;
    //@}
};

#endif // __DetectorAttribute_h_
