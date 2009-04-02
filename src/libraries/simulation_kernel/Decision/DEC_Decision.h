// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Decision_h_
#define __DEC_Decision_h_

#include "MIL.h"
#include "Decision/DEC_Decision_ABC.h"

class MIL_Entity_ABC;

// =============================================================================
/** @class  DEC_Decision
    @brief  DEC_Decision
    Template T must be a subcless of MIL_Entity_ABC
*/
// Created: LDC 2009-02-27
// =============================================================================
template <class T >
class DEC_Decision : public DEC_Decision_ABC
                   , public DIA_Engine
{

public:
    //! @name Cosntructor
    //@{
             DEC_Decision( T& entity, const std::string& type );
    explicit DEC_Decision( const std::string& type );
    virtual ~DEC_Decision();
    //@}

    //! @name Operations
    //@{
    virtual void UpdateDecision();
    virtual void Reset();
    virtual DIA_Variable_ABC* ExecuteFunction( const std::string& szFuncName, DIA_Parameters& parameters );    
    //@}
    
    //!@name Accessors
    //@{
    DIA_BehaviorPart&   GetBehaviorPart () const;
    DIA_Knowledge_Part& GetKnowledgePart() const;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void CleanStateAfterCrash     () = 0;

            void StartDefaultBehavior     ();
            void StopDefaultBehavior      ();
            void LogCrash                 ();
    //@}

private://! @name Helpers
    //@{
    void HandleUpdateDecisionError ();
    bool IsDefaultBehaviorAvailable() const;
    //@}

protected:
    //!@name Data
    //@{
    T*                              pEntity_;
    DIA_Parameters                  defaultBehaviorParameters_;
    std::auto_ptr<DIA_Variable_Id>  pDefaultParameter_;
    //@}
};

#include "DEC_Decision.inl"

#endif // __DEC_Decision_h_
