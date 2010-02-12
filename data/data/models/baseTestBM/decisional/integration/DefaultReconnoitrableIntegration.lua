--Default Reconnoitrable Implementation

--- Get current UrbanBlock  reconnaissanceState
-- @param knowledge on an urbanBlock
-- @author MGD
-- @release 2010-02-11
integration.getUrbanBlockReconnaissanceState = function( urbanBlock )
  return integration.getUrbanBlockPerception( urbanBlock )
end

integration.getPointReconnaissanceState = function( point )
  return 0
end

integration.startRecceUrbanBlock = function( urbanBlock )
  urbanBlock.recceAction = DEC_Perception_ActiverReconnaissanceDansBlocUrbain( urbanBlock.source )
  return true
end

integration.stopRecceUrbanBlock  = function( urbanBlock )
  DEC_Perception_DesactiverReconnaissanceDansBlocUrbain( urbanBlock.recceAction )
  return true
end

integration.startReccePoint = function( point )
  point.recceAction = DEC_Perception_ActiverReconnaissancePoint( point.source, 100, 2 )
  return true
end

integration.stopReccePoint  = function( point )
  DEC_Perception_DesactiverReconnaissancePoint( point.recceAction )
  return true
end

integration.startRecceUrbanBlock = function( urbanBlock )
  urbanBlock.recceAction = DEC_Perception_ActiverReconnaissanceDansBlocUrbain( urbanBlock.source )
  return true
end

integration.stopRecceUrbanBlock  = function( urbanBlock )
  DEC_Perception_DesactiverReconnaissanceDansBlocUrbain( urbanBlock.recceAction )
  return true
end

