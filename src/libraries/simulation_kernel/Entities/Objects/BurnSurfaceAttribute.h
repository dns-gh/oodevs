// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __BurnSurfaceAttribute_h_
#define __BurnSurfaceAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include <boost/serialization/export.hpp>

class MIL_BurningCells;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

namespace sword
{
	enum EnumBurningCellPhase;
}

// =============================================================================
/** @class  BurnSurfaceAttribute
    @see "Feu sauvage" in 4311340-Mod�les de feu.doc
*/
// Created: BCI 2010-12-09
// =============================================================================
class BurnSurfaceAttribute : public ObjectAttribute_ABC
                    , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BurnSurfaceAttribute( MIL_Object_ABC* pObject = 0 );
    virtual ~BurnSurfaceAttribute();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    virtual void Register( MIL_Object_ABC& object ) const;
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual void SendUpdate( sword::ObjectAttributes& asn ) const;
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Notifications
    //@{
	void NotifyCellsUpdated();
	//@}

    //! @name Operations
    //@{
    BurnSurfaceAttribute( const BurnSurfaceAttribute& ); //!< Copy operator
    BurnSurfaceAttribute& operator=( const BurnSurfaceAttribute& ); //!< Assignment operator
    //@}

private:

	//! @name Member data
    //@{
	MIL_BurningCells& burningCells_;
	MIL_Object_ABC* pObject_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( BurnSurfaceAttribute )

#endif // __BurnSurfaceAttribute_h_
