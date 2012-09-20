integration.getCommanderPosition = function( commander )
    return DEC_Automate_Position( commander.source )
end

integration.getSubordinateAgentsFromCommander = function( commander )
    return DEC_Automate_PionsDeAutomateAvecPC( commander.source )
end

integration.getSubordinatePosition = function( subordinate )
    return DEC_Automate_Position( subordinate.source )
end
