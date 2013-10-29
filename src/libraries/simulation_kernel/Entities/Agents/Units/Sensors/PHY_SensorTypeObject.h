// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/agents/units/sensors/PHY_SensorTypeObject.h $
// $Author: Jvt $
// $Modtime: 12/04/05 10:11 $
// $Revision: 3 $
// $Workfile: PHY_SensorTypeObject.h $
//
// *****************************************************************************

#ifndef __PHY_SensorTypeObject_h_
#define __PHY_SensorTypeObject_h_

namespace xml
{
    class xistream;
}

class MIL_Object_ABC;
class DEC_Knowledge_Object;
class MIL_Agent_ABC;
class PHY_PerceptionLevel;
class PHY_SensorTypeObjectData;
class PHY_SensorType;
class MIL_ObjectType_ABC;
class ObjectTypeResolver_ABC;

// =============================================================================
// @class  PHY_SensorTypeObject
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SensorTypeObject : private boost::noncopyable
{
public:
             PHY_SensorTypeObject( const PHY_SensorType& type, xml::xistream& xis, const ObjectTypeResolver_ABC& resolver );
    virtual ~PHY_SensorTypeObject();

    //! @name Accessors
    //@{
    double GetMaxDistance() const;
    const PHY_SensorType& GetType() const;
    //@}

    //! @name Operations
    //@{
    const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& perceiver, const MIL_Object_ABC&   target, double rSensorHeight ) const;
    const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& perceiver, const DEC_Knowledge_Object& target, double rSensorHeight ) const;
    //@}

    //! @name Helpers
    //@{
    void ReadObject( xml::xistream& xis, const ObjectTypeResolver_ABC& resolver );
    bool CanPerceive( const MIL_ObjectType_ABC& objectType ) const;
    //@}

private:
    //! @name Helpers
    //@{
    const PHY_SensorTypeObjectData* GetSensorTypeObjectData( const MIL_ObjectType_ABC& objectType ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const PHY_SensorTypeObjectData* > T_ObjectDataVector;
    //@}

private:
    const PHY_SensorType& type_;
    T_ObjectDataVector objectData_;
    double             rMaxDistance_;
};

namespace boost
{
namespace archive
{
    template< class Archive >
    void save( Archive& ar, const PHY_SensorTypeObject* t )
    {
        unsigned int id = t ? t->GetType().GetID() : std::numeric_limits< unsigned int >::max();
        ar << id;
    }
    template< class Archive >
    void load( Archive& ar, const PHY_SensorTypeObject*& t )
    {
        unsigned int id;
        ar >> id;
        auto type = PHY_SensorType::FindSensorType( id );
        if( ! type )
        {
            t = 0;
            return;
        }
        t = type->GetTypeObject();
    }
}
}

#endif // __PHY_SensorTypeObject_h_
