// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ObjectManipulator_h_
#define __MIL_ObjectManipulator_h_

#include "MIL_ObjectManipulator_ABC.h"

class MIL_Object_ABC;

// =============================================================================
/** @class  MIL_ObjectManipulator
    @brief  MIL Object manipulator
*/
// Created: JCR 2008-06-02
// =============================================================================
class MIL_ObjectManipulator : public MIL_ObjectManipulator_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_ObjectManipulator( MIL_Object_ABC& object );
    explicit MIL_ObjectManipulator( const MIL_Object_ABC& object );
    virtual ~MIL_ObjectManipulator();
    //@}

    //! @name
    //@{
    virtual void Construct( double rDeltaPercentage );
    virtual void Destroy  ( double rDeltaPercentage );
    virtual void Mine     ( double rDeltaPercentage );
    virtual void Demine   ( double rDeltaPercentage );
    virtual void Bypass   ( double rDeltaPercentage );
    virtual void Activate ();

    virtual void Construct();
    virtual void Destroy  ();
    virtual void Mine     ();

    virtual void Extinguish( int agent, int nHose );

    virtual bool CanBeAnimatedBy( const MIL_Agent_ABC& agent ) const;
    virtual void AddAnimator( const MIL_Agent_ABC& agent );
    virtual void AddDetector( const MIL_Agent_ABC& agent );
    virtual void AddCreator( const MIL_Agent_ABC& agent );
    virtual void ReleaseAnimator( const MIL_Agent_ABC& agent );
    //@}

    //! @name Accessors
    //@{
    virtual bool CanBeConstructed          () const;
    virtual bool CanBeMined                () const;
    virtual bool CanBeDemined              () const;
    virtual bool CanBeDestroyed            () const;
    virtual bool CanBeBypassed             () const;
    virtual bool CanBePerceived            () const;
    virtual bool CanBeActivated            () const;
    virtual bool CanBeOccupiedBy           ( const MIL_Agent_ABC& agent ) const;
    //@}

    //! @name
    //@{
    virtual bool IsBuilt() const;
    virtual bool IsBypassed() const;
    virtual bool IsMined() const;
    virtual bool IsFullyMined() const;
    //@}

    //! @name
    //@{
    virtual double GetSizeCoef() const;
    //@}

    //! @name
    //@{
    virtual bool HasMobilityInfluence      () const;
    virtual double ApplySpeedPolicy( double rAgentSpeedWithinObject, double rAgentSpeedWithinEnvironment, double rAgentMaxSpeed, const MIL_Entity_ABC& agent ) const;
    virtual double GetMaxSpeed() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_ObjectManipulator( const MIL_ObjectManipulator& );            //!< Copy constructor
    MIL_ObjectManipulator& operator=( const MIL_ObjectManipulator& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    MIL_Object_ABC& object_;
    //@}
};


#endif // __MIL_ObjectManipulator_h_
