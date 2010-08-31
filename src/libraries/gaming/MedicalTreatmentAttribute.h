// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalTreatmentAttribute_h_
#define __MedicalTreatmentAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "tools/Resolver_ABC.h"
#include "protocol/Protocol.h"
#include <list>

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class MedicalTreatmentType;
}

// =============================================================================
/** @class  MedicalTreatmentAttribute
    @brief  MedicalTreatmentAttribute
*/
// Created: RFT 2008-02-14
// =============================================================================
class MedicalTreatmentAttribute : public kernel::MedicalTreatmentAttribute_ABC
                    , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectKnowledgeUpdate >
                    , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectUpdate >
                    , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectCreation >
{
private:
    typedef std::list< const kernel::MedicalTreatmentType* > T_MedicalTreatmentTypeList;
    typedef T_MedicalTreatmentTypeList::const_iterator       CIT_MedicalTreatmentTypeList;
    typedef T_MedicalTreatmentTypeList::iterator             IT_MedicalTreatmentTypeList;


public:
    //! @name Constructors/Destructor
    //@{
             MedicalTreatmentAttribute( kernel::Controller& controller, const tools::Resolver_ABC< kernel::MedicalTreatmentType >& resolver );
    virtual ~MedicalTreatmentAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MedicalTreatmentAttribute( const MedicalTreatmentAttribute& );            //!< Copy constructor
    MedicalTreatmentAttribute& operator=( const MedicalTreatmentAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectUpdate& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectCreation& message );
    template< typename T >
    void UpdateData( const T& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::MedicalTreatmentType >& resolver_;
    T_MedicalTreatmentTypeList typeList_;
    int nMedicalTreatmentType_;
    int beds_;
    int availableBeds_;
    int doctors_;
    int availableDoctors_;
    //@}
};

#endif // __MedicalTreatmentAttribute_h_
