// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_LivingAreaBlock_h_
#define __MIL_LivingAreaBlock_h_

#include "MIL.h"

namespace client
{
    class PopulationUpdate;
}

class MIL_Army_ABC;
class MIL_InhabitantType;
class MIL_LivingArea;
class PHY_ResourceNetworkType;
class TER_Localisation;
class UrbanObjectWrapper;

// =============================================================================
/** @class  MIL_LivingAreaBlock
    @brief  MIL_LivingAreaBlock
*/
// Created: JSR 2011-03-23
// =============================================================================
class MIL_LivingAreaBlock : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_LivingAreaBlock( UrbanObjectWrapper& urbanObject );
    virtual ~MIL_LivingAreaBlock();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive& ar, const unsigned int );
    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_LivingAreaBlock* block, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_LivingAreaBlock* block, const unsigned int /*version*/ );
    //@}

    //! @name Network
    //@{
    void SendFullState( client::PopulationUpdate& msg ) const;
    //@}

    //! @name Operations
    //@{
    const UrbanObjectWrapper& GetObject() const;
    unsigned int GetPersonsForAccomodation( const std::string& accomodation ) const;
    unsigned int GetNominalOccupation( const std::string& motivation ) const;
    unsigned int GetMaxOccupation( const std::string& motivation ) const;
    bool CanMove() const;
    bool IsUsableForMotivation( const std::string& motivation ) const;
    bool IsAlerted( const TER_Localisation& localisation ) const;
    void SetAlerted( bool alerted );
    void SetEvacuated( bool evacuated );
    void SetConfined( bool confined );

    void DistributeHumans( unsigned int persons, MIL_LivingArea& livingArea );
    float ComputeOccupationFactor() const;
    void DecreasePeopleWhenMoving( const std::string& motivation, unsigned int number, MIL_LivingArea& livingArea );
    unsigned int IncreasePeopleWhenMoving( const std::string& motivation, unsigned int number, MIL_LivingArea& livingArea );
    float Consume( const PHY_ResourceNetworkType& resource, unsigned int consumption, bool& angry );
    void ManageAngryCrowd( const MIL_InhabitantType& type, MIL_Army_ABC& army );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, unsigned int > T_Persons;
    typedef T_Persons::iterator                  IT_Persons;
    typedef T_Persons::const_iterator           CIT_Persons;
    //@}

private:
    //! @name Helpers
    //@{
    float GetProportion( const std::string& motivation ) const;
    unsigned int GetTotalNumberOfPersons() const;
    //@}

private:
    //! @name Member data
    //@{
    static const std::string defaultAccomodation_;
    UrbanObjectWrapper& urbanObject_;
    T_Persons persons_;
    float angriness_;
    bool alerted_;
    bool confined_;
    bool evacuated_;
    bool outsideAngry_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_LivingAreaBlock )

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::serialize
// Created: JSR 2011-03-29
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_LivingAreaBlock::serialize( Archive& file, const unsigned int )
{
    file & persons_
         & angriness_
         & alerted_
         & confined_
         & evacuated_
         & outsideAngry_;
}

#endif // __MIL_LivingAreaBlock_h_
