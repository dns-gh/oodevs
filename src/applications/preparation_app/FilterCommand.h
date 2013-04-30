// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FilterCommand_h_
#define __FilterCommand_h_

#include "Filter.h"
#include "preparation/ConsistencyErrorTypes.h"
#include "frontend/ProcessObserver_ABC.h"

namespace gui
{
    class ConsistencyDialog_ABC;
}

class FilterInputArgument;
class Model;

// =============================================================================
/** @class  FilterCommand
    @brief  FilterCommand
*/
// Created: ABR 2011-06-17
// =============================================================================
class FilterCommand : public Filter
                    , public frontend::ProcessObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             FilterCommand( xml::xistream& xis, const tools::ExerciseConfig& config, Model& model, gui::ConsistencyDialog_ABC& consistency );
    virtual ~FilterCommand();
    //@}

    //! @name Filter_ABC implementation
    //@{
    virtual void Execute();
    virtual const std::string GetName() const;
    virtual QWidget* CreateParametersWidget( QWidget* parent );
    virtual bool IsValid() const;
    virtual bool NeedToReloadExercise() const;
    virtual void Update();
    //@}

    //! @name ProcessObserver_ABC implementation
    //@{
    virtual void NotifyStopped();
    virtual void NotifyError( const std::string& error, std::string /*commanderEndpoint*/ = ""  );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadArguments( xml::xistream& xis );
    void ReadArgument( xml::xistream& xis );
    void ComputeArgument();
    void ComputePath();
    std::string ConvertArgumentVariable( const std::string& value ) const;
    bool IsInputArgument( int index ) const;
    void ReadError( xml::xistream& xis );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged( const QString& );
    void OnLogFileChanged( const QString& );
    void OnFinished();
    //@}

signals:
    //! @name Signals
    //@{
    void ForceSaveAndAddActionPlanning( const std::string& filename );
    void Finished();
    //@}

private:
    //! @name Type
    //@{
    struct FilterArgument
    {
        std::string name_;
        std::string displayName_;
        std::string value_;
    };

    typedef std::vector< FilterArgument > T_Arguments;

    typedef std::map< size_t, FilterInputArgument* > T_InputArguments;
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    Model&                       model_;
    gui::ConsistencyDialog_ABC&  consistency_;
    const std::string            command_;
    std::string                  argumentsLine_;
    T_Arguments                  arguments_;
    T_InputArguments             inputArguments_;
    std::string                  path_;
    bool                         reloadExercise_;
    bool                         minimalDisplay_;
    bool                         nonBlocking_;
    QLabel*                      commandLabel_;
    std::string                  logFile_;
    //@}
};

#endif // __FilterCommand_h_
