// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ObstacleAttribute_h_
#define __ObstacleAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include <boost/serialization/export.hpp>

// =============================================================================
/** @class  ObstacleAttribute
    @brief  ObstacleAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class ObstacleAttribute : public ObjectAttribute_ABC
                        , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObstacleAttribute();
    explicit ObstacleAttribute( xml::xistream& xis );
    explicit ObstacleAttribute( const sword::MissionParameter_Value& attributes );
    virtual ~ObstacleAttribute();
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive& file, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    virtual void Register( MIL_Object_ABC& object ) const;
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual bool SendUpdate( sword::ObjectAttributes& asn ) const;
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Setters/Getters
    //@{
    void SetActivityTime( unsigned int activityTime );
    void SetActivationTime( unsigned int activationTime );
    int GetActivationTime() const;
    int GetActivityTime() const;
    int GetEndActivity() const;
    bool IsTimesUndefined() const;
    void SetActivatedAfterConstruction( bool activatedAfterConstruction );
    bool ActivatedAfterConstruction() const;
    bool IsActivated() const;
    void Activate( bool activate );
    //@}

    //! @name Operations
    //@{
    virtual void OnUpdate( const sword::MissionParameter_Value& attribute ); //<! Update from asn msg
    ObstacleAttribute& operator=( const ObstacleAttribute& ); //!< Assignment operator
    bool Update( const ObstacleAttribute& rhs );
    //@}

private:
    //! @name Member data
    //@{
    bool bActivated_;
    bool activatedAfterConstruction_;
    int activationTime_;
    int activityTime_;
    unsigned int creationTime_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ObstacleAttribute )

#endif // __ObstacleAttribute_h_
