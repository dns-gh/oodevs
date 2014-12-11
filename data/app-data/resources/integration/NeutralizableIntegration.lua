-- --Neutralizable Implementation

--- Starts neutralizing the given agent knowledge
-- This method can only be called by an agent.
-- @see integration.stopNeutralizingIt
-- @param target Directia agent knowledge
integration.startNeutralizingIt = function( target )
    target[myself] = target[myself] or {} 
    if not target[myself].actionNeutralize then
        target[myself].actionNeutralize = _DEC__StartTirDirectSurComposantesMajeures( myself, -1, target.source, 100, eTirDirectNormal )
        actionCallbacks[ target[myself].actionNeutralize ] = function( arg ) target[myself].eTir = arg end
    end
end

--- Stops neutralizing the given agent knowledge
-- This method can only be called by an agent.
-- @see integration.startNeutralizingIt
-- @param target Directia agent knowledge
integration.stopNeutralizingIt = function( target )
    if target[myself] and target[myself].actionNeutralize then
        _DEC__StopAction( myself, target[myself].actionNeutralize )
        target[myself].actionNeutralize = nil
        g_myEnemy = nil
    end
end

--- Returns true if the provided agent is neutralized (by artillery fire), false otherwise.
-- If no agent is provided, this function considers this entity instead.
-- If no agent is provided, this function will fail if called by anything but an agent.
-- @param agent Simulation agent (optional)
-- @return Boolean
integration.isAgentNeutralized = function( agent )
    if agent then
        return agent:DEC_Agent_EstNeutralise()
    else
        return _DEC_Agent_EstNeutralise( myself )
    end
end