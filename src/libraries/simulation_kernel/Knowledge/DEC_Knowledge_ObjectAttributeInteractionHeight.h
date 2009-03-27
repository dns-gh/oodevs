// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectAttributeInteractionHeight_h_
#define __DEC_Knowledge_ObjectAttributeInteractionHeight_h_

#include "DEC_Knowledge_ObjectAttribute_ABC.h"

class InteractionHeightAttribute;

// =============================================================================
/** @class  DEC_Knowledge_ObjectAttributeInteractionHeight
    @brief  DEC_Knowledge_ObjectAttributeInteractionHeight
*/
// Created: LDC 2009-03-09
// =============================================================================
class DEC_Knowledge_ObjectAttributeInteractionHeight
    : public DEC_Knowledge_ObjectAttribute_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectAttributeInteractionHeight();
    explicit DEC_Knowledge_ObjectAttributeInteractionHeight( const InteractionHeightAttribute& attribute );
    virtual ~DEC_Knowledge_ObjectAttributeInteractionHeight();
    //@}

    //! @name Accessors
    //@{
    float Get() const;
    //@}
    
    //! @name CheckPoints
    //@{
    virtual void UpdateOnPerceptionLevel( const PHY_PerceptionLevel& currentPerceptionLevel );    
    virtual void UpdateOnPerception( const DEC_Knowledge_ObjectPerception& perception );
    virtual void UpdateOnCollision( const DEC_Knowledge_ObjectCollision& collision  );
    virtual void Send( ASN1T_ObjectAttributes& asn ) const;
    //@}

    //! @name CheckPoint / boost deserialize
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( DEC_Knowledge_Object& knObject );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DEC_Knowledge_ObjectAttributeInteractionHeight( const DEC_Knowledge_ObjectAttributeInteractionHeight& );            //!< Copy constructor
    DEC_Knowledge_ObjectAttributeInteractionHeight& operator=( const DEC_Knowledge_ObjectAttributeInteractionHeight& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    float height_;
    //@}
};

#endif // __DEC_Knowledge_ObjectAttributeInteractionHeight_h_
