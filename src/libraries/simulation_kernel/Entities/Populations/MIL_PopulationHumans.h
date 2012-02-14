// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_PopulationHumans_h_
#define __MIL_PopulationHumans_h_

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  MIL_PopulationHumans
    @brief  MIL_PopulationHumans
*/
// Created: JSR 2011-03-10
// =============================================================================
class MIL_PopulationHumans
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_PopulationHumans();
             MIL_PopulationHumans( const MIL_PopulationHumans& );
    explicit MIL_PopulationHumans( xml::xistream& xis );
    explicit MIL_PopulationHumans( unsigned int healthy, unsigned int contaminated = 0, unsigned int wounded = 0, unsigned int dead = 0 );
    virtual ~MIL_PopulationHumans();
    //@}

    //! @name Accessors
    //@{
    unsigned int GetHealthyHumans() const;
    unsigned int GetWoundedHumans() const;
    unsigned int GetContaminatedHumans() const;
    unsigned int GetDeadHumans() const;
    //@}

    //! @name Operations
    //@{
    MIL_PopulationHumans& operator +=( const MIL_PopulationHumans& humans );
    MIL_PopulationHumans& operator -=( const MIL_PopulationHumans& humans );
    MIL_PopulationHumans& operator *=( double ratio );
    void Empty();
    bool IsEmpty() const;
    unsigned int GetAllHumans() const;
    unsigned int GetTotalLivingHumans() const;
    double GetLivingRatio() const;
    void KillAll();
    void KillHumans( unsigned int human );
    void WoundHumans( unsigned int human );
    unsigned int ApplyNumberOfDead( unsigned int dead );
    unsigned int ApplyWounds( unsigned int wounds );
    void ApplyContamination();
    bool ApplyDecontamination();
    void ApplyIntoxication( double woundedPercentage, double deadPercentage );
    void PullFrom( unsigned int number, MIL_PopulationHumans& humans );
    void ReintegrateUrbanBlock();
    //@}

    //! @name Serialization
    //@{
    template< typename Archive >
    void serialize( Archive& ar, const unsigned int );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_PopulationHumans& operator=( const MIL_PopulationHumans& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int healthy_;
    unsigned int wounded_;
    unsigned int contaminated_;
    unsigned int dead_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: MIL_PopulationHumans::serialize
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_PopulationHumans::serialize( Archive& ar, const unsigned int )
{
    ar & healthy_
       & wounded_
       & contaminated_
       & dead_;
}

#endif // __MIL_PopulationHumans_h_
