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

#include "MIL.h"
#include "MIL_LivingArea_ABC.h"
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

class MIL_Inhabitant;
class MIL_LivingAreaBlock;
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
    // $$$$ _RC_ JSR 2011-03-24: à passer en private
    //! @name Types
    //@{
    // $$$$ _RC_ JSR 2011-03-24: mettre un std::set avec la fonction de comparaison?
    typedef std::vector< MIL_LivingAreaBlock* > T_Blocks;
    typedef T_Blocks::iterator                 IT_Blocks;
    typedef T_Blocks::const_iterator          CIT_Blocks;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MIL_LivingArea();
             MIL_LivingArea( xml::xistream& xis, unsigned long population, MIL_Inhabitant& inhabitant );
    virtual ~MIL_LivingArea();
    //@}

    //! @name Initialization
    //@{
    virtual void Finalize();
    //@}

    //! @name Operations
    //@{
    virtual void StartMotivation( const std::string& motivation );
    virtual void MovePeople( const std::string& motivation, int occurence );
    virtual void FinishMoving( const std::string& motivation );
    virtual T_PointVector ComputeMovingArea() const;

    void DistributeHumans( unsigned long population );
    void WriteODB( xml::xostream& xos ) const;
    float HealthCount() const;
    void SendCreation( client::PopulationCreation& msg ) const;
    void SendFullState( client::PopulationUpdate& msg ) const;
    void UpdateNetwork( client::PopulationUpdate& msg ) const;
    float ComputeOccupationFactor() const;
    void GetUsagesOccupation( std::map< std::string, unsigned int >& occupations ) const;
    void Alert( const TER_Localisation& localisation );
    bool IsAlerted( const TER_Localisation& localisation ) const;
    void SetAlerted( bool, UrbanObjectWrapper* pUrbanObject = 0 );
    void Confine( const TER_Localisation& localisation );
    void SetConfined( bool, UrbanObjectWrapper* pUrbanObject = 0 );
    void SetEvacuated( bool, UrbanObjectWrapper* pUrbanObject = 0 );
    float Consume( const PHY_ResourceNetworkType& resource, unsigned int consumption, T_Blocks& angryBlocks );
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive& file, const unsigned int );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, unsigned int >      T_PersonsPerAccomodation;
    typedef T_PersonsPerAccomodation::iterator        IT_PersonsPerAccomodation;
    typedef T_PersonsPerAccomodation::const_iterator CIT_PersonsPerAccomodation;

    typedef std::map< MIL_LivingAreaBlock*, T_PersonsPerAccomodation > T_BlockCompositions;
    typedef T_BlockCompositions::iterator                             IT_BlockCompositions;
    typedef T_BlockCompositions::const_iterator                      CIT_BlockCompositions;

    typedef std::map< MIL_LivingAreaBlock*, float > T_BlockRatio;
    typedef T_BlockRatio::iterator                 IT_BlockRatio;
    typedef T_BlockRatio::const_iterator          CIT_BlockRatio;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadUrbanBlock( xml::xistream& xis );
    T_Blocks GetBlockUsage( const std::string& motivation ) const;
    const T_Blocks GetNonConfinedBlocks() const;
    void Clean();
    //@}

private:
    //! @name Member data
    //@{
    MIL_Inhabitant* pInhabitant_;
    unsigned long population_;
    std::set< unsigned long > urbanIds_;
    T_Blocks blocks_;
    T_BlockCompositions startingBlocks_;
    T_BlockCompositions currentStartingState_;
    T_BlockRatio finalBlocks_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_LivingArea )

#endif // __MIL_LivingArea_h
