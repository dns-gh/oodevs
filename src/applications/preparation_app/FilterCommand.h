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
#include "frontend/ProcessObserver_ABC.h"

class FilterInputArgument;

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
             FilterCommand( xml::xistream& xis, const tools::ExerciseConfig& config );
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
    bool SearchCommand( const tools::Path& ) const;
    std::string ConvertArgumentVariable( const std::string& value ) const;
    bool IsInputArgument( size_t index ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged();
    //@}

signals:
    //! @name Signals
    //@{
    void ForceSaveAndAddActionPlanning( const std::string& filename );
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

    //typedef std::vector< std::pair< std::string, std::string > > T_Arguments;
    typedef std::vector< FilterArgument >                        T_Arguments;
    typedef T_Arguments::iterator                               IT_Arguments;
    typedef T_Arguments::const_iterator                        CIT_Arguments;

    typedef std::map< size_t, FilterInputArgument* >             T_InputArguments;
    typedef T_InputArguments::const_iterator                   CIT_InputArguments;
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    const tools::Path            command_;
    std::string                  argumentsLine_;
    T_Arguments                  arguments_;
    T_InputArguments             inputArguments_;
    tools::Path                  path_;
    bool                         reloadExercise_;
    bool                         minimalDisplay_;
    bool                         nonBlocking_;
    QLabel*                      commandLabel_;
    //@}
};

#endif // __FilterCommand_h_
