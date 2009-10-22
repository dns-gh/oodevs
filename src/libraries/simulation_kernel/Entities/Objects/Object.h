// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Object_h_
#define __Object_h_

#include <vector>
#include "MIL_Object_ABC.h"
#include "CapacityContainer_ABC.h"
#include "tools/Extendable.h"

class ObjectAttribute_ABC;
class MIL_ObjectType_ABC;
class MIL_Army_ABC;
class MIL_ObjectBuilder_ABC;

// =============================================================================
/** @class  Object
    @brief  Object
*/
// Created: JCR 2008-04-18
// =============================================================================
class Object : public MIL_Object_ABC
 {
public:
	//! @name Constructors/Destructor
    //@{
            Object( uint id, const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC& army, const TER_Localisation* pLocation, const std::string& name = std::string(), bool reserved = true );
            Object();
	virtual ~Object();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name 
    //@{    
    template< typename T > void AddCapacity( T* capacity );
                           void Register( ObjectCapacity_ABC* capacity );
                           void Register( MIL_InteractiveContainer_ABC* capacity );
    template< typename T >             T&   GetAttribute();
    template< typename T >       const T&   GetAttribute() const;
    template< typename T, typename I > T&   GetAttribute();
    template< typename T, typename I > void SetAttribute( const T& attribute );
    template< typename T >             T*   RetrieveAttribute();
    template< typename T >       const T*   RetrieveAttribute() const;
    //@}

	//! @name Construction
    //@{
	void Update( uint time );
    void UpdateLocalisation( const TER_Localisation& location );
    //@}

    //! @name Instanciate / Build / Copy object
    //@{
    void Instanciate( Object& object ) const; //<! create and register every prototyped capacity
    void Finalize(); //<! finalize capacity instanciation : for instance once the object location has been defined
    //@}

    //! @name Knowledge
    //@{
    boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledge( const MIL_Army_ABC& team );
    //@}

    //! @name Manipulators
    //@{
    const MIL_ObjectManipulator_ABC& operator()() const;
          MIL_ObjectManipulator_ABC& operator()();
    //@}

    //! @name Interaction
    //@{
    virtual bool CanInteractWith( const MIL_Agent_ABC& agent ) const;

    void ProcessAgentEntering    ( MIL_Agent_ABC& agent );
    void ProcessAgentExiting     ( MIL_Agent_ABC& agent );
    void ProcessAgentMovingInside( MIL_Agent_ABC& agent );
    void ProcessAgentInside      ( MIL_Agent_ABC& agent );
    void ProcessPopulationInside ( MIL_PopulationElement_ABC& population );
    //@}

    //! @name Network
    //@{
    virtual   void    UpdateState();
    ASN1T_EnumObjectErrorCode OnUpdate( const ASN1T_ObjectAttributes& asn );
    void    SendCreation() const;
    void    SendDestruction() const;
    void    SendFullState() const;
    //@}

    //! @name HLA
    //@{
    HLA_Object_ABC* GetHLAView() const;
    void            SetHLAView( HLA_Object_ABC& view );
    void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    void Serialize( HLA_UpdateFunctor& functor ) const;
    //@}

    //! @name 
    //@{
    uint  GetID() const;    
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Object( const Object& );            //!< Copy constructor
    Object& operator=( const Object& ); //!< Assignment operator
    //@}

    //! @name 
    //@{
    void SendMsgUpdate() const;
    //@}

    //! @name 
    //@{
    void Register( ObjectAttribute_ABC* attribute );    
    //@}

private:
    //! @name Types containers
    //@{
    typedef std::vector< ObjectAttribute_ABC* >     T_Attributes;
    typedef T_Attributes::const_iterator            CIT_Attributes;
    
    typedef std::vector< ObjectCapacity_ABC* >              T_Capacities;
    typedef std::vector< MIL_InteractiveContainer_ABC* >    T_InteractiveCapacities;
    //@}

    //! @name Types
    //@{
    enum E_AttributeUpdate
    {
        eAttrUpdate_ConstructionPercentage    = 0x01,
        eAttrUpdate_MiningPercentage          = 0x02,
        eAttrUpdate_BypassPercentage          = 0x04,
        eAttrUpdate_ReservedObstacleActivated = 0x08,
        eAttrUpdate_Localisation              = 0x10,
        eAttrUpdate_SpecificAttributes        = 0x20,
        eAttrUpdate_All                       = 0xFF
    };
    //@}

private:
	//! @name Member data
    //@{
    std::string             name_;
    uint                    id_;
    T_Capacities            capacities_;
    T_InteractiveCapacities interactives_;
    T_Attributes            attributes_;    
    //@}

    //! @name 
    //@{
    std::auto_ptr< MIL_ObjectManipulator_ABC > manipulator_;
    //@}

    //! @name HLA
    //@{
    HLA_Object_ABC* pView_;
    //@}

    //! @name Network
    //@{ 
    mutable uint8 xAttrToUpdate_;
    mutable uint8 xAttrToUpdateForHLA_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: template< typename T >             T& Object::GetAttribute
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
template< typename T > T& Object::GetAttribute()
{
    return GetAttribute< T, T >();
}

// -----------------------------------------------------------------------------
// Name: template< typename T >             T& Object::GetAttribute
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
template< typename T > const T& Object::GetAttribute() const
{
    return tools::Extendable< ObjectAttribute_ABC >::Get< T >();
}

// -----------------------------------------------------------------------------
// Name: template< typename T > T& Object::GetAttribute
// Created: JCR 2008-05-26
// -----------------------------------------------------------------------------
template< typename T, typename I > T& Object::GetAttribute()
{
    I* attribute = tools::Extendable< ObjectAttribute_ABC >::Retrieve< I >();
    if( !attribute )
    {
        attribute = new T();
        tools::Extendable< ObjectAttribute_ABC >::Attach< I >( *attribute );
        Register( attribute );
    }
    return *static_cast< T* >( attribute );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > void Object::SetAttribute
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
template< typename T, typename I >
void Object::SetAttribute( const T& attribute )
{
    GetAttribute< T, I >() = attribute;
}

// -----------------------------------------------------------------------------
// Name: template< typename T > T* Object::RetrieveAttribute
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
template< typename T > 
T* Object::RetrieveAttribute()
{
    return tools::Extendable< ObjectAttribute_ABC >::Retrieve< T >();
}

// -----------------------------------------------------------------------------
// Name: template< typename T > T* Object::RetrieveAttribute
// Created: LDC 2009-03-25
// -----------------------------------------------------------------------------
template< typename T > 
const T* Object::RetrieveAttribute() const
{
    return tools::Extendable< ObjectAttribute_ABC >::Retrieve< T >();
}

// -----------------------------------------------------------------------------
// Name: Object::AddCapacity
// Created: JCR 2008-05-26
// -----------------------------------------------------------------------------
template< typename T > 
void Object::AddCapacity( T* capacity )
{
    tools::Extendable< ObjectCapacity_ABC >::Attach( *capacity );
    Register( static_cast< ObjectCapacity_ABC *>( capacity ) );
}

#endif
