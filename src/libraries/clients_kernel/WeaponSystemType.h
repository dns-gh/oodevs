// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __WeaponSystemType_h_
#define __WeaponSystemType_h_

#include "MT_Tools/MT_Tools_Types.h"
#include "MT_Tools/MT_InterpolatedFunction.h"
#include "tools/Resolver.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
class VolumeType;

// =============================================================================
/** @class  WeaponSystemType
    @brief  WeaponSystemType
*/
// Created: SBO 2008-08-06
// =============================================================================
class WeaponSystemType
{

public:
    //! @name Constructors/Destructor
    //@{
             WeaponSystemType( xml::xistream& xis, tools::Resolver< VolumeType >& volumes );
    virtual ~WeaponSystemType();
    //@}

    //! @name Operations
    //@{
    std::string GetId() const;
    unsigned int GetMaxRange() const;
    unsigned int GetMinRange() const;
    unsigned int GetEfficientRange( unsigned int volumeId, MT_Float ph ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MT_InterpolatedFunction< MT_Float > >  T_PhVector;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    WeaponSystemType( const WeaponSystemType& );            //!< Copy constructor
    WeaponSystemType& operator=( const WeaponSystemType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadDirectFire( xml::xistream& xis );
    void ReadDirectFireHitProbabilities( xml::xistream& xis );
    void ReadDirectFireHitProbability( xml::xistream& xis, MT_InterpolatedFunction< MT_Float >* phFunction );
    void ReadIndirectFire( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::string launcher_;
    std::string ammunition_;
    unsigned int maxIndirectRange_;
    unsigned int minIndirectRange_;
    unsigned int maxDirectRange_;
    unsigned int minDirectRange_;
    tools::Resolver< VolumeType >& volumes_;
    T_PhVector phs_;
    //@}
};

}

#endif // __WeaponSystemType_h_
