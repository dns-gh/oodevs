// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ObjectManipulator_ABC_h_
#define __MIL_ObjectManipulator_ABC_h_

class MIL_Agent_ABC;
class MIL_Entity_ABC;

// =============================================================================
/** @class  MIL_ObjectManipulator_ABC
    @brief  MIL Object manipulator definition
*/
// Created: JCR 2008-06-02
// =============================================================================
class MIL_ObjectManipulator_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ObjectManipulator_ABC() {}
    virtual ~MIL_ObjectManipulator_ABC() {}
    //@}

    //! @name Manipulations
    //@{
    virtual void Construct( double rDeltaPercentage ) = 0;
    virtual void Destroy  ( double rDeltaPercentage ) = 0;
    virtual void Mine     ( double rDeltaPercentage ) = 0;
    virtual void Demine   ( double rDeltaPercentage ) = 0;
    virtual void Bypass   ( double rDeltaPercentage ) = 0;

    virtual void Activate () = 0;
    virtual void Construct() = 0;
    virtual void Destroy  () = 0;
    virtual void Mine     () = 0;

    virtual void Extinguish( int agent, int nHose ) = 0;

    virtual bool CanBeAnimatedBy( const MIL_Agent_ABC& agent ) const = 0;
    virtual void AddAnimator( const MIL_Agent_ABC& agent ) = 0;
    virtual void AddDetector( const MIL_Agent_ABC& agent ) = 0;
    virtual void AddCreator( const MIL_Agent_ABC& agent ) = 0;
    virtual void ReleaseAnimator( const MIL_Agent_ABC& agent ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual bool CanBeConstructed() const = 0;
    virtual bool CanBeMined      () const = 0;
    virtual bool CanBeDemined    () const = 0;
    virtual bool CanBeDestroyed  () const = 0;
    virtual bool CanBeBypassed   () const = 0;
    virtual bool CanBePerceived  () const = 0;
    virtual bool CanBeActivated  () const = 0;
    virtual bool CanBeOccupiedBy ( const MIL_Agent_ABC& agent ) const = 0;
    //@}

    //! @name Construction interface
    //@{
    virtual double GetSizeCoef() const = 0;
    //@}

    //! @name Speed
    //@{
    virtual bool HasMobilityInfluence() const = 0;
    virtual double ApplySpeedPolicy( double rAgentSpeedWithinObject, double rAgentSpeedWithinEnvironment, double rAgentMaxSpeed, const MIL_Entity_ABC& agent ) const = 0;
    virtual double GetMaxSpeed() const = 0;
    //@}

    //! @name
    //@{
    virtual bool IsBypassed() const = 0;
    virtual bool IsMined() const = 0;
    virtual bool IsFullyMined() const = 0;
    virtual bool IsBuilt() const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_ObjectManipulator_ABC( const MIL_ObjectManipulator_ABC& );            //!< Copy constructor
    MIL_ObjectManipulator_ABC& operator=( const MIL_ObjectManipulator_ABC& ); //!< Assignment operator
    //@}
};

#endif // __MIL_ObjectManipulator_ABC_h_
