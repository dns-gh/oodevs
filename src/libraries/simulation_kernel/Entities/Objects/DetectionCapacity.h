// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __DetectionCapacity_h_
#define __DetectionCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"
#include <boost/serialization/export.hpp>

class MIL_Agent_ABC;
class TER_Localisation;
// =============================================================================
/** @class  DetectionCapacity
    @brief  DetectionCapacity
*/
// Created: MGD 2009-03-05
// =============================================================================
class DetectionCapacity : public ObjectCapacity_ABC
                        , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DetectionCapacity();
    explicit DetectionCapacity( xml::xistream& xis );
    virtual ~DetectionCapacity();
    //@}

     //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );    
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( Object& object ) const;
    virtual void Register( Object& object );
    virtual void ProcessAgentInside( Object& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering( Object& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting( Object& object, MIL_Agent_ABC& agent );

    void AddDetector( Object& object, const MIL_Agent_ABC& agent );
    void ScaleLocalisation( TER_Localisation& localisation );
    //@}

    //! @name Helpers
    //@{
    void ReadAcquisitionTime( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< MIL_Agent_ABC* , int >                        T_AgentMap;
    typedef T_AgentMap::const_iterator                              CIT_AgentMap;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DetectionCapacity( const DetectionCapacity& );  //!< Copy constructor
    //@}

private:
    //! @name Members
    //@{
        float rDetectionTime_;
        float rRecognitionTime_;
        float rIdentificationTime_;

        T_AgentMap agentInsideMap_;
    //@}

};

BOOST_CLASS_EXPORT_KEY( DetectionCapacity )

#endif // __DetectionCapacity_h_
