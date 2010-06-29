// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __BridgingCapacity_h_
#define __BridgingCapacity_h_

#include "MIL.h"
#include "ObjectCapacity_ABC.h"
#include "MIL_DynamicPathDataHandler.h"

class MIL_Object_ABC;
class TER_DynamicData;

// =============================================================================
/** @class  BridgingCapacity
    @brief  BridgingCapacity
*/
// Created: JCR 2008-05-30
// =============================================================================
class BridgingCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BridgingCapacity();
    explicit BridgingCapacity( xml::xistream& xis );
    virtual ~BridgingCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void Finalize( MIL_Object_ABC& object );
    //@}

private:
    //! @name Copy
    //@{
    BridgingCapacity( const BridgingCapacity& from );
    //@}

    //! @name Helpers
    //@{
    void CreateBridgeGeometry( const T_PointVector& points, T_PointVector& output ) const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_DynamicPathDataHandler handler_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( BridgingCapacity )

#endif
