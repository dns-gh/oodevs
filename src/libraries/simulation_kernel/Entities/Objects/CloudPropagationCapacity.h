// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __CloudPropagationCapacity_h_
#define __CloudPropagationCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"
#include "MT_Tools/MT_Vector2D.h"

class TER_Localisation;

// =============================================================================
/** @class  CloudPropagationCapacity
    @brief  Cloud propagation capacity
*/
// Created: LGY 2011-07-04
// =============================================================================
class CloudPropagationCapacity : public ObjectCapacity_ABC
                               , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CloudPropagationCapacity();
    virtual ~CloudPropagationCapacity();
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void Finalize( MIL_Object_ABC& object );
    virtual void Update( MIL_Object_ABC& object, unsigned int time );
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CloudPropagationCapacity( const CloudPropagationCapacity& );            //!< Copy constructor
    //@}

    //! @name Helpers
    //@{
    bool UpdateLocalisation( double angle, TER_Localisation& newLocalisation );
    //@}

private:
    //! @name Member data
    //@{
    MT_Vector2D origin_;
    unsigned int time_;
    double rCurrentCircleRadius_;
    double rCurrentPropagationLenght_;
    double length_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( CloudPropagationCapacity )

#endif // __CloudPropagationCapacity_h_
