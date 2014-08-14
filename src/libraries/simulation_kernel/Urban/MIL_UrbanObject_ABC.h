// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MIL_UrbanObject_ABC_h_
#define __MIL_UrbanObject_ABC_h_

#include "Entities/Objects/MIL_Object.h"
#include <tools/Extendable.h>
#include <tools/Resolver.h>

class MIL_LivingArea;

// =============================================================================
/** @class  MIL_UrbanObject_ABC
    @brief  MIL_UrbanObject_ABC
*/
// Created: JSR 2012-07-31
// =============================================================================
class MIL_UrbanObject_ABC : public MIL_Object
                          , public tools::Resolver< MIL_UrbanObject_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_UrbanObject_ABC( const MIL_ObjectType_ABC& type, unsigned long forcedId = 0 ) : MIL_Object( 0, type, forcedId ) {}
             MIL_UrbanObject_ABC() {}
    virtual ~MIL_UrbanObject_ABC() {}
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< MIL_Object >( *this );
    }
    virtual void WriteUrban( xml::xostream& xos ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetUrbanId() const = 0;
    virtual const std::string& GetName() const = 0;
    virtual MIL_UrbanObject_ABC* GetParent() const = 0;
    virtual bool IsBlock() const = 0;
    virtual void GetUrbanBlocks( std::vector< const MIL_UrbanObject_ABC* >& blocks ) const = 0;
    virtual float GetLivingSpace() const = 0;
    virtual unsigned int GetTotalInhabitants() const = 0;
    virtual TER_Polygon GetScaledLocation( double distance ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void ComputeConvexHull() = 0;
    virtual float ComputeComplexity() const = 0;
    virtual const std::vector< boost::shared_ptr< MT_Vector2D > >& ComputeLocalisationsInsideBlock() const = 0;
    virtual void AddLivingArea( MIL_LivingArea& livingArea ) = 0;
    virtual unsigned int GetTotalInhabitantsForMotivation( const std::string& motivation ) const = 0;
    virtual void UpdateInhabitants( MIL_LivingArea& livingArea, const std::string& motivation, unsigned int number ) = 0;
    //@}
};

#endif // __MIL_UrbanObject_ABC_h_
