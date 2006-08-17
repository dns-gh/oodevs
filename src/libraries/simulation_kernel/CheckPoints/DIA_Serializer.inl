// *****************************************************************************
//
// $Created: JVT 2005-04-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/CheckPoints/DIA_Serializer.inl $
// $Author: Jvt $
// $Modtime: 12/04/05 14:49 $
// $Revision: 2 $
// $Workfile: DIA_Serializer.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DIA_Serializer::load
// Created: JVT 2005-04-06
// -----------------------------------------------------------------------------
template< typename Archive > 
void DIA_Serializer::load( Archive& file, const uint )
{
    uint nNbr;
    file >> nNbr;
    while( nNbr-- )
    {
        std::string strName;
        file >> strName;
        
        DIA_Stimulus* pStimulus = motivationPart_.FindStimulus( strName.c_str() );
        assert( pStimulus );
        file >> pStimulus->rValue_;        
    }
    
    file >> nNbr;
    while( nNbr-- )
    {
        std::string strName;
        file >> strName;
        
        DIA_Emotion* pEmotion = motivationPart_.FindEmotion( strName.c_str() );
        assert( pEmotion );      
        file >> pEmotion->rCurrentValue_
             >> pEmotion->rCurrentAmpStimulusValue_
             >> pEmotion->rCurrentSurpriseValue_
             >> pEmotion->rCurrentHabitValue_
             >> pEmotion->rCurrentUpsetValue_
             >> pEmotion->rCurrentCrudeEmotionValue_
             >> pEmotion->rOldValue_;
    }
    
    file >> nNbr;
    while( nNbr-- )
    {
        std::string strName;
        file >> strName;
        
        DIA_StateVariable* pStateVar = motivationPart_.FindStateVariable( strName.c_str() );
        assert( pStateVar );
        file >> pStateVar->rValue_
             >> pStateVar->rOutputValue_
             >> pStateVar->rExternalEffects_;
    }
    
    file >> nNbr;
    while( nNbr-- )
    {
        std::string strName;
        file >> strName;
        
        DIA_Motivation* pMotivation = motivationPart_.FindMotivation( strName.c_str() );
        assert( pMotivation );
        file >> pMotivation->rValue_;        
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DIA_Serializer::save
// Created: JVT 2005-04-06
// -----------------------------------------------------------------------------
template< typename Archive > 
void DIA_Serializer::save( Archive& file, const uint ) const
{
    file << motivationPart_.stimulusList_.size();    
    for( CIT_StimulusVector itStimulus = motivationPart_.stimulusList_.begin(); itStimulus != motivationPart_.stimulusList_.end(); ++itStimulus )
    {
        const DIA_Stimulus& stimulus = **itStimulus;
        file << std::string( stimulus.GetName() )
             << stimulus.rValue_;
    }

    file << motivationPart_.emotionList_.size();    
    for( CIT_EmotionVector itEmotion = motivationPart_.emotionList_.begin(); itEmotion != motivationPart_.emotionList_.end(); ++itEmotion )
    {
        const DIA_Emotion& emotion = **itEmotion;
        file << std::string( emotion.GetName() )
             << emotion.rCurrentValue_
             << emotion.rCurrentAmpStimulusValue_
             << emotion.rCurrentSurpriseValue_
             << emotion.rCurrentHabitValue_
             << emotion.rCurrentUpsetValue_
             << emotion.rCurrentCrudeEmotionValue_
             << emotion.rOldValue_;
    }


    file << motivationPart_.stateVariableList_.size();    
    for( CIT_StateVariableVector itStateVar = motivationPart_.stateVariableList_.begin(); itStateVar != motivationPart_.stateVariableList_.end(); ++itStateVar )
    {
        const DIA_StateVariable& stateVar = **itStateVar;
        file << std::string( stateVar.GetName() )
             << stateVar.rValue_
             << stateVar.rOutputValue_
             << stateVar.rExternalEffects_;
    }


    file << motivationPart_.motivationList_.size();    
    for( CIT_MotivationVector itMotivation = motivationPart_.motivationList_.begin(); itMotivation != motivationPart_.motivationList_.end(); ++itMotivation )
    {
        const DIA_Motivation& motivation = **itMotivation;
        file << std::string( motivation.GetName() )
             << motivation.rValue_;
    }    
}
