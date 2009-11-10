-- see: http://www.postgis.fr/book/print/191#195195195

SELECT terrain_grid.id, sum(sde.st_length(population.shape)) as meters
FROM sword.population, sword.terrain_grid
WHERE sword.population.shape && sword.terrain_grid.shape
	AND sde.st_contains(terrain_grid.shape, population.shape)
 GROUP BY terrain_grid.id;
 
 