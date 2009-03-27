// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ContaminationCapacity_h_
#define __ContaminationCapacity_h_

#include "simulation_kernel/MIL.h"
#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"
#include <boost/ptr_container/ptr_vector.hpp>

class MIL_NbcAgentType;
class MIL_Agent_ABC;

// =============================================================================
/** @class  ContaminationCapacity
    @brief  ContaminationCapacity
*/
// Created: JCR 2008-06-02
// =============================================================================
class ContaminationCapacity  
    : public ObjectCapacity_ABC
    , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ContaminationCapacity( xml::xistream& xis );
             ContaminationCapacity();
    virtual ~ContaminationCapacity();
    //@}

     //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );    
    virtual void Register( Object& /*object*/ );
    //@}

	//! @name virtual
    //@{
    virtual void Instanciate( Object& object ) const;	
	//@}

    //! @name 
    //@{
    virtual void ProcessAgentInside( Object& object, MIL_Agent_ABC& agent );    
    //@}

    //! @name 
    //@{
    void DecontaminateZone( const TER_Localisation& zone );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::ptr_vector< TER_Localisation >   T_LocalisationVector;
    typedef T_LocalisationVector::const_iterator    CIT_LocalisationVector;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ContaminationCapacity( const ContaminationCapacity& );  //!< Copy constructor
    //@}

    //! @name Operations
    //@{
    bool IsInsideDecontaminatedZone( const MT_Vector2D& position ) const;
    //@}

private:
    //! @name 
    //@{
    std::string             type_;
    int                     maxToxic_;
    T_LocalisationVector    decontaminatedZones_;
    //@}
};

#endif // __ContaminationCapacity_h_
