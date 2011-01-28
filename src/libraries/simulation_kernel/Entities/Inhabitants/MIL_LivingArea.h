// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_LivingArea_h
#define __MIL_LivingArea_h

#include "MIL_LivingArea_ABC.h"
#include "MIL.h"
#include <vector>
#include <map>

namespace xml
{
    class xistream;
    class xostream;
}

namespace urban
{
    class TerrainObject_ABC;
}

namespace client
{
    class PopulationCreation;
    class PopulationUpdate;
}

class MIL_StructuralStateNotifier_ABC;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_LivingArea
    @brief  MIL living area
*/
// Created: LGY 2011-01-20
// =============================================================================
class MIL_LivingArea : public MIL_LivingArea_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_LivingArea();
             MIL_LivingArea( xml::xistream& xis, unsigned long population );
    virtual ~MIL_LivingArea();
    //@}

    //! @name Operations
    //@{
    virtual void StartMotivation( const std::string& motivation );

    void Register( MIL_StructuralStateNotifier_ABC& structural );
    void WriteODB( xml::xostream& xos ) const;
    float HealthCount() const;
    void SendCreation( client::PopulationCreation& msg ) const;
    void SendFullState( client::PopulationUpdate& msg ) const;
    void UpdateNetwork( client::PopulationUpdate& msg ) const;
    unsigned int GetTotalOccupation() const;
    void GetUsagesOccupation( std::map< std::string, unsigned int >& occupations ) const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< const urban::TerrainObject_ABC*, unsigned int > T_Block;
    typedef std::vector< T_Block >                                     T_Blocks;
    typedef T_Blocks::iterator                                        IT_Blocks;
    typedef T_Blocks::const_iterator                                 CIT_Blocks;

    typedef std::map< unsigned long, unsigned int > T_Identifiers;
    typedef T_Identifiers::const_iterator         CIT_Identifiers;

    typedef std::map< std::string, float >     T_Accommodations;
    typedef T_Accommodations::const_iterator CIT_Accommodations;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadUrbanBlock( xml::xistream& xis, float& area );
    void LoadAccommodations();
    void DistributeHumans( float area );
    float GetProportion( const T_Block& block, const std::string& motivation ) const;
    T_Blocks GetBlockUsage( const std::string& motivation ) const;
    unsigned int GetOccupation( const T_Block& block, const std::string& motivation ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned long population_;
    T_Accommodations accommodations_;
    T_Blocks blocks_;
    mutable bool hasChanged_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_LivingArea )

#endif // __MIL_LivingArea_h
