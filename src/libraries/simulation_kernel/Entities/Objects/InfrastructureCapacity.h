// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfrastructureCapacity_h_
#define __InfrastructureCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace sword
{
    class MissionParameter_Value;
    class UrbanAttributes;
}

class PHY_InfrastructureType;

// =============================================================================
/** @class  InfrastructureCapacity
    @brief  InfrastructureCapacity
*/
// Created: SLG 2010-01-13
// =============================================================================
class InfrastructureCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             InfrastructureCapacity();
    explicit InfrastructureCapacity( const PHY_InfrastructureType& type );
    explicit InfrastructureCapacity( xml::xistream& xis );
    virtual ~InfrastructureCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    void WriteUrban( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void Update( MIL_Object_ABC& object, unsigned int time );
    void Update( xml::xistream& xis, const MIL_Object_ABC& /*object*/ );
    void SendState( sword::UrbanAttributes& message ) const;
    void SendFullState( sword::UrbanAttributes& message ) const;
    void OnUpdate( const sword::MissionParameter_Value& attribute );
    bool IsActive() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfrastructureCapacity( const InfrastructureCapacity& );            //!< Copy constructor
    InfrastructureCapacity& operator=( const InfrastructureCapacity& ); //!< Assignment operator

    //! @name Helpers
    //@{
    void InitializeData( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    float functionalState_;
    std::string role_;
    float threshold_;
    bool enabled_;
    mutable bool needUpdate_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( InfrastructureCapacity )

#endif // __InfrastructureCapacity_h_
