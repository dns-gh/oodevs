// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InitialState_h_
#define __InitialState_h_

#include <boost/noncopyable.hpp>
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class DotationCapacityType;
}

namespace tools
{
    template< typename Container > class Iterator;
}

namespace xml
{
    class xistream;
    class xostream;
}

class StaticModel;
class InitialStateCrew;
class InitialStateEquipment;
class InitialStateResource;

// =============================================================================
/** @class  InitialState
    @brief  InitialState
*/
// Created: ABR 2011-03-01
// =============================================================================
class InitialState : public kernel::Extension_ABC
                   , public kernel::Serializable_ABC
                   , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< InitialStateEquipment >    T_Equipments;
    typedef T_Equipments::iterator                 IT_Equipments;
    typedef T_Equipments::const_iterator          CIT_Equipments;

    typedef std::vector< InitialStateCrew >         T_Crews;
    typedef T_Crews::const_iterator               CIT_Crews;

    typedef std::vector< InitialStateResource >     T_Resources;
    typedef T_Resources::const_iterator           CIT_Resources;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             InitialState( const StaticModel& staticModel, unsigned int typeId );
             InitialState( xml::xistream& xis, const StaticModel& staticModel, unsigned int typeId );
    virtual ~InitialState();
    //@}

    //! @name Operations
    //@{
    void Reset();
    bool CleanUnsupportedState();
    const std::vector< std::string >& GetUnknownResources() const;
    bool IsOriginalResource( const QString& resourceName ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void Initialize();
    void UpdateResourceMaximum();
    void FillResources( tools::Iterator< const kernel::DotationCapacityType& > resourcesIterator, unsigned factor = 1 );
    void ReadCrew( xml::xistream& xis );
    void ReadEquipment( xml::xistream& xis );
    void ReadResource( xml::xistream& xis );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    bool IsCrewsSaveNeeded() const;
    bool IsEquipmentsSaveNeeded() const;
    bool IsResourcesSaveNeeded() const;
    const QString RetrieveResourceCategory( const QString& resourceName ) const;
    double RetrieveNormalizedConsumption( const QString& resourceName ) const;
    double RetrieveDefaultLogisticThreshold( const QString& resourceName ) const;
    void UpdateEquipmentsWithOriginal() ;
    //@}

public:
    //! @name Public member data
    //@{
    T_Equipments equipments_;
    T_Crews      crews_;
    T_Resources  resources_;
    //@}

private:
    //! @name Private member data
    //@{
    const StaticModel& staticModel_;
    unsigned int       typeId_;

    T_Equipments originalEquipments_;
    T_Crews      originalCrews_;
    T_Resources  originalResources_;

    std::vector< std::string > unknownResources_;
    //@}
};

#endif // __InitialState_h_
