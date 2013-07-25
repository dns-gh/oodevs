// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalCapacity_h_
#define __MedicalCapacity_h_

#include "ObjectCapacity_ABC.h"

#include <queue>

class MedicalTreatmentAttribute;

// =============================================================================
/** @class  MedicalCapacity
    @brief  MedicalCapacity
*/
// Created: RFT 2008-05-30
// =============================================================================
class MedicalCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MedicalCapacity();
    explicit MedicalCapacity( xml::xistream& xis );

    virtual ~MedicalCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    //@}

private:
    //! @name Copy
    //@{
    MedicalCapacity( const MedicalCapacity& );
    //@}

    //! @name Helpers
    //@{
    void InitializeData( xml::xistream& xis );
    //@}
};

BOOST_CLASS_EXPORT_KEY( MedicalCapacity )

#endif // __MedicalCapacity_h_
