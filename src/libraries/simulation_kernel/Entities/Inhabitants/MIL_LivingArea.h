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

namespace client
{
    class PopulationCreation;
    class PopulationUpdate;
}

class MIL_StructuralStateNotifier_ABC;
class PHY_ResourceNetworkType;
class UrbanObjectWrapper;
class TER_Localisation;

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
    virtual void MovePeople( int occurence );
    virtual void FinishMoving();
    virtual geometry::Polygon2f ComputeMovingArea() const;
    virtual geometry::Polygon2f ComputeLivingArea() const;

    void DistributeHumans( unsigned long population );
    void Register( MIL_StructuralStateNotifier_ABC& structural );
    void WriteODB( xml::xostream& xos ) const;
    float HealthCount() const;
    void SendCreation( client::PopulationCreation& msg ) const;
    void SendFullState( client::PopulationUpdate& msg ) const;
    void UpdateNetwork( client::PopulationUpdate& msg ) const;
    float ComputeOccupationFactor() const;
    void GetUsagesOccupation( std::map< std::string, unsigned int >& occupations ) const;
    void Alert( const TER_Localisation& localisation );
    void SetAlerted( bool );
    float Consume( const PHY_ResourceNetworkType& resource, unsigned int consumption );
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
    struct T_Block
    {
        T_Block( UrbanObjectWrapper* pUrbanObject, unsigned int person = 0, bool alerted = false )
            : pUrbanObject_( pUrbanObject )
            , person_      ( person )
            , alerted_     ( alerted )
        {}

        UrbanObjectWrapper* pUrbanObject_;
        unsigned int person_;
        bool alerted_;
    };
    typedef std::vector< T_Block >                         T_Blocks;
    typedef T_Blocks::iterator                            IT_Blocks;
    typedef T_Blocks::const_iterator                     CIT_Blocks;

    typedef std::map< unsigned long, unsigned int > T_Identifiers;
    typedef T_Identifiers::const_iterator         CIT_Identifiers;

    typedef std::map< std::string, float >     T_Accommodations;
    typedef T_Accommodations::const_iterator CIT_Accommodations;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadUrbanBlock( xml::xistream& xis );
    void LoadAccommodations();
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
    T_Identifiers peopleMovingBlock_;
    T_Identifiers identifiers_;
    mutable bool hasChanged_;
    float area_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_LivingArea )

#endif // __MIL_LivingArea_h
