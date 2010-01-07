// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __UrbanBlockDeserializer_h_
#define __UrbanBlockDeserializer_h_

#include <Urban/UrbanBlockDeserializer_ABC.h>
#include "game_asn/Simulation.h"

class ColorRGBA;
namespace urban
{
    class Block;
}

// =============================================================================
/** @class  UrbanBlockDeserializer
@brief  UrbanBlockDeserializer
*/
// Created: SLG 2009-12-04
// =============================================================================
class UrbanBlockDeserializer : public urban::UrbanBlockDeserializer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanBlockDeserializer( const ASN1T_MsgUrbanCreation& asn );
    virtual ~UrbanBlockDeserializer();
    //@}

    //! @name Operations
    //@{
    virtual float        GetArchitectureHeight              () const;
    virtual unsigned int GetArchitectureFloorNumber         () const;
    virtual unsigned int GetArchitectureBasementLevelNumber () const;
    virtual std::string  GetArchitectureRoofShape           () const;
    virtual std::string  GetArchitectureMaterial            () const;
    virtual float        GetArchitectureInnerCluttering     () const;
    virtual float        GetArchitectureFacadeOpacity       () const;

    virtual float        GetSoilOccupation                  () const;
    virtual float        GetSoilTrafficability              () const;
    virtual bool         GetSoilMultiplicity                () const;
    virtual std::string  GetSoilCompoundClearing            () const;

    virtual std::string  GetVegetationType                  () const;
    virtual unsigned int GetVegetationHeight                () const;
    virtual float        GetVegetationDensity               () const;

    std::auto_ptr< ColorRGBA >  GetColor                    () const;

    virtual bool HasColor()         const;
    virtual bool HasArchitecture()  const;
    virtual bool HasSoil()          const;
    virtual bool HasVegetation()    const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanBlockDeserializer( const UrbanBlockDeserializer& );            //!< Copy constructor
    UrbanBlockDeserializer& operator=( const UrbanBlockDeserializer& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const ASN1T_MsgUrbanCreation& message_;
    //@}
};

#endif // __UrbanBlockDeserializer_h_
