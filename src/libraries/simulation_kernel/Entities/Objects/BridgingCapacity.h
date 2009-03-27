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

class TER_DynamicData;
class Object;

class BridgingCapacity
    : public ObjectCapacity_ABC
{
public:
	//! @name Constructors/Destructor
    //@{
    explicit BridgingCapacity( xml::xistream& xis );
             BridgingCapacity();
    virtual ~BridgingCapacity();
	//@}

    //! @name CheckPoints
    //@{    
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( Object& /*object*/ );
    //@}

	//! @name virtual
    //@{
    virtual void Instanciate( Object& object ) const;
    virtual void Finalize( Object& object );	
	//@}

private:
    //! @name Copy
    //@{
    BridgingCapacity( const BridgingCapacity& from );
    //@}

    //! @name 
    //@{
    void CreateBridgeGeometry( const T_PointVector& points, T_PointVector& output ) const;
    //@}

private:
    //! @name 
    //@{
    // Path finder enhancement
    MIL_DynamicPathDataHandler handler_;
    //@}
};

#endif