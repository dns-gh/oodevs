// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BridgingCapacity_h_
#define __BridgingCapacity_h_

#include "MIL.h"
#include "ObjectCapacity_ABC.h"
#include "MIL_DynamicPathDataHandler.h"

class Object;
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
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

	//! @name Operations
    //@{
    virtual void Instanciate( Object& object ) const;
    virtual void Register( Object& object );
    virtual void Finalize( Object& object );
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

#endif