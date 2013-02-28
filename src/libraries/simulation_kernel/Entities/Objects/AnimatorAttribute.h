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
#include <tools/Set.h>
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

class MIL_Agent_ABC;

// =============================================================================
/** @class  AnimatorAttribute
    @brief  AnimatorAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class AnimatorAttribute : public ObjectAttribute_ABC
{
private:
    //! @name Types
    //@{
    typedef tools::Set< const MIL_Agent_ABC* > T_AgentSet;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             AnimatorAttribute();
    explicit AnimatorAttribute( unsigned int maxAnimator );
    explicit AnimatorAttribute( xml::xistream& xis );
    virtual ~AnimatorAttribute();
    //@}

    //! @name CheckPoint
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template< typename Archive > void load( Archive&, const unsigned int );
    template< typename Archive > void save( Archive&, const unsigned int ) const;
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name Operations
    //@{
    double GetAnimatorsRatio() const;
    int GetMaxAnimators() const;
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

    const T_AgentSet& GetAnimators() const;

private:
    //! @name Member data
    //@{
    T_AgentSet animators_;
    unsigned int maxAnimators_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( AnimatorAttribute )

#endif // __AnimatorAttribute_h_
