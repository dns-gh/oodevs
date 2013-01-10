// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_HumanRepartition_h_
#define __MIL_HumanRepartition_h_

// =============================================================================
/** @class  MIL_HumanRepartition
    @brief  MIL_HumanRepartition
*/
// Created: MMC 2011-10-07
// =============================================================================
class MIL_HumanRepartition : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_HumanRepartition();
    explicit MIL_HumanRepartition( xml::xistream& xis );
    virtual ~MIL_HumanRepartition();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void SendFullState( T& msg ) const;
    bool IsCivilian() const;
    float GetMale() const;
    float GetFemale() const;
    float GetChildren() const;
    //@}

private:
    //! @name Member data
    //@{
    float male_;
    float female_;
    float children_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition::SendFullState
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
template< typename T >
void MIL_HumanRepartition::SendFullState( T& msg ) const
{
    sword::HumanRepartition& repartition = *msg().mutable_repartition();
    repartition.set_male( male_ );
    repartition.set_female( female_ );
    repartition.set_children( children_ );
}

#endif // __MIL_HumanRepartition_h_
