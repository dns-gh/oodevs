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

class Object;

// =============================================================================
/** @class  MIL_ObjectManipulator
    @brief  MIL_ObjectManipulator
*/
// Created: JCR 2008-06-02
// =============================================================================
class MIL_ObjectManipulator : public MIL_ObjectManipulator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_ObjectManipulator( Object& object );
    explicit MIL_ObjectManipulator( const Object& object );    
    virtual ~MIL_ObjectManipulator();
    //@}

    //! @name 
    //@{
    virtual void Construct ( MT_Float rDeltaPercentage );
    virtual void Destroy   ( MT_Float rDeltaPercentage );
    virtual void Mine      ( MT_Float rDeltaPercentage );
    virtual void Demine    ( MT_Float rDeltaPercentage );
    virtual void Bypass    ( MT_Float rDeltaPercentage );
    virtual void Activate  ();

    virtual void Construct();
    virtual void Destroy  ();
    virtual void Mine     ();

	virtual void Extinguish( int agent, int nHose );

    virtual bool CanBeAnimatedBy( const MIL_Agent_ABC& agent ) const;
    virtual void AddAnimator( const MIL_Agent_ABC& agent );
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
    virtual MT_Float GetSizeCoef() const;
    //@}

    //! @name 
    //@{
    virtual bool HasMobilityInfluence      () const;
    virtual MT_Float ApplySpeedPolicy( MT_Float rAgentSpeedWithinObject, MT_Float rAgentSpeedWithinEnvironment, MT_Float rAgentMaxSpeed ) const;
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
    Object& object_;
    //@}
};


#endif // __MIL_ObjectManipulator_h_
