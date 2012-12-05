// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FilterInputArgument_h_
#define __FilterInputArgument_h_

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class XmlDescription;
}

class FilterPartiesListView;

// =============================================================================
/** @class  FilterInputArgument
    @brief  FilterInputArgument
*/
// Created: ABR 2011-09-28
// =============================================================================
class FilterInputArgument : public QObject
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit FilterInputArgument( const tools::ExerciseConfig& config, const std::string& argumentValue, const kernel::XmlDescription& description, const std::string& exerciseDir = "" );
    virtual ~FilterInputArgument();
    //@}

    //! @name Operations
    //@{
    QWidget* CreateWidget( QWidget* parent );
    QString GetText() const;
    void Update();
    bool IsValid();
    //@}

private:
    //! @name Types
    //@{
    enum E_ArgumentType { eInput = 0, eFile = 1, eDirectory = 2, eTeamList = 3 };
    //@}

signals:
    //! @name Signals
    //@{
    void ValueChanged();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnBrowse();
    void OnTextChanged( const QString& text = "" );
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    E_ArgumentType               type_;
    QLineEdit*                   line_;
    FilterPartiesListView*       listView_;
    QLabel*                      errorLabel_;
    const std::string            exerciseDir_;
    const std::string            description_;
    //@}
};

#endif // __FilterInputArgument_h_
