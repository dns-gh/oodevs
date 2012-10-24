return
{

    init = function( self, params )
      self.scoutPoints = {}
      self.mainPoints = {}
      self.previous = {}
      self.delayPoints = {} 

      self.ptrDelayPoints = {}
      myself.leadData.pionsLima1 = {}
      myself.leadData.pionsLima2 = {}

      -- self.scoutPoints: Points sur les limas pour les pions d'eclairage
      meKnowledge.nbPionsScout = meKnowledge.nbPionsScout or 0
      self.pointsOnLimasScout = integration.getPointsLimas( eTypeLima_LCAR, meKnowledge.nbPionsScout )
      for _, points in pairs( self.pointsOnLimasScout ) do
       for _, point in pairs( points ) do
         self.scoutPoints[ #self.scoutPoints + 1 ] = CreateKnowledge( world.Point, point )
       end
      end
      self.numberEchelons = myself.taskParams.echelonNumber or 0
      if self.numberEchelons == NIL or self.numberEchelons == 0 then
        self.numberEchelons = 1 -- par défaut les pions décrochent sur 1 échelon
      end

      -- self.mainPoints: Points sur les limas pour les pions ayant la mission principale
      -- meKnowledge.nbPionsMain + meKnowledge.nbPionsMain % 2  nombre de points sont créés sur n echelons
      meKnowledge.nbPionsMain = meKnowledge.nbPionsMain or 0
      self.pointsOnLimas = integration.getPointsLimas( eTypeLima_LCAR, ( meKnowledge.nbPionsMain + meKnowledge.nbPionsMain % 2 ) /self.numberEchelons )
      self.list = {}
      self.listScout = {}
      local compt = 0
      for _, points in pairs( self.pointsOnLimas ) do
       for _, point in pairs( points ) do
         if meKnowledge.nbPionsMain % 2 == 0 or compt % ( meKnowledge.nbPionsMain + 1 ) ~= meKnowledge.nbPionsMain then
           self.mainPoints[ #self.mainPoints + 1 ] = CreateKnowledge( world.Point, point )
         end
         compt = compt + 1  
       end
      end 
      if not self.pointsOnLimas or #self.pointsOnLimas == 0 then
        integration.report( eRC_LimaParameterNotPresent, eTypeLima_LCAR )
      end
      if params.pcObjective and params.pcObjective ~= NIL then
        for i = 1, meKnowledge.nbPionsMain do
          self.mainPoints[ #self.mainPoints + 1 ] = params.pcObjective
        end
        for i = 1, meKnowledge.nbPionsScout do
          self.scoutPoints[ #self.scoutPoints + 1 ]  = params.pcObjective
        end
      end

      local nb = 1
      for pos = 1, ((#self.pointsOnLimas + #self.pointsOnLimas%2 )/self.numberEchelons + 1 ) do
        self.list[ pos ] = {}
        for i = 1, meKnowledge.nbPionsMain do  
          table.insert(self.list[ pos ], self.mainPoints[ nb ])
          nb = nb + 1
        end
      end
     
      local nbScout = 1
      for pos = 1, #self.pointsOnLimas + 1 do
        self.listScout[ pos ] = {}
        for i = 1, meKnowledge.nbPionsScout do  
          table.insert(self.listScout[ pos ], self.scoutPoints[ nbScout ])
          nbScout = nbScout + 1
        end
      end

    end,

    getReachable = function( self, params, entity )  
        if params.meetingPoint then
           return { params.meetingPoint }
        end
        return {}
    end,    
 
    getDelayObjectives = function( self, params, entity )
      local objectives = {}

      if not myself.leadData.currentMoveToPosition then
        myself.leadData.currentMoveToPosition = 0
      end
      myself.leadData.currentMoveToPosition = myself.leadData.currentMoveToPosition + 1
   
      for _, points in pairs( self.list ) do
        objectives[ #objectives + 1 ] = points[ myself.leadData.currentMoveToPosition ]
      end
      
       -- Mis à jour des echelons: A revoir pour un nombre d'échelons > 2
      if myself.leadData.currentMoveToPosition <= ( meKnowledge.nbPionsMain + meKnowledge.nbPionsMain%2)/self.numberEchelons  then
        myself.leadData.pionsLima1[entity] = entity
        F_Pion_SeteEtatEchelon( entity.source, eEtatEchelon_First )
      else
        myself.leadData.pionsLima2[entity] = entity
        F_Pion_SeteEtatEchelon( entity.source, eEtatEchelon_Second )
      end
      
      return objectives
    end,
    
    -- Screen objectives
    getScreenPositions = function( self, params, entity )
      local positions = {}
      
      if not myself.leadData.currentScoutPosition then
        myself.leadData.currentScoutPosition = 0
      end
      myself.leadData.currentScoutPosition = myself.leadData.currentScoutPosition + 1
      
      for _, points in pairs( self.listScout ) do
        positions[ #positions + 1 ] = points[ myself.leadData.currentScoutPosition ]
      end

      return positions
    end,

    getObstacles = function( self, params )
        return { params.obstacles }
    end,

    getNbrFront = function( self )
        return integration.query.getNbrFront( 1 )
    end,

    getWithImprovement = function( self, params )
        return params.withImprovement or false
    end,
}
