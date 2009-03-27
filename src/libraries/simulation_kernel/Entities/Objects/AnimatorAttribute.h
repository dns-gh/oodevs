// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __AnimatorAttribute_h_
#define __AnimatorAttribute_h_

#include "ObjectAttribute_ABC.h"

class MIL_Agent_ABC;

// =============================================================================
/** @class  AnimatorAttribute
    @brief  AnimatorAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class AnimatorAttribute : public ObjectAttribute_ABC
{ 

public:
    //! @name Constructors/Destructor
    //@{
             AnimatorAttribute();    
    explicit AnimatorAttribute( uint maxAnimator ); 
    explicit AnimatorAttribute( xml::xistream& xis );
    virtual ~AnimatorAttribute();
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    uint GetMaxAnimators() const;
    uint GetCurrent() const;
    bool AddAnimator( const MIL_Agent_ABC& agent );
    void ReleaseAnimator( const MIL_Agent_ABC& agent );
    //@}

    //! @name 
    //@{
    bool CanBeAnimatedBy( const MIL_Agent_ABC& agent ) const;
    //@}

    //! @name Operations
    //@{
    AnimatorAttribute& operator=( const AnimatorAttribute& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::set< const MIL_Agent_ABC* >  T_AgentSet;
    typedef T_AgentSet::const_iterator  CIT_AgentSet;
    //@}

    const T_AgentSet& GetAnimators() const;

private:
    //! @name Member data
    //@{
    T_AgentSet  animators_;
    uint        maxAnimators_;
    //@}
};

#endif // __AnimatorAttribute_h_
