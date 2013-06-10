// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "RandomPluginConfigPanel.h"
#include "moc_RandomPluginConfigPanel.cpp"
#include "CreateSession.h"
#include "clients_kernel/Tools.h"
#pragma warning( push, 0 )
#include "QtCore/qcoreapplication.h"
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtCore/qsettings.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qvalidator.h>
#pragma warning( pop )

using namespace frontend;

namespace
{
    void ReadGaussian( bool* distributions )
    {
        QSettings settings( "MASA Group", "SWORD" );
        for( int i = 0; i < RandomPluginConfigPanel::eContextsNbr; ++i )
            distributions[ i ] = settings.value( QString( "/sword/RandomDistribution" ) + QString::number( i ) ).toBool();
    }

    void ReadDeviation( double* deviations )
    {
        QSettings settings( "MASA Group", "SWORD" );
        for( int i = 0; i < RandomPluginConfigPanel::eContextsNbr; ++i )
            deviations[ i ] = settings.value( QString( "/sword/RandomDeviation" ) + QString::number( i ), 0.5 ).toDouble();
    }

    void ReadMean( double* means )
    {
        QSettings settings( "MASA Group", "SWORD" );
        for( int i = 0; i < RandomPluginConfigPanel::eContextsNbr; ++i )
            means[ i ] = settings.value( QString( "/sword/RandomMean" ) + QString::number( i ), 0.5 ).toDouble();
    }

    bool ReadHasSeed()
    {
        QSettings settings( "MASA Group", "SWORD" );
        return settings.value( "/sword/RandomHasSeed", false ).toBool();
    }

    int ReadSeed()
    {
        QSettings settings( "MASA Group", "SWORD" );
        return settings.value( "/sword/RandomSeed", 1 ).toInt();
    }

    class Validator : public QDoubleValidator
    {
    public:
        Validator( QObject* pParent )
            : QDoubleValidator( 0., 1., 10, pParent ) {}

        QValidator::State validate( QString& input, int& /*nPos*/ ) const
        {
            double b = bottom();
            double t = top();
            int d = decimals();

            QRegExp empty( QString::fromLatin1(" *-?\\.? *") );
            if( b >= 0 &&
                input.trimmed().startsWith(QString::fromLatin1("-")) )
                return Invalid;
            if( empty.exactMatch(input) )
                return Intermediate;
            bool ok = TRUE;
            double entered = input.toDouble( &ok );
            if( !ok )
                return Invalid;

            int i = input.indexOf( '.' );
            if( i >= 0 )
                if( d==0 )
                    return Invalid;
                else
                {
                    // has decimal point (but no E), now count digits after that
                    i++;
                    int j = i;
                    while( input[j].isDigit() )
                        j++;
                    if( j - i > d )
                        return Invalid;
                }

            if( entered > t )
                return Invalid;
            else if( entered < b )
                return Intermediate;
            else
               return Acceptable;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel constructor
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
RandomPluginConfigPanel::RandomPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
{
    hasSeed_ = new QCheckBox();
    connect( hasSeed_, SIGNAL( toggled( bool ) ), SLOT( OnSeedToggled() ) );
    seed_ = new QSpinBox();
    seed_->setRange( 1, std::numeric_limits< int >::max() );
    seed_->setSingleStep( 1 );
    bool bHasSeed = ReadHasSeed();
    seed_->setValue( ReadSeed() );
    seed_->setEnabled( bHasSeed );
    hasSeed_->setChecked( bHasSeed );

    contextLabel_ = new QLabel();
    contextList_ = new QComboBox();
    connect( contextList_, SIGNAL( activated( int ) ), SLOT( OnContextChanged( int ) ) );

    distributionLabel_ = new QLabel();
    distributionList_ = new QComboBox();
    connect( distributionList_, SIGNAL( activated( int ) ), SLOT( OnDistributionChanged( int ) ) );

    standardDeviationLabel_ = new QLabel();
    deviation_ = new QLineEdit();
    deviation_->setValidator( new Validator( deviation_ ) );
    connect( deviation_, SIGNAL( textChanged( const QString& ) ), SLOT( OnDeviationChanged( const QString& ) ) );

    meanLabel_ = new QLabel();
    mean_ = new QLineEdit();
    mean_->setValidator( new Validator( mean_ ) );
    connect( mean_, SIGNAL( textChanged( const QString& ) ), SLOT( OnMeanChanged( const QString& ) ) );

    defaultButton_ = new QPushButton();
    connect( defaultButton_, SIGNAL( clicked() ), this, SLOT( OnDefaultClicked() ) );

    mainBox_ = new QGroupBox();
    QGridLayout* commentBoxLayout = new QGridLayout( mainBox_ );
    commentBoxLayout->addWidget( hasSeed_, 0, 0, 1, 1 );
    commentBoxLayout->addWidget( seed_, 0, 1, 1, 1 );
    commentBoxLayout->addWidget( contextLabel_, 1, 0, 1, 1 );
    commentBoxLayout->addWidget( contextList_, 1, 1, 1, 1 );
    commentBoxLayout->addWidget( distributionLabel_, 2, 0, 1, 1 );
    commentBoxLayout->addWidget( distributionList_, 2, 1, 1, 1 );
    commentBoxLayout->addWidget( standardDeviationLabel_, 3, 0, 1, 1 );
    commentBoxLayout->addWidget( deviation_, 3, 1, 1, 1 );
    commentBoxLayout->addWidget( meanLabel_, 4, 0, 1, 1 );
    commentBoxLayout->addWidget( mean_, 4, 1, 1, 1 );
    commentBoxLayout->addWidget( defaultButton_, 5, 0, 1, 1 );

    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->addWidget( mainBox_ );
    layout->setMargin( 5 );
    ReadRandomValues();
    OnContextChanged( 0 );
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel destructor
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
RandomPluginConfigPanel::~RandomPluginConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::OnLanguageChanged
// Created: ABR 2011-11-10
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::OnLanguageChanged()
{
    mainBox_->setTitle(               tools::translate( "RandomPluginConfigPanel", "Random Generator" ) );
    hasSeed_->setText(                tools::translate( "RandomPluginConfigPanel", "Seed:" ) );
    contextLabel_->setText(           tools::translate( "RandomPluginConfigPanel", "Context:" ) );
    distributionLabel_->setText(      tools::translate( "RandomPluginConfigPanel", "Distribution:" ) );
    standardDeviationLabel_->setText( tools::translate( "RandomPluginConfigPanel", "Standard deviation:" ) );
    meanLabel_->setText(              tools::translate( "RandomPluginConfigPanel", "Mean:" ) );
    defaultButton_->setText(          tools::translate( "RandomPluginConfigPanel", "Set as default" ) );

    int currentIndex = ( contextList_->count() ) ? contextList_->currentIndex() : 0;
    contextList_->clear();
    contextList_->addItem( tools::translate( "RandomPluginConfigPanel", "Fire" ), eFire );
    contextList_->addItem( tools::translate( "RandomPluginConfigPanel", "Wounds" ), eWounds );
    contextList_->addItem( tools::translate( "RandomPluginConfigPanel", "Perception" ), ePerception );
    contextList_->addItem( tools::translate( "RandomPluginConfigPanel", "Breakdowns" ), eBreakdowns );
    contextList_->setCurrentIndex( currentIndex );

    currentIndex = ( distributionList_->count() ) ? distributionList_->currentIndex() : 0;
    distributionList_->clear();
    distributionList_->addItem( tools::translate( "RandomPluginConfigPanel", "Linear" ) );
    distributionList_->addItem( tools::translate( "RandomPluginConfigPanel", "Gaussian" ) );
    distributionList_->setCurrentIndex( currentIndex );
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::GetName
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
QString RandomPluginConfigPanel::GetName() const
{
    return tools::translate( "RandomPluginConfigPanel", "Random" );
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::Commit
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::Commit( const tools::Path& exercise, const tools::Path& session )
{
    frontend::CreateSession action( config_, exercise, session );
    action.SetOption( "session/config/simulation/random/@seed", hasSeed_->isChecked() ? seed_->value() : 0 );
    for( int i = 0; i < eContextsNbr; ++i ) // Fire, Wounds, Perception, Breakdowns
    {
        std::stringstream stream;
        stream << "session/config/simulation/random";
        stream << i;
        action.SetOption( stream.str() + "/@distribution", bDistributions_[ i ] );
        action.SetOption( stream.str() + "/@deviation"   , rDeviations_[ i ] );
        action.SetOption( stream.str() + "/@mean"        , rMeans_[ i ] );
    }
    action.Commit();
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::ReadRandomValues
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::ReadRandomValues()
{
    ReadGaussian( bDistributions_ );
    ReadDeviation( rDeviations_ );
    ReadMean( rMeans_ );
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::OnContextChanged
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::OnContextChanged( int index )
{
    ERandomContexts context = ( ERandomContexts ) index;
    if( context >= eFire && context < eContextsNbr )
    {
        distributionList_->setCurrentIndex( bDistributions_[ context ] ? 1 : 0 );
        deviation_->setText( QString::number( rDeviations_[ context ] ) );
        mean_->setText( QString::number( rMeans_[ context ] ) );
        standardDeviationLabel_->setVisible( bDistributions_[ context ] );
        deviation_->setVisible( bDistributions_[ context ] );
        mean_->setVisible( bDistributions_[ context ] );
        meanLabel_->setVisible( bDistributions_[ context ] );
    }
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::OnDistributionChanged
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::OnDistributionChanged( int index )
{
    standardDeviationLabel_->setVisible( index != 0 );
    deviation_->setVisible( index != 0 );
    meanLabel_->setVisible( index != 0 );
    mean_->setVisible( index != 0 );
    ERandomContexts context = ( ERandomContexts ) contextList_->currentIndex();
    if( context >= eFire && context < eContextsNbr )
    {
        bDistributions_[ context ] = ( index != 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::OnDeviationChanged
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::OnDeviationChanged( const QString& )
{
    if( deviation_->hasAcceptableInput() )
    {
        ERandomContexts context = ( ERandomContexts ) contextList_->currentIndex();
        if( context >= eFire && context < eContextsNbr )
            rDeviations_[ context ] = deviation_->text().toDouble();
    }
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::OnMeanChanged
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::OnMeanChanged( const QString& )
{
    if( mean_->hasAcceptableInput() )
    {
        ERandomContexts context = ( ERandomContexts ) contextList_->currentIndex();
        if( context >= eFire && context < eContextsNbr )
            rMeans_[ context ] = mean_->text().toDouble();
    }
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::OnSeedToggled
// Created: JSR 2010-07-01
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::OnSeedToggled()
{
    seed_->setEnabled( hasSeed_->isChecked() );
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::OnDefaultClicked
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::OnDefaultClicked()
{
    QSettings settings( "MASA Group", "SWORD" );
    for( int i = 0; i < eContextsNbr; ++i )
    {
        settings.setValue( QString( "/sword/RandomDistribution" ) + QString::number( i ), bDistributions_[ i ] );
        settings.setValue( QString( "/sword/RandomDeviation" ) + QString::number( i ), rDeviations_[ i ] );
        settings.setValue( QString( "/sword/RandomMean" ) + QString::number( i ), rMeans_[ i ] );
    }
    settings.setValue( "/sword/RandomHasSeed", hasSeed_->isChecked() );
    settings.setValue( "/sword/RandomSeed", seed_->value() );
}
