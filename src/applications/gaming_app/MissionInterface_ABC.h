// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionInterface_ABC_h_
#define __MissionInterface_ABC_h_

#include "game_asn/Asn.h"
#include "ParamComboBox.h"
#include "ParamRadioBtnGroup.h"

namespace kernel
{
    class ActionController;
    class Entity_ABC;
}

class Param_ABC;
class OptionalParamFunctor_ABC;
class Action_ABC;
class ActionsModel;

// =============================================================================
/** @class  MissionInterface_ABC
    @brief  MissionInterface_ABC
*/
// Created: APE 2004-04-20
// =============================================================================
class MissionInterface_ABC : public QVBox
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             MissionInterface_ABC( QWidget* parent, kernel::Entity_ABC& agent, kernel::ActionController& controller );
    virtual ~MissionInterface_ABC();
    //@}

    //! @name Operations
    //@{
    void Draw( const kernel::GlTools_ABC& tools, kernel::Viewport_ABC& extent ) const;
    bool IsEmpty() const;
    void AddParameter( Param_ABC& parameter, OptionalParamFunctor_ABC* pOptional );
    //@}

public slots:
    //! @name Slots
    //@{
    virtual void OnOk();
    //@}

protected:
    //! @name Helpers
    //@{
    void CreateOkCancelButtons();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionInterface_ABC( const MissionInterface_ABC& );
    MissionInterface_ABC& operator=( const MissionInterface_ABC& );
    //@}

    //! @name Helpers
    //@{
    bool CheckValidity();
    void Commit();
    virtual void Publish() = 0;
    //@}

    //! @name Types
    //@{
    typedef std::vector< Param_ABC* >      T_Parameters;
    typedef T_Parameters::const_iterator CIT_Parameters;

    typedef std::vector< OptionalParamFunctor_ABC* >  T_OptionalFunctors;
    typedef T_OptionalFunctors::const_iterator      CIT_OptionalFunctors;
    //@}

private:
    //! @name Member data
    //@{
    kernel::ActionController& controller_;
    kernel::Entity_ABC& agent_;

    T_Parameters  parameters_;
    T_OptionalFunctors optionalFunctors_;
};

#endif // __MissionInterface_ABC_h_
