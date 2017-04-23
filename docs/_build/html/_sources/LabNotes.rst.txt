
GE using DEAP pacakge
=====================

https://github.com/DEAP/deap/

Exp1 - Onemax using GE
~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

    import random
    
    from deap import base
    from deap import creator
    from deap import tools
    import matplotlib.pyplot as plt
    %matplotlib inline  

.. code:: python

    creator.create("FitnessMax", base.Fitness, weights=(1.0,))
    creator.create("Individual", list, fitness=creator.FitnessMax)
    
    toolbox = base.Toolbox()
    toolbox.register("attr_bool", random.randint, 0, 1)
    
    # Attribute generator 
    #                      define 'attr_bool' to be an attribute ('gene')
    #                      which corresponds to integers sampled uniformly
    #                      from the range [0,1] (i.e. 0 or 1 with equal
    #                      probability)
    toolbox.register("attr_bool", random.randint, 0, 1)
    
    # Structure initializers
    #                         define 'individual' to be an individual
    #                         consisting of 100 'attr_bool' elements ('genes')
    toolbox.register("individual", tools.initRepeat, creator.Individual, 
        toolbox.attr_bool, 10)
    
    # define the population to be a list of individuals
    toolbox.register("population", tools.initRepeat, list, toolbox.individual)
    
    # the goal ('fitness') function to be maximized

.. code:: python

    # the goal ('fitness') function to be maximized
    def evalOneMax(individual):
        return sum(individual),
    
    #----------
    # Operator registration
    #----------
    # register the goal / fitness function
    toolbox.register("evaluate", evalOneMax)
    
    # register the crossover operator
    toolbox.register("mate", tools.cxTwoPoint)
    
    # register a mutation operator with a probability to
    # flip each attribute/gene of 0.05
    toolbox.register("mutate", tools.mutFlipBit, indpb=0.05)
    
    # operator for selecting individuals for breeding the next
    # generation: each individual of the current generation
    # is replaced by the 'fittest' (best) of three individuals
    # drawn randomly from the current generation.
    toolbox.register("select", tools.selTournament, tournsize=3)


.. code:: python

    #----------
    popFitNess = []
    def main():
        
        random.seed(64)
    
        # create an initial population of 300 individuals (where
        # each individual is a list of integers)
        pop = toolbox.population(n=5)
    
        # CXPB  is the probability with which two individuals
        #       are crossed
        #
        # MUTPB is the probability for mutating an individual
        #
        # NGEN  is the number of generations for which the
        #       evolution runs
        CXPB, MUTPB, NGEN = 0.5, 0.2, 10
        
        print("Start of evolution")
        
        # Evaluate the entire population
        fitnesses = list(map(toolbox.evaluate, pop))
        for ind, fit in zip(pop, fitnesses):
            ind.fitness.values = fit
        
        print("  Evaluated %i individuals" % len(pop))
        
        # Begin the evolution
        for g in range(NGEN):
            print("-- Generation %i --" % g)
            print(pop)
            
            
            # Select the next generation individuals
            offspring = toolbox.select(pop, len(pop))
            print("offspring 1 : ",offspring)
            # Clone the selected individuals
            offspring = list(map(toolbox.clone, offspring))
            
            print("off spring 2 : ",offspring)
            print("off spring[::2] : ",offspring[::2])
            print("off spring [1::2] : ",offspring[1::2])
            # Apply crossover and mutation on the offspring
            for child1, child2 in zip(offspring[::2], offspring[1::2]):
                print("Child 1 : ",child1)
                print("Child 2 : ",child2)
                # cross two individuals with probability CXPB
                if random.random() < CXPB:
                    toolbox.mate(child1, child2)
    
                    # fitness values of the children
                    # must be recalculated later
                    del child1.fitness.values
                    del child2.fitness.values
    
            for mutant in offspring:
    
                # mutate an individual with probability MUTPB
                if random.random() < MUTPB:
                    toolbox.mutate(mutant)
                    del mutant.fitness.values
        
            # Evaluate the individuals with an invalid fitness
            invalid_ind = [ind for ind in offspring if not ind.fitness.valid]
            fitnesses = map(toolbox.evaluate, invalid_ind)
            for ind, fit in zip(invalid_ind, fitnesses):
                ind.fitness.values = fit
            
            print("  Evaluated %i individuals" % len(invalid_ind))
            
            # The population is entirely replaced by the offspring
            pop[:] = offspring
            
            # Gather all the fitnesses in one list and print the stats
            fits = [ind.fitness.values[0] for ind in pop]
            
            length = len(pop)
            mean = sum(fits) / length
            sum2 = sum(x*x for x in fits)
            std = abs(sum2 / length - mean**2)**0.5
            
            # for analyis 
            popFitNess.append(mean)
            
            print("  Min %s" % min(fits))
            print("  Max %s" % max(fits))
            print("  Avg %s" % mean)
            print("  Std %s" % std)
        
        print("-- End of (successful) evolution --")
        
        best_ind = tools.selBest(pop, 1)[0]
        print("Best individual is %s, %s" % (best_ind, best_ind.fitness.values))
    
    if __name__ == "__main__":
        main()
        print(popFitNess)
        plt.plot(popFitNess)
        plt.ylabel('some numbers')
        plt.xlabel('Genrations')
        plt.show()


.. parsed-literal::

    Start of evolution
      Evaluated 5 individuals
    -- Generation 0 --
    [[1, 0, 1, 0, 0, 1, 0, 0, 0, 1], [1, 0, 0, 0, 1, 0, 0, 0, 0, 0], [1, 0, 0, 0, 0, 1, 1, 0, 1, 0], [1, 1, 1, 1, 1, 0, 0, 0, 0, 1], [0, 0, 0, 0, 1, 0, 0, 0, 1, 0]]
    offspring 1 :  [[1, 0, 0, 0, 0, 1, 1, 0, 1, 0], [0, 0, 0, 0, 1, 0, 0, 0, 1, 0], [1, 0, 0, 0, 0, 1, 1, 0, 1, 0], [1, 1, 1, 1, 1, 0, 0, 0, 0, 1], [1, 0, 0, 0, 0, 1, 1, 0, 1, 0]]
    off spring 2 :  [[1, 0, 0, 0, 0, 1, 1, 0, 1, 0], [0, 0, 0, 0, 1, 0, 0, 0, 1, 0], [1, 0, 0, 0, 0, 1, 1, 0, 1, 0], [1, 1, 1, 1, 1, 0, 0, 0, 0, 1], [1, 0, 0, 0, 0, 1, 1, 0, 1, 0]]
    off spring[::2] :  [[1, 0, 0, 0, 0, 1, 1, 0, 1, 0], [1, 0, 0, 0, 0, 1, 1, 0, 1, 0], [1, 0, 0, 0, 0, 1, 1, 0, 1, 0]]
    off spring [1::2] :  [[0, 0, 0, 0, 1, 0, 0, 0, 1, 0], [1, 1, 1, 1, 1, 0, 0, 0, 0, 1]]
    Child 1 :  [1, 0, 0, 0, 0, 1, 1, 0, 1, 0]
    Child 2 :  [0, 0, 0, 0, 1, 0, 0, 0, 1, 0]
    Child 1 :  [1, 0, 0, 0, 0, 1, 1, 0, 1, 0]
    Child 2 :  [1, 1, 1, 1, 1, 0, 0, 0, 0, 1]
      Evaluated 4 individuals
      Min 2.0
      Max 6.0
      Avg 4.2
      Std 1.3266499161421592
    -- Generation 1 --
    [[1, 0, 0, 0, 1, 0, 1, 1, 1, 0], [0, 0, 0, 0, 0, 1, 0, 0, 1, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 0, 0, 1, 1, 0, 0, 0, 0, 1], [1, 0, 0, 0, 0, 1, 1, 0, 1, 0]]
    offspring 1 :  [[1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 0, 0, 0, 1, 0, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 0, 0, 1, 1, 0, 0, 0, 0, 1], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0]]
    off spring 2 :  [[1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 0, 0, 0, 1, 0, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 0, 0, 1, 1, 0, 0, 0, 0, 1], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0]]
    off spring[::2] :  [[1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0]]
    off spring [1::2] :  [[1, 0, 0, 0, 1, 0, 1, 1, 1, 0], [1, 0, 0, 1, 1, 0, 0, 0, 0, 1]]
    Child 1 :  [1, 1, 1, 0, 0, 1, 1, 0, 1, 0]
    Child 2 :  [1, 0, 0, 0, 1, 0, 1, 1, 1, 0]
    Child 1 :  [1, 1, 1, 0, 0, 1, 1, 0, 1, 0]
    Child 2 :  [1, 0, 0, 1, 1, 0, 0, 0, 0, 1]
      Evaluated 3 individuals
      Min 5.0
      Max 6.0
      Avg 5.2
      Std 0.39999999999999575
    -- Generation 2 --
    [[1, 1, 1, 0, 0, 1, 1, 0, 0, 0], [1, 0, 0, 0, 1, 0, 1, 1, 1, 0], [1, 1, 1, 0, 0, 0, 1, 0, 1, 0], [1, 0, 0, 1, 1, 1, 0, 0, 0, 1], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0]]
    offspring 1 :  [[1, 1, 1, 0, 0, 1, 1, 0, 0, 0], [1, 0, 0, 0, 1, 0, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 0, 0, 0, 1, 0, 1, 1, 1, 0], [1, 1, 1, 0, 0, 0, 1, 0, 1, 0]]
    off spring 2 :  [[1, 1, 1, 0, 0, 1, 1, 0, 0, 0], [1, 0, 0, 0, 1, 0, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 0, 0, 0, 1, 0, 1, 1, 1, 0], [1, 1, 1, 0, 0, 0, 1, 0, 1, 0]]
    off spring[::2] :  [[1, 1, 1, 0, 0, 1, 1, 0, 0, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 1, 1, 0, 0, 0, 1, 0, 1, 0]]
    off spring [1::2] :  [[1, 0, 0, 0, 1, 0, 1, 1, 1, 0], [1, 0, 0, 0, 1, 0, 1, 1, 1, 0]]
    Child 1 :  [1, 1, 1, 0, 0, 1, 1, 0, 0, 0]
    Child 2 :  [1, 0, 0, 0, 1, 0, 1, 1, 1, 0]
    Child 1 :  [1, 1, 1, 0, 0, 1, 1, 0, 1, 0]
    Child 2 :  [1, 0, 0, 0, 1, 0, 1, 1, 1, 0]
      Evaluated 2 individuals
      Min 3.0
      Max 7.0
      Avg 5.2
      Std 1.3266499161421592
    -- Generation 3 --
    [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 0, 0, 0, 1, 0, 1, 0, 0, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 0, 0, 0, 1, 0, 1, 1, 1, 0], [1, 1, 1, 0, 0, 0, 1, 0, 1, 0]]
    offspring 1 :  [[1, 1, 1, 0, 0, 0, 1, 0, 1, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 1, 1, 0, 0, 0, 1, 0, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0]]
    off spring 2 :  [[1, 1, 1, 0, 0, 0, 1, 0, 1, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 1, 1, 0, 0, 0, 1, 0, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0]]
    off spring[::2] :  [[1, 1, 1, 0, 0, 0, 1, 0, 1, 0], [1, 1, 1, 0, 0, 0, 1, 0, 1, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0]]
    off spring [1::2] :  [[1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    Child 1 :  [1, 1, 1, 0, 0, 0, 1, 0, 1, 0]
    Child 2 :  [1, 1, 1, 0, 0, 1, 1, 0, 1, 0]
    Child 1 :  [1, 1, 1, 0, 0, 0, 1, 0, 1, 0]
    Child 2 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
      Evaluated 0 individuals
      Min 5.0
      Max 7.0
      Avg 5.8
      Std 0.7483314773547898
    -- Generation 4 --
    [[1, 1, 1, 0, 0, 0, 1, 0, 1, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 1, 1, 0, 0, 0, 1, 0, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0]]
    offspring 1 :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 0, 1, 0, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring 2 :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 0, 1, 0, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring[::2] :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 0, 1, 0, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring [1::2] :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    Child 1 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 2 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 1 :  [1, 1, 1, 0, 0, 0, 1, 0, 1, 0]
    Child 2 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
      Evaluated 2 individuals
      Min 6.0
      Max 7.0
      Avg 6.6
      Std 0.48989794855663765
    -- Generation 5 --
    [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 0, 1, 0], [1, 1, 1, 0, 0, 0, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    offspring 1 :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring 2 :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring[::2] :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring [1::2] :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    Child 1 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 2 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 1 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 2 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
      Evaluated 0 individuals
      Min 7.0
      Max 7.0
      Avg 7.0
      Std 0.0
    -- Generation 6 --
    [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    offspring 1 :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring 2 :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring[::2] :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring [1::2] :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    Child 1 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 2 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 1 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 2 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
      Evaluated 4 individuals
      Min 4.0
      Max 7.0
      Avg 6.4
      Std 1.1999999999999962
    -- Generation 7 --
    [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 0, 1, 0, 0, 1, 1, 0, 0, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    offspring 1 :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring 2 :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring[::2] :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring [1::2] :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    Child 1 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 2 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 1 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 2 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
      Evaluated 4 individuals
      Min 7.0
      Max 7.0
      Avg 7.0
      Std 0.0
    -- Generation 8 --
    [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    offspring 1 :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring 2 :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring[::2] :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring [1::2] :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    Child 1 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 2 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 1 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 2 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
      Evaluated 0 individuals
      Min 7.0
      Max 7.0
      Avg 7.0
      Std 0.0
    -- Generation 9 --
    [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    offspring 1 :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring 2 :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring[::2] :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    off spring [1::2] :  [[1, 1, 1, 0, 0, 1, 1, 1, 1, 0], [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]]
    Child 1 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 2 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 1 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
    Child 2 :  [1, 1, 1, 0, 0, 1, 1, 1, 1, 0]
      Evaluated 0 individuals
      Min 7.0
      Max 7.0
      Avg 7.0
      Std 0.0
    -- End of (successful) evolution --
    Best individual is [1, 1, 1, 0, 0, 1, 1, 1, 1, 0], (7.0,)
    [4.2, 5.2, 5.2, 5.8, 6.6, 7.0, 6.4, 7.0, 7.0, 7.0]



.. image:: output_4_1.png


.. code:: python

        plt.plot(popFitNess)
        plt.ylabel('Fitness in a population (mean)')
        plt.xlabel('Genrations')
        plt.show()
        plt.savefig('foo.png', bbox_inches='tight')



.. image:: output_5_0.png



.. parsed-literal::

    <matplotlib.figure.Figure at 0xac9d57ac>


.. code:: python

    ### Saving it to File 

.. code:: python

        plt.plot(popFitNess)
        plt.ylabel('Fitness in a population (mean)')
        plt.xlabel('Genrations')
        plt.savefig('foo.png', bbox_inches='tight')



.. image:: output_7_0.png


.. code:: python

        plt.plot(popFitNess)
        plt.ylabel('Fitness in a population (mean)')
        plt.xlabel('Genrations')
        plt.title("Fitness acorss 100 genration")
        plt.savefig('foo.pdf', bbox_inches='tight')



.. image:: output_8_0.png


Json Tutorial
=============

.. code:: python

    import json

.. code:: python

    json.dumps(['foo', {'bar': ('baz', None, 1.0, 2)}])




.. parsed-literal::

    '["foo", {"bar": ["baz", null, 1.0, 2]}]'



Stats for OneMax using GE
=========================

.. code:: python

    #    This file is part of DEAP.
    #
    #    DEAP is free software: you can redistribute it and/or modify
    #    it under the terms of the GNU Lesser General Public License as
    #    published by the Free Software Foundation, either version 3 of
    #    the License, or (at your option) any later version.
    #
    #    DEAP is distributed in the hope that it will be useful,
    #    but WITHOUT ANY WARRANTY; without even the implied warranty of
    #    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    #    GNU Lesser General Public License for more details.
    #
    #    You should have received a copy of the GNU Lesser General Public
    #    License along with DEAP. If not, see <http://www.gnu.org/licenses/>.
    
    
    #    example which maximizes the sum of a list of integers
    #    each of which can be 0 or 1
    
    import random
    
    from deap import base
    from deap import creator
    from deap import tools
    
    creator.create("FitnessMax", base.Fitness, weights=(1.0,))
    creator.create("Individual", list, fitness=creator.FitnessMax)
    
    toolbox = base.Toolbox()
    
    # Attribute generator 
    #                      define 'attr_bool' to be an attribute ('gene')
    #                      which corresponds to integers sampled uniformly
    #                      from the range [0,1] (i.e. 0 or 1 with equal
    #                      probability)
    toolbox.register("attr_bool", random.randint, 0, 1)
    
    # Structure initializers
    #                         define 'individual' to be an individual
    #                         consisting of 100 'attr_bool' elements ('genes')
    toolbox.register("individual", tools.initRepeat, creator.Individual, 
        toolbox.attr_bool, 5)
    
    # define the population to be a list of individuals
    toolbox.register("population", tools.initRepeat, list, toolbox.individual)
    
    # the goal ('fitness') function to be maximized
    def evalOneMax(individual):
        return sum(individual),
    
    #----------
    # Operator registration
    #----------
    # register the goal / fitness function
    toolbox.register("evaluate", evalOneMax)
    
    # register the crossover operator
    toolbox.register("mate", tools.cxTwoPoint)
    
    # register a mutation operator with a probability to
    # flip each attribute/gene of 0.05
    toolbox.register("mutate", tools.mutFlipBit, indpb=0.05)
    
    # operator for selecting individuals for breeding the next
    # generation: each individual of the current generation
    # is replaced by the 'fittest' (best) of three individuals
    # drawn randomly from the current generation.
    toolbox.register("select", tools.selTournament, tournsize=3)
    
    #----------
    
    def main():
        random.seed(64)
    
        # create an initial population of 30 individuals (where
        # each individual is a list of integers)
        pop = toolbox.population(n=30)
    
        # CXPB  is the probability with which two individuals
        #       are crossed
        #
        # MUTPB is the probability for mutating an individual
        #
        # NGEN  is the number of generations for which the
        #       evolution runs
        CXPB, MUTPB, NGEN = 0.5, 0.2, 3
        
        print("Start of evolution")
        
        # Evaluate the entire population
        fitnesses = list(map(toolbox.evaluate, pop))
        for ind, fit in zip(pop, fitnesses):
            ind.fitness.values = fit
        
        print("  Evaluated %i individuals" % len(pop))
        
        # Begin the evolution
        for g in range(NGEN):
            print("-- Generation %i --" % g)
            
            # Select the next generation individuals
            offspring = toolbox.select(pop, len(pop))
            
            print("total offspring = %s" % len(offspring))
            print("ofspring contains %s" % offspring[0])
            # print(" offspring = %s" % list(offspring))
            
            # Clone the selected individuals
            offspring = list(map(toolbox.clone, offspring))
            print("cloned total offspring = %s" % len(offspring))
        
            # Apply crossover and mutation on the offspring
            for child1, child2 in zip(offspring[::2], offspring[1::2]):
    
                # cross two individuals with probability CXPB
                if random.random() < CXPB:
                    print("c1..",child1)
                    print("c2..",child2)
                    toolbox.mate(child1, child2)
                    print("....",toolbox.mate(child1, child2))
                    # fitness values of the children
                    # must be recalculated later
                    del child1.fitness.values
                    del child2.fitness.values
    
            for mutant in offspring:
    
                # mutate an individual with probability MUTPB
                if random.random() < MUTPB:
                    toolbox.mutate(mutant)
                    del mutant.fitness.values
        
            # Evaluate the individuals with an invalid fitness
            invalid_ind = [ind for ind in offspring if not ind.fitness.valid]
            fitnesses = map(toolbox.evaluate, invalid_ind)
            for ind, fit in zip(invalid_ind, fitnesses):
                ind.fitness.values = fit
            
            print("  Evaluated %i individuals" % len(invalid_ind))
            
            # The population is entirely replaced by the offspring
            pop[:] = offspring
            
            # Gather all the fitnesses in one list and print the stats
            fits = [ind.fitness.values[0] for ind in pop]
            print("@@@@@ind value", ind)
            print("@@@@@ind value", ind.fitness)
            
            length = len(pop)
            mean = sum(fits) / length
            sum2 = sum(x*x for x in fits)
            std = abs(sum2 / length - mean**2)**0.5
            
            print("  Min %s" % min(fits))
            print("  Max %s" % max(fits))
            print("  Avg %s" % mean)
            print("  Std %s" % std)
        
        print("-- End of (successful) evolution --")
        
        best_ind = tools.selBest(pop, 1)[0]
        print("Best individual is %s, %s" % (best_ind, best_ind.fitness.values))
    
    if __name__ == "__main__":
        main()


.. parsed-literal::

    Start of evolution
      Evaluated 30 individuals
    -- Generation 0 --
    total offspring = 30
    ofspring contains [0, 1, 1, 1, 0]
    cloned total offspring = 30
    c1.. [0, 1, 1, 1, 0]
    c2.. [0, 1, 1, 0, 1]
    .... ([0, 1, 1, 1, 0], [0, 1, 1, 0, 1])
    c1.. [1, 1, 0, 1, 1]
    c2.. [1, 1, 0, 1, 0]
    .... ([1, 1, 0, 1, 0], [1, 1, 0, 1, 1])
    c1.. [1, 0, 1, 1, 1]
    c2.. [0, 1, 1, 1, 1]
    .... ([1, 0, 1, 1, 1], [0, 1, 1, 1, 1])
    c1.. [0, 0, 0, 1, 1]
    c2.. [1, 1, 0, 1, 1]
    .... ([0, 1, 0, 1, 1], [1, 0, 0, 1, 1])
    c1.. [1, 1, 0, 1, 1]
    c2.. [0, 1, 1, 0, 1]
    .... ([1, 1, 1, 0, 1], [0, 1, 0, 1, 1])
    c1.. [1, 0, 0, 0, 0]
    c2.. [1, 0, 1, 0, 0]
    .... ([1, 0, 1, 0, 0], [1, 0, 0, 0, 0])
      Evaluated 17 individuals
    @@@@@ind value [1, 0, 0, 0, 0]
    @@@@@ind value (1.0,)
      Min 1.0
      Max 5.0
      Avg 3.3
      Std 0.9000000000000002
    -- Generation 1 --
    total offspring = 30
    ofspring contains [1, 1, 1, 1, 1]
    cloned total offspring = 30
    c1.. [1, 1, 1, 1, 1]
    c2.. [0, 1, 1, 1, 1]
    .... ([1, 1, 1, 1, 1], [0, 1, 1, 1, 1])
    c1.. [1, 1, 1, 1, 1]
    c2.. [1, 1, 0, 1, 1]
    .... ([1, 1, 0, 1, 1], [1, 1, 1, 1, 1])
    c1.. [1, 1, 1, 0, 1]
    c2.. [1, 0, 1, 1, 1]
    .... ([1, 0, 1, 0, 1], [1, 1, 1, 1, 1])
    c1.. [1, 1, 0, 1, 1]
    c2.. [0, 1, 0, 1, 1]
    .... ([1, 1, 0, 1, 1], [0, 1, 0, 1, 1])
    c1.. [0, 1, 1, 0, 1]
    c2.. [0, 1, 1, 1, 0]
    .... ([0, 1, 1, 1, 1], [0, 1, 1, 0, 0])
    c1.. [1, 1, 1, 1, 1]
    c2.. [1, 1, 1, 0, 1]
    .... ([1, 1, 1, 0, 1], [1, 1, 1, 1, 1])
    c1.. [0, 1, 0, 1, 1]
    c2.. [0, 0, 1, 1, 1]
    .... ([0, 0, 1, 1, 1], [0, 1, 0, 1, 1])
    c1.. [1, 1, 1, 1, 1]
    c2.. [0, 1, 1, 1, 1]
    .... ([1, 1, 1, 1, 1], [0, 1, 1, 1, 1])
      Evaluated 21 individuals
    @@@@@ind value [0, 1, 1, 1, 1]
    @@@@@ind value (4.0,)
      Min 2.0
      Max 5.0
      Avg 3.8666666666666667
      Std 0.76303487615064
    -- Generation 2 --
    total offspring = 30
    ofspring contains [1, 1, 1, 1, 1]
    cloned total offspring = 30
    c1.. [1, 1, 1, 1, 1]
    c2.. [1, 1, 0, 1, 1]
    .... ([1, 1, 0, 1, 1], [1, 1, 1, 1, 1])
    c1.. [1, 0, 1, 1, 1]
    c2.. [1, 1, 0, 1, 1]
    .... ([1, 0, 1, 1, 1], [1, 1, 0, 1, 1])
    c1.. [1, 1, 0, 1, 1]
    c2.. [1, 1, 1, 1, 1]
    .... ([1, 1, 1, 1, 1], [1, 1, 0, 1, 1])
    c1.. [1, 1, 1, 0, 1]
    c2.. [1, 1, 1, 1, 1]
    .... ([1, 1, 1, 0, 1], [1, 1, 1, 1, 1])
    c1.. [1, 1, 0, 1, 1]
    c2.. [0, 1, 1, 1, 1]
    .... ([1, 1, 1, 1, 1], [0, 1, 0, 1, 1])
    c1.. [1, 0, 1, 1, 1]
    c2.. [1, 1, 0, 1, 1]
    .... ([1, 1, 0, 1, 1], [1, 0, 1, 1, 1])
    c1.. [1, 1, 1, 1, 1]
    c2.. [1, 1, 1, 1, 1]
    .... ([1, 1, 1, 1, 1], [1, 1, 1, 1, 1])
    c1.. [1, 0, 1, 1, 1]
    c2.. [1, 1, 1, 1, 1]
    .... ([1, 1, 1, 1, 1], [1, 0, 1, 1, 1])
    c1.. [1, 1, 1, 1, 1]
    c2.. [1, 1, 1, 1, 1]
    .... ([1, 1, 1, 1, 1], [1, 1, 1, 1, 1])
      Evaluated 21 individuals
    @@@@@ind value [1, 1, 1, 1, 1]
    @@@@@ind value (5.0,)
      Min 3.0
      Max 5.0
      Avg 4.433333333333334
      Std 0.5587684871413363
    -- End of (successful) evolution --
    Best individual is [1, 1, 1, 1, 1], (5.0,)


Zip
---

.. code:: python

    def zip1(*iterables):
        # zip('ABCD', 'xy') --> Ax By
        sentinel = object()
        iterators = [iter(it) for it in iterables]
        while iterators:
            result = []
            for it in iterators:
                elem = next(it, sentinel)
                if elem is sentinel:
                    return
                result.append(elem)
            yield tuple(result)

.. code:: python

    x = [1, 2, 3]
    y = [4, 5, 6]
    zipped = zip(x, y)
    list(zipped)





.. parsed-literal::

    [(1, 4), (2, 5), (3, 6)]



.. code:: python

    
    x2, y2 = zip(*zip(x, y))
    x == list(x2) and y == list(y2)





.. parsed-literal::

    True



.. code:: python

    a = b = c = range(20)
    z2 = zip(a, b, c)


.. code:: python

    list(z2)




.. parsed-literal::

    [(0, 0, 0),
     (1, 1, 1),
     (2, 2, 2),
     (3, 3, 3),
     (4, 4, 4),
     (5, 5, 5),
     (6, 6, 6),
     (7, 7, 7),
     (8, 8, 8),
     (9, 9, 9),
     (10, 10, 10),
     (11, 11, 11),
     (12, 12, 12),
     (13, 13, 13),
     (14, 14, 14),
     (15, 15, 15),
     (16, 16, 16),
     (17, 17, 17),
     (18, 18, 18),
     (19, 19, 19)]



.. code:: python

     for x, y, z  in zip(*z2):
            print(x)
            print(y)
            print(z)

.. code:: python

    c={'gaurav':'waghs','nilesh':'kashid','ramesh':'sawant','anu':'raje'}
    d={'amit':'wagh','swapnil':'dalavi','anish':'mane','raghu':'rokda'}
    list(zip(c,d))




.. parsed-literal::

    [('gaurav', 'anish'),
     ('ramesh', 'raghu'),
     ('nilesh', 'amit'),
     ('anu', 'swapnil')]



.. code:: python

    list?


Offspring for onemax
~~~~~~~~~~~~~~~~~~~~

.. code:: python

    test_offspring = [[0, 1, 0, 1, 1], [0, 0, 1, 1, 1], [0, 1, 1, 1, 0], [1, 1, 1, 1, 0]]

.. code:: python

    test_offspring




.. parsed-literal::

    [[0, 1, 0, 1, 1], [0, 0, 1, 1, 1], [0, 1, 1, 1, 0], [1, 1, 1, 1, 0]]



.. code:: python

    len(test_offspring)




.. parsed-literal::

    4



.. code:: python

    len(test_offspring[::2])




.. parsed-literal::

    2



.. code:: python

    len(test_offspring[1::2])




.. parsed-literal::

    2



.. code:: python

    part1 = test_offspring[::2]

.. code:: python

    part2 = test_offspring[1::2]

.. code:: python

    for child1, child2 in zip(part1, part2):
        print("c1 : ",child1)
        print("c2 : ",child2)


.. parsed-literal::

    c1 :  [0, 1, 0, 1, 1]
    c2 :  [0, 0, 1, 1, 1]
    c1 :  [0, 1, 1, 1, 0]
    c2 :  [1, 1, 1, 1, 0]


.. code:: python

    random.random() < 0.5 




.. parsed-literal::

    True



.. code:: python

    random.random() < 0.5 




.. parsed-literal::

    False



.. code:: python

    random.random() < 0.5 




.. parsed-literal::

    True



.. code:: python

    random.random() < 0.5 




.. parsed-literal::

    False



.. code:: python

    m = {"m1" : {"ram" : 4, "HDD" : 500, "cores" : 3 },
         "m2" : {"ram" : 12, "HDD" : 1000, "cores" : 5 } 
        }
    j = {"j1" : {"ram" : 200, "HDD" : 100, "cores" : 3},
         "j2" : {"ram" : 500, "HDD" : 50, "cores" : 5}, 
         "j3" : {"ram" : 100, "HDD" : 30, "cores" : 3}, 
         "j4" : {"ram" : 1024, "HDD" : 1, "cores" : 5}
        }
    
    #print(len(m))
    #print(len(j))
    
    #n_j_m = len(j)/len(m)
    #print(n_j_m)
    
    schedule_1 = {"m1" : ["j1", "j3"],
                "m2" : ["j2", "j4"]
               }
    schedule_2 = {"m1" : ["j1", "j2"],
                "m2" : ["j3", "j4"]
               }


.. code:: python

    def evaluate_schedule(schedule):
        l = len(schedule)
        fit = 0
        for key, values in schedule.items():
            #print(key)
            #print(values)
            #fit = 0
            max_core = 0
            t_r = 0
            t_h = 0
            m_c = m[key]
            #print(m_c)
            for item in values:
                #print(item)
                j_c = j[item]
                #print(j_c)
                if j_c["cores"] > max_core:
                    max_core = j_c["cores"]
                t_r += j_c["ram"]
                t_h += j_c["HDD"]
            if (max_core <= m_c["cores"]) and (t_r <= m_c["ram"] * 1024) and (t_h <= m_c["HDD"]):
                #print("Found a fit")
                fit += 1
        score = (fit/l)*100
        print("Score : %f"%score)
            
    evaluate_schedule(schedule_1)
    evaluate_schedule(schedule_2)


.. parsed-literal::

    Score : 100.000000
    Score : 50.000000


.. code:: python

    import math as m
    def crossover_1(schedule_1,schedule_2):
        """
        crosses over jobs on same machine in different schedule. example below :
        old schedule_1 :  {'m1': ['j1', 'j2'], 'm2': ['j3', 'j4']}
        old schedule_2 :  {'m1': ['j4', 'j3'], 'm2': ['j2', 'j1']}
    
        new schedule_1 :  {'m1': ['j1', 'j3'], 'm2': ['j3', 'j1']}
        new schedule_2 :  {'m1': ['j4', 'j2'], 'm2': ['j2', 'j4']}
        """
        l = len(schedule_1)
        n_s_1 = {}
        n_s_2 = {}
        for i in range(l):
            ith_machine_jobs_1 = schedule_1["m"+str(i+1)]
            ith_machine_jobs_2 = schedule_2["m"+str(i+1)]
            slice_point = 0
            if len(ith_machine_jobs_1) > len(ith_machine_jobs_2):
                slice_point = m.ceil(len(ith_machine_jobs_1)/2)
            else:
                slice_point = m.ceil(len(ith_machine_jobs_2)/2)
                
            new_1 = [item for item in ith_machine_jobs_1[:slice_point]]
            for item in ith_machine_jobs_2[slice_point:]:
                new_1.append(item)
            n_s_1.update({"m"+str(i+1) : new_1 })
            
            new_2 = [item for item in ith_machine_jobs_2[:slice_point]]
            for item in ith_machine_jobs_1[slice_point:]:
                new_2.append(item)
            
            n_s_2.update({"m"+str(i+1) : new_2 })
        print("old schedule_1 : ",schedule_1)
        print("old schedule_2 : ",schedule_2)
        print()
        print("new schedule_1 : ",n_s_1)
        print("new schedule_2 : ",n_s_2)
        return n_s_1, n_s_2
        
    crossover_1(schedule_1,schedule_2)



.. parsed-literal::

    old schedule_1 :  {'m1': ['j1', 'j3'], 'm2': ['j2', 'j4']}
    old schedule_2 :  {'m1': ['j1', 'j2'], 'm2': ['j3', 'j4']}
    
    new schedule_1 :  {'m1': ['j1', 'j2'], 'm2': ['j2', 'j4']}
    new schedule_2 :  {'m1': ['j1', 'j3'], 'm2': ['j3', 'j4']}




.. parsed-literal::

    ({'m1': ['j1', 'j2'], 'm2': ['j2', 'j4']},
     {'m1': ['j1', 'j3'], 'm2': ['j3', 'j4']})



.. code:: python

    def crossover_2(s_1, s_2):
        """
        crosses over jobs of the machine in the second part of this schedule with the second part of other schedule.
        example below :
        old schedule_1 :  {'m1': ['j1', 'j2'], 'm2': ['j3', 'j4']}
        old schedule_2 :  {'m1': ['j4', 'j3'], 'm2': ['j2', 'j1']}
    
        new schedule_1 :  {'m1': ['j1', 'j2'], 'm2': ['j2', 'j1']} 
        new schedule_2 :  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j4']}
        """
        l = len(s_1)
        n_s_1 = {}
        n_s_2 = {}
        slice_point = m.ceil(len(s_1)/2)
        for i in range(l):
            if i < slice_point:
                n_s_1.update({"m"+str(i+1) : s_1["m"+str(i+1)]})
                n_s_2.update({"m"+str(i+1) : s_2["m"+str(i+1)]})
            else:
                n_s_1.update({"m"+str(i+1) : s_2["m"+str(i+1)]})
                n_s_2.update({"m"+str(i+1) : s_1["m"+str(i+1)]})
        print("old schedule_1 : ",s_1)
        print("old schedule_2 : ",s_2)
        print()
        print("new schedule_1 : ",n_s_1)
        print("new schedule_2 : ",n_s_2)
        return n_s_1, n_s_2
    
    crossover_2(schedule_1,schedule_2)


.. parsed-literal::

    old schedule_1 :  {'m1': ['j1', 'j3'], 'm2': ['j2', 'j4']}
    old schedule_2 :  {'m1': ['j1', 'j2'], 'm2': ['j3', 'j4']}
    
    new schedule_1 :  {'m1': ['j1', 'j3'], 'm2': ['j3', 'j4']}
    new schedule_2 :  {'m1': ['j1', 'j2'], 'm2': ['j2', 'j4']}




.. parsed-literal::

    ({'m1': ['j1', 'j3'], 'm2': ['j3', 'j4']},
     {'m1': ['j1', 'j2'], 'm2': ['j2', 'j4']})



.. code:: python

    import random
    def crossover(s_1, s_2):
        choice = random.randint(0,1)
        print(choice)
        if choice == 0:
            crossover_1(s_1, s_2)
        else:
            crossover_2(s_1, s_2)
    
    crossover(schedule_1, schedule_2)


.. parsed-literal::

    1
    old schedule_1 :  {'m1': ['j1', 'j3'], 'm2': ['j2', 'j4']}
    old schedule_2 :  {'m1': ['j1', 'j2'], 'm2': ['j3', 'j4']}
    
    new schedule_1 :  {'m1': ['j1', 'j3'], 'm2': ['j3', 'j4']}
    new schedule_2 :  {'m1': ['j1', 'j2'], 'm2': ['j2', 'j4']}


GA v2
=====

Knapsack

.. code:: python

    #    This file is part of DEAP.
    #
    #    DEAP is free software: you can redistribute it and/or modify
    #    it under the terms of the GNU Lesser General Public License as
    #    published by the Free Software Foundation, either version 3 of
    #    the License, or (at your option) any later version.
    #
    #    DEAP is distributed in the hope that it will be useful,
    #    but WITHOUT ANY WARRANTY; without even the implied warranty of
    #    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    #    GNU Lesser General Public License for more details.
    #
    #    You should have received a copy of the GNU Lesser General Public
    #    License along with DEAP. If not, see <http://www.gnu.org/licenses/>.
    
    import random
    
    import numpy
    
    from deap import algorithms
    from deap import base
    from deap import creator
    from deap import tools
    
    IND_INIT_SIZE = 5
    MAX_ITEM = 50
    MAX_WEIGHT = 50
    NBR_ITEMS = 20
    
    # To assure reproductibility, the RNG seed is set prior to the items
    # dict initialization. It is also seeded in main().
    random.seed(64)
    
    # Create the item dictionary: item name is an integer, and value is 
    # a (weight, value) 2-uple.
    items = {}
    # Create random items and store them in the items' dictionary.
    for i in range(NBR_ITEMS):
        items[i] = (random.randint(1, 10), random.uniform(0, 100))
    
    creator.create("Fitness", base.Fitness, weights=(-1.0, 1.0))
    creator.create("Individual", set, fitness=creator.Fitness)
    
    toolbox = base.Toolbox()
    
    # Attribute generator
    toolbox.register("attr_item", random.randrange, NBR_ITEMS)
    
    # Structure initializers
    toolbox.register("individual", tools.initRepeat, creator.Individual, 
        toolbox.attr_item, IND_INIT_SIZE)
    toolbox.register("population", tools.initRepeat, list, toolbox.individual)
    
    def evalKnapsack(individual):
        weight = 0.0
        value = 0.0
        for item in individual:
            weight += items[item][0]
            value += items[item][1]
        if len(individual) > MAX_ITEM or weight > MAX_WEIGHT:
            return 10000, 0             # Ensure overweighted bags are dominated
        return weight, value
    
    def cxSet(ind1, ind2):
        """Apply a crossover operation on input sets. The first child is the
        intersection of the two sets, the second child is the difference of the
        two sets.
        """
        temp = set(ind1)                # Used in order to keep type
        ind1 &= ind2                    # Intersection (inplace)
        ind2 ^= temp                    # Symmetric Difference (inplace)
        return ind1, ind2
        
    def mutSet(individual):
        """Mutation that pops or add an element."""
        if random.random() < 0.5:
            if len(individual) > 0:     # We cannot pop from an empty set
                individual.remove(random.choice(sorted(tuple(individual))))
        else:
            individual.add(random.randrange(NBR_ITEMS))
        return individual,
    
    toolbox.register("evaluate", evalKnapsack)
    toolbox.register("mate", cxSet)
    toolbox.register("mutate", mutSet)
    toolbox.register("select", tools.selNSGA2)
    
    def main():
        random.seed(64)
        NGEN = 50
        MU = 50
        LAMBDA = 100
        CXPB = 0.7
        MUTPB = 0.2
        
        pop = toolbox.population(n=MU)
        hof = tools.ParetoFront()
        stats = tools.Statistics(lambda ind: ind.fitness.values)
        stats.register("avg", numpy.mean, axis=0)
        stats.register("std", numpy.std, axis=0)
        stats.register("min", numpy.min, axis=0)
        stats.register("max", numpy.max, axis=0)
        
        algorithms.eaMuPlusLambda(pop, toolbox, MU, LAMBDA, CXPB, MUTPB, NGEN, stats,
                                  halloffame=hof)
        
        return pop, stats, hof
                     
    if __name__ == "__main__":
        main() 


.. parsed-literal::

    Individual({3, 12, 15, 17, 19})
    Individual({0, 8, 4, 6})
    Individual({0, 2, 6, 13, 14})
    Individual({0, 2, 12, 5})
    Individual({8, 1, 18, 3})
    Individual({0, 2, 6, 7, 17})
    Individual({4, 6, 9, 12, 14})
    Individual({9, 11, 13, 16, 17})
    Individual({1, 5, 11, 15, 16})
    Individual({0, 5, 6, 7, 14})
    Individual({8, 1, 6, 7})
    Individual({0, 9, 3, 5})
    Individual({18, 4, 7})
    Individual({8, 9, 13, 14, 19})
    Individual({8, 11, 4, 5})
    Individual({9, 3, 4, 1})
    Individual({1, 3, 5, 10, 16})
    Individual({1, 8, 13, 15, 17})
    Individual({16, 1, 10, 15})
    Individual({3, 4, 10, 16, 17})
    Individual({4, 9, 10, 16, 19})
    Individual({10, 11, 12, 4})
    Individual({0, 9, 19, 13})
    Individual({16, 12, 5, 7})
    Individual({2, 8, 14, 15, 18})
    Individual({0, 8, 9, 10, 13})
    Individual({3, 11, 15})
    Individual({1, 3, 15, 16, 17})
    Individual({0, 16, 2, 19})
    Individual({3, 12, 13})
    Individual({4, 5, 9, 11, 16})
    Individual({0, 3, 6, 10, 18})
    Individual({2, 4, 6, 7, 12})
    Individual({0, 4, 9, 13, 16})
    Individual({0, 4, 9, 13, 15})
    Individual({8, 10, 13, 16, 19})
    Individual({4, 6, 10, 14, 17})
    Individual({5, 6, 8, 10, 17})
    Individual({0, 1, 5, 10, 15})
    Individual({5, 6, 9, 10, 18})
    Individual({0, 8, 9, 12, 17})
    Individual({8, 11, 13, 14, 16})
    Individual({0, 1, 8, 11, 13})
    Individual({0, 2, 5, 12, 15})
    Individual({1, 13, 14, 16, 17})
    Individual({4, 12, 14, 16, 17})
    Individual({0, 11, 14})
    Individual({8, 16, 12, 0})
    Individual({0, 4, 13, 6})
    Individual({2, 7, 12, 14, 18})
    gen	nevals	avg                          	std                        	min                        	max                          
    0  	50    	[  22.78        210.00877715]	[  5.88316241  71.09309649]	[ 10.          49.69958685]	[  38.          345.35491309]
    Individual({4, 9, 10, 16, 19})
    Individual({2, 7, 8, 14, 15, 18})
    Individual()
    Individual({2, 4, 6, 7, 12, 16})
    Individual()
    Individual()
    Individual()
    Individual({16})
    Individual({1, 3, 16, 17})
    Individual({8, 10, 13, 14, 16, 19})
    Individual({16, 1, 15})
    Individual({0, 15})
    Individual({0, 1, 3, 8, 18})
    Individual()
    Individual({0})
    Individual({13, 14, 16, 17})
    Individual({12})
    Individual({12, 15})
    Individual({3})
    Individual()
    Individual({0, 16, 2, 19})
    Individual()
    Individual({8, 1})
    Individual({0, 4})
    Individual({9, 19})
    Individual({3})
    Individual({0})
    Individual({8, 11, 5})
    Individual({16, 10})
    Individual({14})
    Individual({8, 13, 14})
    Individual({13})
    Individual({16, 10})
    Individual({16, 17})
    Individual({3, 15})
    Individual()
    Individual()
    Individual({17})
    Individual()
    Individual({8, 9, 12, 13, 14, 19})
    Individual()
    Individual({8, 1})
    Individual({8, 16, 12})
    Individual({12, 5, 7})
    Individual({0})
    Individual({9, 4, 1})
    Individual({2, 7, 12, 14, 15, 18})
    Individual({0, 9, 19, 13})
    Individual({15})
    Individual({0, 6})
    Individual({17})
    Individual({13})
    Individual({0})
    Individual({0})
    Individual()
    Individual()
    Individual({1})
    Individual()
    Individual({0, 8, 12, 16, 17})
    Individual({0, 2, 5, 15})
    Individual()
    Individual({0, 11})
    Individual({15})
    Individual({1, 3, 14, 15, 16, 17})
    Individual({2, 6})
    Individual({18})
    Individual({18})
    Individual({1, 13, 16, 17})
    Individual({17, 14})
    Individual({12})
    Individual()
    Individual({0, 2})
    Individual({0, 5})
    Individual()
    Individual({9, 13})
    Individual({17})
    Individual({0, 3, 6, 10})
    Individual({2, 8, 14, 15, 18})
    Individual({8, 0, 12})
    Individual({6, 7})
    Individual()
    Individual({8})
    Individual({10, 4})
    Individual({0, 3})
    Individual({3})
    Individual({0, 9, 13})
    Individual({4, 9, 10, 16})
    1  	87    	[   9.96        145.20790666]	[  10.61312395  139.1868852 ]	[ 0.  0.]                  	[  45.          414.48478381]
    Individual({16, 14})
    Individual()
    Individual({13, 7})
    Individual({2})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 19, 13, 14})
    Individual({2})
    Individual()
    Individual()
    Individual()
    Individual({8})
    Individual()
    Individual()
    Individual()
    Individual({7})
    Individual()
    Individual({2, 8, 12, 14, 15, 18})
    Individual({16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8})
    Individual({8})
    Individual()
    Individual()
    Individual()
    Individual({8})
    Individual()
    Individual({9, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4})
    Individual({8, 11})
    Individual()
    Individual()
    Individual({11, 13, 14, 16})
    Individual()
    Individual()
    Individual()
    Individual({11})
    Individual({10, 4})
    Individual({2, 14, 15, 18})
    Individual({16})
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({10})
    Individual()
    Individual({4, 6, 10, 13, 14, 17})
    Individual({9})
    Individual({1, 10, 4})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({18})
    Individual({8})
    Individual({3, 12, 15})
    Individual()
    Individual({18})
    Individual()
    Individual({16})
    Individual({11})
    Individual()
    Individual({4})
    Individual()
    Individual({8})
    Individual({8})
    Individual({4, 13})
    Individual({16, 13})
    Individual()
    Individual()
    Individual({8, 11, 13, 14, 16})
    Individual({11, 3, 15})
    Individual()
    Individual()
    Individual()
    Individual({16, 14})
    Individual({13})
    2  	91    	[  3.26        61.20087478]  	[   7.44797959  125.53892809]	[ 0.  0.]                  	[  28.          414.48478381]
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({1})
    Individual()
    Individual({15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({9})
    Individual({8})
    Individual()
    Individual()
    Individual({15})
    Individual({16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10, 11, 4})
    Individual()
    Individual({15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({17})
    Individual()
    Individual()
    Individual({8, 16, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual({10, 19})
    Individual()
    Individual({8, 14})
    Individual()
    Individual()
    3  	88    	[  4.6         84.51641114]  	[   8.57438044  140.51459866]	[ 0.  0.]                  	[  28.          414.48478381]
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 19})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual({1})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({17})
    Individual({12})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16, 19})
    Individual()
    Individual({10, 19, 4})
    Individual()
    Individual()
    Individual()
    Individual({17})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({18})
    Individual()
    Individual({10})
    Individual({7})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({2, 7, 8, 14, 18})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    4  	92    	[  2.4         52.24310488]  	[   5.82065288  108.88598622]	[ 0.  0.]                  	[  28.          414.48478381]
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({3})
    Individual()
    Individual()
    Individual()
    Individual({8, 9, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({6})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 19})
    Individual()
    Individual()
    Individual()
    Individual({8})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({19})
    Individual({18})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({5})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({14})
    Individual({3, 8, 13, 14, 16})
    Individual()
    5  	87    	[  3.66        74.97342258]  	[   6.99316809  127.02866009]	[ 0.  0.]                  	[  28.          414.48478381]
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({0, 9, 10, 13})
    Individual({18})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({9})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 14})
    Individual()
    Individual({5})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 13})
    Individual({14})
    Individual({10})
    Individual()
    Individual({2})
    Individual()
    Individual({8, 17, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({16})
    Individual({15})
    Individual({16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    6  	82    	[   5.3         111.43072783]	[   7.61117599  142.76899122]	[ 0.  0.]                  	[  28.          414.48478381]
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 19})
    Individual()
    Individual({8, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10, 15})
    Individual()
    Individual({8, 14})
    Individual({8, 14})
    Individual()
    Individual({9})
    Individual()
    Individual()
    Individual({8, 14, 15})
    Individual({8, 14})
    Individual()
    Individual({8, 13, 14})
    Individual()
    Individual()
    Individual({0, 8, 9, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 13})
    Individual({8, 9, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({0, 8, 9, 13})
    Individual()
    Individual({0, 8, 9, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({0, 8, 9, 13})
    Individual({11})
    Individual({8, 0, 13, 14})
    Individual()
    Individual({0, 8, 9, 10, 13})
    Individual()
    Individual()
    Individual({8, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({0, 8, 9, 10, 13, 17})
    Individual()
    Individual({8, 13})
    Individual()
    Individual()
    Individual()
    Individual({8})
    Individual()
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual()
    7  	90    	[  3.38        76.37304048]  	[   6.06593769  116.8118772 ]	[ 0.  0.]                  	[  28.          414.48478381]
    Individual({8})
    Individual({8, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({14})
    Individual({1})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({18})
    Individual()
    Individual({8, 14})
    Individual()
    Individual({8, 14})
    Individual()
    Individual()
    Individual()
    Individual({8, 13})
    Individual()
    Individual()
    Individual({7})
    Individual({1, 10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 14})
    Individual({12})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8})
    Individual({17})
    Individual()
    Individual({8})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({5})
    Individual({8, 14})
    Individual({8})
    Individual({8, 13})
    Individual()
    Individual()
    8  	86    	[  3.12        71.66806998]  	[   6.50427552  126.61232639]	[ 0.  0.]                  	[  28.          414.48478381]
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({11})
    Individual({3})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7})
    Individual()
    Individual({6})
    Individual({4})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 13, 14})
    Individual()
    Individual({8, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8})
    Individual()
    Individual()
    Individual()
    Individual({19})
    Individual({18})
    Individual({8, 10, 13, 14, 16, 19})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10, 4})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    9  	89    	[  4.28        96.82282974]  	[   7.61587815  140.68614155]	[ 0.  0.]                  	[  28.          414.48478381]
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({6})
    Individual()
    Individual({0, 8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({8, 13})
    Individual({2})
    Individual()
    Individual({14})
    Individual({8, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({0, 8, 10, 13})
    Individual()
    Individual({8, 10, 11, 13, 14, 16, 19})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({8, 3, 13, 14})
    Individual()
    Individual()
    Individual({8, 13, 14, 16, 17})
    Individual()
    Individual({8, 9, 10, 13, 14, 16})
    Individual()
    Individual({8})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8})
    Individual()
    Individual({8, 19, 13, 14})
    Individual()
    Individual()
    Individual({8, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({15})
    Individual()
    Individual({13})
    Individual()
    Individual({5})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({19})
    Individual({5})
    Individual()
    10 	91    	[  4.66        94.20965502]  	[   8.72607586  154.59704209]	[ 0.  0.]                  	[  33.          436.40977463]
    Individual()
    Individual({8, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 11, 13, 14, 16, 19})
    Individual()
    Individual()
    Individual({5})
    Individual({8, 17, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual()
    Individual({13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16, 18, 19})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 19, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 13, 14, 7})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({3})
    Individual()
    Individual()
    Individual({19})
    Individual()
    Individual()
    Individual()
    Individual({13})
    Individual()
    Individual()
    Individual()
    Individual({8, 3, 13})
    Individual()
    Individual({3})
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16, 19})
    Individual()
    Individual()
    11 	94    	[  3.38        82.54801261]  	[   7.42937413  143.83372367]	[ 0.  0.]                  	[  34.         483.3208272]  
    Individual({8, 13})
    Individual({19})
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16})
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8})
    Individual()
    Individual({18})
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({7, 8, 12, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({3})
    Individual({3, 8, 13, 14, 16})
    Individual({8, 17, 13})
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 13, 7})
    Individual()
    Individual({8, 10})
    Individual()
    Individual({4})
    Individual()
    Individual()
    Individual()
    Individual({3})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({9})
    Individual()
    Individual({8, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual({8, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    12 	92    	[   4.72        112.19978461]	[   8.5370721   164.14270307]	[ 0.  0.]                  	[  34.         483.3208272]  
    Individual()
    Individual({8, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({0, 5, 8, 10, 13})
    Individual({10})
    Individual({0, 4, 8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual({17})
    Individual()
    Individual({8, 13})
    Individual()
    Individual({8, 13, 14})
    Individual()
    Individual({8, 9, 10, 14, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual({10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({1})
    Individual()
    Individual({4})
    Individual()
    Individual({8, 3, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual({10, 3})
    Individual()
    Individual()
    Individual({5})
    Individual()
    Individual()
    Individual({8})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 13, 14})
    Individual()
    13 	89    	[  4.14        95.86909694]  	[   8.66258622  165.78275753]	[ 0.  0.]                  	[  34.         483.3208272]  
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 16, 18, 19})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({5})
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({19})
    Individual({8, 10, 13, 14, 16})
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({17})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7})
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    14 	86    	[   5.7         125.59256851]	[  10.31552228  184.80667754]	[ 0.  0.]                  	[  38.        492.202995]    
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({11})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({8})
    Individual({9})
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 9, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual({0})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({17})
    Individual()
    Individual()
    Individual({0})
    Individual()
    Individual()
    Individual({8, 9, 10, 14, 16})
    Individual()
    Individual({8})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    15 	86    	[   9.02       201.1503762]  	[  11.73625153  198.65247411]	[ 0.  0.]                  	[  38.        492.202995]    
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({1, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({11})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual()
    Individual({8, 9, 10, 13, 14, 16})
    Individual()
    Individual({7, 8, 10, 13, 15, 16})
    Individual({8, 10})
    Individual({17})
    Individual()
    Individual({10, 13})
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({10})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4})
    Individual()
    Individual()
    Individual({8, 13})
    Individual()
    Individual()
    Individual({8, 13})
    Individual({15})
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 14, 15, 16})
    Individual({8, 9, 10, 13, 14, 16})
    Individual()
    Individual({8, 10, 13, 14, 15, 16})
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({10})
    16 	90    	[   6.          121.10597089]	[  11.05621997  186.00117203]	[ 0.  0.]                  	[  38.        492.202995]    
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({1})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({6})
    Individual({8, 16, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 15})
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 16, 10, 13})
    Individual({8, 10, 13, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 15, 16})
    Individual()
    Individual()
    17 	91    	[   7.02        145.26997395]	[  11.35163424  195.13724753]	[ 0.  0.]                  	[  38.        492.202995]    
    Individual({10, 13})
    Individual()
    Individual()
    Individual()
    Individual({3})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual({3})
    Individual()
    Individual({8, 10})
    Individual()
    Individual({8, 9, 10, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({7, 8, 13, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual({8, 10, 13, 15, 16})
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({15})
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual()
    Individual({8, 10})
    Individual()
    Individual({8})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10, 13, 15, 16})
    Individual({6})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 16, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({16, 10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({12})
    Individual({7, 8, 9, 10, 13, 14, 16})
    18 	87    	[   8.88        176.19932087]	[  12.36064723  206.72218973]	[ 0.  0.]                  	[  38.        492.202995]    
    Individual()
    Individual({7, 8, 10, 13, 15, 18})
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual({8, 10, 13, 15})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 15, 16})
    Individual({1})
    Individual({8, 10, 13, 14, 16, 18, 19})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 13, 14})
    Individual({16})
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 13})
    Individual({18})
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 7})
    Individual({3})
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16})
    Individual({10})
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual({7, 8, 10, 11, 13, 15})
    Individual({7, 8, 10, 13, 14, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16, 18, 19})
    Individual()
    Individual()
    Individual({8, 9, 10, 13, 14, 16})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual({19})
    Individual()
    Individual()
    Individual({13})
    Individual({7, 8, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 15})
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({8, 10, 11})
    Individual({5})
    Individual()
    19 	89    	[   9.          185.80512507]	[  13.14990494  205.56098522]	[ 0.  0.]                  	[  38.        492.202995]    
    Individual({8, 10, 12, 14, 16, 18, 19})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16})
    Individual({3})
    Individual()
    Individual({7, 8, 10, 13, 14, 15})
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual({3})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({1})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual({10})
    Individual()
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual({8, 10, 13, 14, 16})
    Individual({8, 10, 13})
    Individual({8, 10})
    Individual()
    Individual()
    Individual({8, 10, 13, 15})
    Individual({4, 7, 8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 15})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 12, 13, 14, 16, 19})
    Individual()
    Individual()
    Individual({4})
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual({8})
    Individual()
    Individual({8, 10, 13, 14, 16})
    Individual({8, 10, 13})
    20 	90    	[  14.26        270.31330726]	[  14.86312215  209.42601383]	[ 0.  0.]                  	[  38.        492.202995]    
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 7})
    Individual({7, 8, 10, 13, 14, 15})
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual({8, 10, 13})
    Individual({10, 12, 13, 14, 16, 18, 19})
    Individual({8, 10, 13, 14, 16})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13, 14, 16})
    Individual({8, 10, 13})
    Individual({0, 7, 8, 10, 13, 15, 18})
    Individual({8})
    Individual()
    Individual({8, 10, 13, 14})
    Individual({8, 10})
    Individual()
    Individual({8, 10, 13, 15})
    Individual({8, 10, 13, 14, 15, 16})
    Individual()
    Individual({8, 10, 14})
    Individual({8, 10, 13, 15})
    Individual({18})
    Individual({13})
    Individual()
    Individual({8, 10, 13, 15})
    Individual({8, 10, 13})
    Individual({8, 10})
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual({8, 10})
    Individual({8, 10, 13, 14})
    Individual()
    Individual({8, 10, 13, 14, 16, 18, 19})
    Individual()
    Individual({7, 8, 10, 13, 15})
    Individual({8, 10, 13, 14})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 13, 14, 15, 16})
    Individual({4, 8, 10, 13, 14})
    Individual({8, 10, 13, 14, 16})
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13, 15})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10})
    Individual({0, 8, 10, 12, 13, 14, 16, 18, 19})
    Individual({7, 8, 10, 13, 15})
    Individual({8, 10, 13})
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({10})
    Individual({8, 10, 13})
    Individual({7})
    Individual({8, 10})
    Individual({8, 10})
    Individual({8, 10, 13, 14})
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 15, 18})
    21 	89    	[  18.56        320.11359043]	[  16.31828422  199.16225476]	[ 0.  0.]                  	[  46.          504.68809431]
    Individual({8, 10, 13, 14, 15})
    Individual({8, 10, 13, 14})
    Individual()
    Individual({8, 10, 13, 14, 16, 18, 19})
    Individual()
    Individual({8, 10, 13, 14, 16})
    Individual({8, 10, 13, 14})
    Individual({7, 8, 10, 13, 14, 15, 16})
    Individual()
    Individual({8, 10, 13, 14, 16, 18, 19})
    Individual({8, 10})
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13, 14, 16, 18, 19})
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual({10})
    Individual({8, 10, 13, 14, 16})
    Individual({7, 8, 10, 13, 15})
    Individual({10})
    Individual({8, 10, 13})
    Individual({8, 10, 12, 13, 14, 16, 19})
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual({8, 10})
    Individual()
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual({0})
    Individual()
    Individual({7, 8, 10, 13, 15})
    Individual({8, 10, 13, 14})
    Individual({8, 10})
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual({8, 10})
    Individual()
    Individual({3})
    Individual({8, 10, 13})
    Individual()
    Individual({8, 10, 13, 14})
    Individual({10})
    Individual()
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual({18})
    Individual({8, 10, 13, 15, 16})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 10})
    Individual({8, 10, 13, 14, 16, 18, 19})
    Individual({7, 8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual({7, 8, 13, 14, 15})
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual({17})
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual()
    Individual({8, 10, 13, 14, 15})
    Individual({8, 9, 10, 12, 13, 14, 16, 18, 19})
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 14, 16, 18, 19})
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual({8, 10, 13})
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13, 14, 16})
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16, 19})
    Individual({8, 10, 13, 14, 16})
    Individual({11})
    Individual()
    22 	92    	[  10.44        224.04582546]	[  13.46723431  208.40787182]	[ 0.  0.]                  	[  46.          504.68809431]
    Individual({8, 9, 10, 12, 13, 16, 18, 19})
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual({8, 10, 7})
    Individual()
    Individual({8, 10, 13, 14, 16, 18, 19})
    Individual()
    Individual({8, 10, 4, 13})
    Individual({5, 8, 10, 13, 14, 15, 16})
    Individual({8, 10, 13, 7})
    Individual({8, 10, 13, 15})
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual({6})
    Individual({8, 10, 13})
    Individual()
    Individual({5})
    Individual({7, 8, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 16, 18, 19})
    Individual()
    Individual({1, 8, 9, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual({8, 10, 12, 13, 14, 16, 18, 19})
    Individual({8, 10, 13, 14})
    Individual()
    Individual({8, 10, 11})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({5, 8, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual({6})
    Individual({11})
    Individual({10})
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual({8, 10})
    Individual({8, 10})
    Individual()
    Individual()
    Individual({18})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13, 14, 15})
    Individual({8, 10, 13, 14})
    Individual({15})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({6, 8, 10, 13, 14, 15, 16})
    Individual()
    Individual({8, 10, 13})
    Individual({4, 8, 9, 10, 13, 14, 15, 16})
    23 	89    	[   7.88        208.66031791]	[   9.97524937  203.26322025]	[ 0.  0.]                  	[  31.          551.23467984]
    Individual({10})
    Individual()
    Individual({5})
    Individual({8, 9, 10, 13, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 15})
    Individual()
    Individual({8, 10})
    Individual({8, 10, 13, 14})
    Individual({7, 8, 10, 15})
    Individual()
    Individual({8, 9, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13, 14, 15})
    Individual({8, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual({7, 8, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual({8, 9, 10, 13, 15, 16})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({9})
    Individual({18})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({10})
    Individual({8, 10, 13})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({9})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual({8, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 15})
    Individual({8, 10, 13})
    24 	88    	[   9.48        249.03636129]	[  10.43310117  198.35214182]	[ 0.  0.]                  	[  31.          551.23467984]
    Individual({3})
    Individual({8, 10})
    Individual()
    Individual({8, 10, 13, 14, 15})
    Individual({8, 10, 13, 15})
    Individual()
    Individual({8, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 15})
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13, 14, 15})
    Individual()
    Individual({14})
    Individual()
    Individual({8, 10, 13, 14})
    Individual({8, 10})
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13, 15})
    Individual({8, 10, 13, 14})
    Individual({8, 9, 10, 14, 15, 16})
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual({8, 10})
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual({8, 10, 13, 15, 16})
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13, 15})
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual({8, 10, 13, 14})
    Individual({4, 8, 9, 10, 13, 14, 15, 16})
    Individual({12})
    Individual({10})
    Individual({6, 7, 8, 10, 13, 14, 15, 16})
    Individual({7, 8, 10, 13, 14, 15, 17})
    Individual({8, 10, 11, 13})
    Individual({8, 10, 13})
    Individual({8, 10, 13, 14})
    Individual({7, 8, 10, 13, 15, 18})
    Individual()
    Individual()
    Individual({8, 10, 11})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7, 8, 9, 10, 13, 14})
    Individual({8, 10, 13})
    Individual({10})
    Individual({4, 8, 9, 10, 13, 14})
    Individual()
    Individual({7, 10, 13, 15})
    Individual()
    Individual()
    Individual({13})
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual({8, 9, 10, 14, 15, 16})
    Individual({8, 10})
    Individual({10, 14})
    Individual()
    Individual()
    25 	89    	[   9.74        259.22144876]	[  10.21921719  193.68124563]	[ 0.  0.]                  	[  31.          551.23467984]
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 7})
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({8, 10, 13, 7})
    Individual({5, 8, 10, 13, 14, 15, 16})
    Individual()
    Individual({8, 10, 13, 15})
    Individual({8, 10, 13, 15})
    Individual({13})
    Individual()
    Individual()
    Individual({8, 10, 4, 13})
    Individual({8, 9, 10, 13, 14, 15, 16})
    Individual({8, 10, 13, 7})
    Individual({4, 7, 8, 10, 13, 19})
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13, 14, 15, 16})
    Individual()
    Individual({7, 8, 10, 11, 13, 15})
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual({7, 8, 10, 13, 14, 15, 16, 18})
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual({8, 10, 13, 15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({8, 9, 10, 13, 14, 15, 16, 17})
    Individual({8, 9, 10, 13, 15, 16})
    Individual({8, 10})
    Individual({8, 9, 10, 13, 14, 16})
    Individual({11})
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13, 14, 15})
    Individual()
    Individual({7, 8, 10, 13, 15})
    Individual({7, 8, 10, 13, 15, 18})
    Individual({1})
    Individual({8, 10, 11, 13, 14, 15})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual({7, 8, 10, 13})
    Individual({8, 10, 13})
    Individual({8, 10, 13, 15})
    Individual()
    Individual()
    Individual({8, 10})
    Individual({4, 7, 8, 9, 10, 13, 14, 15, 16})
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 18})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual({10})
    26 	86    	[  12.24        290.69068602]	[  11.2045705  198.7052511]  	[ 0.  0.]                  	[  32.          559.60127088]
    Individual({5, 8, 10, 13, 14, 15, 16})
    Individual({8, 10, 4, 13})
    Individual({0})
    Individual({7, 8, 10, 13, 14})
    Individual({8, 10, 13, 14, 15, 16})
    Individual({8, 10, 13, 14})
    Individual({8, 10})
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual({8, 9, 10, 13, 14, 15, 16, 18})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 15})
    Individual()
    Individual({4, 8, 10, 13})
    Individual({8, 10})
    Individual()
    Individual({8, 10, 13, 14})
    Individual({8, 9, 10, 13, 14, 16})
    Individual({8, 10, 13, 7})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13, 14})
    Individual()
    Individual({8, 9, 10, 14, 15, 16})
    Individual({8, 10, 13, 14, 15, 16})
    Individual({4, 8, 10, 13, 14, 15})
    Individual({7, 8, 10, 13, 14})
    Individual({10})
    Individual()
    Individual({4, 7, 8, 9, 13, 14, 15, 16})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 14})
    Individual({10, 4})
    Individual({0, 8, 9, 10, 13, 14, 15, 16})
    Individual({8, 10, 13})
    Individual({8, 10, 13, 14})
    Individual()
    Individual({8, 10, 13, 14, 15})
    Individual()
    Individual({11})
    Individual({8, 10, 13})
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13, 14, 15})
    Individual({7, 8, 10, 13, 14})
    Individual({8, 10, 11, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual({7, 8, 10, 13, 15})
    Individual()
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual()
    Individual({7, 8, 10, 13, 15})
    Individual({10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual({10, 4, 13})
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    27 	87    	[   6.12        200.12601646]	[   7.6305701   188.98511995]	[ 0.  0.]                  	[  32.          559.60127088]
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({8, 10, 13, 14, 15})
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual({4, 8, 9, 10, 13, 14, 16})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 7})
    Individual({8, 10, 13, 14, 15})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7, 8, 13, 14})
    Individual({8, 10, 13, 14})
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({7, 8, 10, 13, 14, 19})
    Individual({8, 10})
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual({8, 10, 13, 7})
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual({4})
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual({8, 10})
    Individual({7, 8, 10, 13, 14})
    Individual({4, 10, 13, 14, 15})
    Individual()
    28 	90    	[   5.46        173.68629391]	[   8.35035329  194.13713321]	[ 0.  0.]                  	[  32.          559.60127088]
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8})
    Individual({8, 10})
    Individual({8, 10, 13, 15})
    Individual({8, 10, 4, 13})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({6, 7, 8, 10, 13, 14, 15})
    Individual({12})
    Individual({13})
    Individual({10})
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual({8, 13, 14})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 10, 4, 13})
    Individual({4, 7, 8, 10})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({8})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 15})
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual({8, 10, 4})
    Individual({8, 10, 13, 14, 15})
    Individual({8, 10, 4, 13})
    Individual({8, 10})
    Individual({8, 10, 2, 13})
    Individual({11})
    Individual({8, 10, 13})
    Individual()
    Individual({7, 10, 13, 15})
    Individual({18})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 15})
    29 	94    	[   5.14        153.60942869]	[   8.53465875  196.89251588]	[ 0.  0.]                  	[  32.          559.60127088]
    Individual()
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13, 14})
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 14, 15})
    Individual({4, 7, 8, 10, 13, 14})
    Individual({4, 7, 8, 9, 10, 12, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({0, 8, 10, 13, 14, 15})
    Individual({17})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 2})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({18})
    Individual({8, 10})
    Individual()
    Individual({14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({13})
    Individual()
    Individual({6, 7, 8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    30 	93    	[   5.7         160.04836138]	[   9.55667306  202.73012538]	[ 0.  0.]                  	[  36.          568.48343867]
    Individual({7, 8, 10, 13, 15, 19})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual({8, 10})
    Individual({10})
    Individual()
    Individual({12})
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({5})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 7})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({4, 7, 8, 10, 13})
    Individual({10})
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual({10})
    Individual({7, 8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual({11})
    Individual()
    Individual()
    Individual({10})
    Individual({8})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({10})
    Individual({4})
    Individual()
    Individual({10, 13})
    Individual({7, 8, 10, 13, 15})
    Individual({8, 9, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15, 16})
    Individual()
    Individual({4})
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    31 	95    	[   5.7         175.36993944]	[   8.74585616  196.02530582]	[ 0.  0.]                  	[  36.          568.48343867]
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({8, 10, 13, 7})
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual({2})
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual({4, 7, 8, 10, 13})
    Individual()
    Individual()
    Individual({8, 10, 4, 13})
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 4})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({7, 8, 10, 13, 14})
    Individual({14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 15})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual({10})
    Individual({15})
    Individual({8, 10})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual()
    Individual({6})
    Individual({6})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual({7, 8, 10, 13, 15, 18})
    Individual()
    Individual({10})
    32 	78    	[   5.74        175.66843167]	[   8.75856153  196.28166654]	[ 0.  0.]                  	[  36.          568.48343867]
    Individual()
    Individual({4, 7, 8, 9, 13, 14, 15, 16})
    Individual({15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4, 7, 8, 13})
    Individual()
    Individual()
    Individual({8, 10, 14})
    Individual({1})
    Individual({7, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 7})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({7, 8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 15, 18})
    Individual({8, 10})
    Individual({10})
    Individual({10})
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual({7})
    Individual({8, 10, 13})
    Individual({4, 8, 9, 10, 13, 14, 16})
    Individual()
    Individual()
    Individual()
    Individual({10, 11})
    Individual()
    Individual()
    Individual()
    Individual({4, 7, 8, 10, 13})
    Individual()
    Individual({4, 7, 8, 9, 10, 13, 14, 15, 16})
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual({14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4, 7, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7, 8, 9, 10, 13, 14, 15})
    Individual({5})
    Individual({8, 10, 13})
    Individual({7, 8, 10, 13, 14})
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual({8, 10, 13, 7})
    Individual()
    Individual({12})
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual({10})
    Individual({8, 10, 4, 13})
    Individual()
    Individual({8, 10})
    Individual()
    Individual({13})
    Individual()
    Individual({4, 8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({7, 8, 10, 13, 14, 15, 17})
    Individual({8, 10, 13, 15})
    Individual()
    Individual()
    Individual()
    Individual()
    33 	91    	[   6.72        192.14030284]	[   9.63335871  208.7809893 ]	[ 0.  0.]                  	[  36.          568.48343867]
    Individual({8, 10})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10})
    Individual({4, 7, 8, 9, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7, 10, 13, 14})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual({13})
    Individual({4, 8, 10, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 4, 13})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({7, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10})
    Individual()
    Individual({5})
    Individual()
    Individual()
    Individual()
    Individual({8, 4, 13})
    Individual()
    Individual()
    Individual({19})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({16})
    Individual()
    Individual({4, 8, 10, 14, 15})
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual({8, 10, 4, 13})
    Individual({8, 10, 2, 13})
    Individual()
    Individual()
    34 	92    	[   7.28        196.69424357]	[  10.39815368  216.27735816]	[ 0.  0.]                  	[  36.          568.48343867]
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({6, 7, 8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual({8, 10, 14})
    Individual()
    Individual({7, 8, 10, 13, 14, 15, 18})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 13, 14})
    Individual({19})
    Individual({8, 10})
    Individual({8, 10, 13})
    Individual({8, 10, 13, 7})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({4, 8, 10, 12, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({2, 8, 10, 13, 18})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 2})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({9})
    Individual()
    Individual()
    Individual({4, 7, 8, 10, 13})
    Individual({8, 10})
    Individual()
    Individual()
    Individual({4, 7, 8, 9, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({18})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 4, 13})
    Individual({4, 8, 10, 12, 13, 14})
    Individual()
    Individual()
    Individual()
    35 	89    	[   8.02        231.63771291]	[   9.80711986  209.75037701]	[ 0.  0.]                  	[  36.          568.48343867]
    Individual()
    Individual()
    Individual()
    Individual({4, 7, 8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual({4, 8, 10, 12, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({10})
    Individual()
    Individual({8, 10})
    Individual({8, 10, 13, 15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 9, 10, 13})
    Individual()
    Individual()
    Individual({9})
    Individual()
    Individual({4})
    Individual({3})
    Individual({4, 8, 10, 13, 14, 15})
    Individual({10})
    Individual({8, 13})
    Individual({8, 10, 6})
    Individual({12})
    Individual({4, 7, 8, 10, 13})
    Individual()
    Individual({8, 10, 13, 15})
    Individual()
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual({12})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual({4, 7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual({10})
    Individual({3, 4, 8, 10, 13, 14})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 7})
    Individual({7, 10, 13, 14, 15})
    Individual()
    Individual({10, 2, 13})
    Individual({4, 8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual({4, 8, 9, 10, 13, 15, 16})
    Individual()
    Individual({8, 10, 4, 13})
    Individual()
    Individual({4, 7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual()
    Individual({0, 4, 7, 8, 10, 13})
    Individual({10})
    Individual()
    Individual({3})
    Individual({5, 7, 8, 10, 13, 15})
    Individual()
    Individual()
    36 	90    	[   8.04        232.59848901]	[   9.8080783   207.02589326]	[ 0.  0.]                  	[  36.          568.48343867]
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15, 16})
    Individual({8, 10, 4, 13})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({9})
    Individual()
    Individual({7, 8, 10, 13, 15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4, 8, 9, 10, 13, 14})
    Individual({10})
    Individual({16})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10, 13})
    Individual()
    Individual({8, 10, 4, 13})
    Individual({10})
    Individual({8, 10, 13, 15})
    Individual({4, 8, 10, 12, 13, 14})
    Individual({8, 10, 13, 15})
    Individual({8, 10, 13})
    Individual({7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 7})
    Individual({4, 8, 10, 13, 14})
    Individual({4, 8, 10, 13, 14})
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual({4, 7, 8, 10, 13})
    Individual()
    Individual({7, 8, 10, 13, 15})
    Individual()
    Individual({4, 7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual({4, 7, 8, 10, 13})
    Individual({4, 8, 10, 13, 14})
    Individual({4, 7, 8, 10, 13, 14})
    Individual({8, 10})
    Individual({4, 7, 8, 10, 13, 14})
    Individual({8, 10})
    Individual({8, 10, 13})
    Individual({8, 10, 4, 13})
    Individual({8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({2, 8, 10, 12, 13})
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({8, 10, 4, 13})
    Individual({10})
    Individual()
    Individual({8, 10, 13, 7})
    Individual()
    Individual()
    Individual({8, 10})
    Individual({8, 10, 13, 7})
    Individual()
    Individual()
    37 	85    	[   8.72        254.17230119]	[   9.31244329  204.65321872]	[ 0.  0.]                  	[  36.          568.48343867]
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual({1, 4, 7, 8, 10, 13, 14})
    Individual({10})
    Individual({7, 8, 10, 13, 14})
    Individual({4, 7, 8, 10, 13})
    Individual({7, 8, 10, 13})
    Individual({4, 6, 7, 8, 10, 13, 14})
    Individual({9})
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({4, 7, 8, 13})
    Individual({4, 8, 10, 14, 15})
    Individual()
    Individual({8, 10, 13})
    Individual({4, 7, 8, 10, 13, 14})
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual({4, 8, 9, 10, 14, 15})
    Individual()
    Individual({10})
    Individual()
    Individual({10})
    Individual()
    Individual({4, 7, 8, 13, 14})
    Individual({0})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4, 7, 8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual({4, 7, 8, 13, 14})
    Individual()
    Individual()
    Individual({4, 7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 7})
    Individual()
    Individual()
    Individual({7})
    Individual()
    Individual({8, 10, 4, 13})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({4, 8, 9, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual({4, 7, 8, 10, 13, 14})
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4, 7, 8, 10, 13})
    Individual()
    Individual({4, 8, 10, 14})
    Individual({18})
    Individual({4, 7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual({4, 10, 13, 14, 15})
    Individual({8, 10, 2, 13})
    38 	89    	[   7.48        207.61125864]	[   9.98847336  213.49904673]	[ 0.  0.]                  	[  36.          568.48343867]
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({4, 9, 10, 13, 14, 15})
    Individual({8, 10, 4, 13})
    Individual({8, 10, 13, 7})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({4, 7, 10, 13, 14})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual({4, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18})
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({3})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({5})
    Individual()
    Individual({10})
    Individual({8, 10, 4, 13})
    Individual()
    Individual({2, 10, 13, 18})
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({14})
    Individual({8, 10, 13, 14})
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual({8})
    Individual()
    Individual({0, 4, 8, 10, 13, 14, 15})
    Individual({2})
    Individual({4, 8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 15})
    Individual()
    Individual({4, 8, 9, 10, 13, 14})
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual()
    39 	92    	[   9.36       245.9486636]  	[  11.3256523   215.87109513]	[ 0.  0.]                  	[  42.          637.31948207]
    Individual({10})
    Individual({8, 10, 13, 15})
    Individual()
    Individual()
    Individual()
    Individual({2, 8, 10, 13, 18})
    Individual({7, 8, 10, 13, 14})
    Individual({2, 8, 10, 18})
    Individual()
    Individual()
    Individual({8, 10, 4, 13})
    Individual({13})
    Individual()
    Individual({8, 10, 2, 13})
    Individual({8, 10})
    Individual({4, 7, 9, 10, 13, 14, 15, 16})
    Individual()
    Individual()
    Individual()
    Individual({4, 8, 9, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual({4, 8, 10, 13, 14, 15})
    Individual({8, 10, 13})
    Individual({8, 10})
    Individual()
    Individual({8, 10, 13})
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual({8, 10})
    Individual()
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual({4, 7, 8, 9, 10, 13, 14, 15, 16})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4, 7, 8, 9, 10, 13, 15, 16})
    Individual({4, 6, 8, 9, 10, 13, 14, 15, 16})
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual({4, 8, 10, 11, 13})
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual({7, 8, 10, 14})
    Individual()
    Individual()
    Individual({4, 7, 8, 10, 13})
    Individual({8, 10, 13, 14})
    Individual()
    Individual({8, 10, 13, 7})
    Individual({8, 10, 13})
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10})
    Individual({8, 10})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({19})
    Individual({18})
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual({8, 10, 13, 15})
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual({2, 8, 10, 18})
    Individual({4, 8, 9, 13, 14, 15})
    Individual()
    Individual({6})
    Individual()
    Individual({4, 8, 10, 13})
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual({3, 7, 8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual({3, 4, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual({17})
    Individual()
    Individual()
    Individual({2, 10, 13, 18})
    Individual({14})
    40 	92    	[  10.98        271.16188039]	[  12.40401548  223.61565328]	[ 0.  0.]                  	[  42.          637.31948207]
    Individual()
    Individual()
    Individual({8, 10})
    Individual({10, 13})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 16})
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual({4, 7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual({1, 7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4, 7, 8, 9, 10, 13})
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({7, 8, 10, 14, 15})
    Individual()
    Individual()
    Individual({4, 8, 10, 13, 15})
    Individual({1, 4, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({9})
    Individual()
    Individual({4, 7, 8, 10, 13, 17})
    Individual({8, 10, 4, 13})
    Individual({10})
    Individual()
    Individual({4, 7, 9, 10, 12, 13, 14, 15, 16})
    Individual({8, 10, 4, 13})
    Individual({4, 7, 8, 9, 12, 13, 14, 15, 16})
    Individual({10})
    Individual({2, 8, 10, 13})
    Individual({10})
    Individual({8, 10, 13})
    Individual()
    Individual({4, 8, 9, 10, 12, 13, 14, 15, 16})
    Individual()
    Individual({8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({4, 8, 10, 13, 14})
    Individual({4, 8, 10, 13, 14, 15})
    Individual({7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15, 16})
    Individual({4, 6, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual({8, 10, 4, 13})
    Individual({2, 5, 8, 10, 13, 18})
    Individual({10})
    Individual({7, 8, 10, 13, 15})
    Individual({4, 9, 10, 13, 14, 15})
    Individual()
    Individual({10})
    Individual({8, 10, 13})
    Individual({4, 7, 8, 10, 13})
    Individual()
    41 	93    	[  10.8         272.43239273]	[  12.13424905  220.99277855]	[ 0.  0.]                  	[  42.          637.31948207]
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13, 14, 15})
    Individual({4, 10, 13, 14})
    Individual()
    Individual({4, 5, 8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual({8})
    Individual({4, 7, 8, 10, 13})
    Individual({4, 8, 10, 13, 14})
    Individual({4, 7, 8, 9, 10, 13, 14, 15, 16})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 10, 13, 15})
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual({8, 10, 2})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 0, 10})
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual({8, 10, 13, 7})
    Individual({4, 8, 10, 13})
    Individual()
    Individual({4, 8, 10, 13, 14, 15})
    Individual({8, 10})
    Individual()
    Individual()
    Individual({8})
    Individual({8, 10, 13})
    Individual({4, 8, 9, 13, 14, 15})
    Individual({8, 10})
    Individual({8, 10, 13})
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual({3, 4, 6, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 4, 13})
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual({7, 8, 13, 15})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual({0, 4, 8, 10, 13, 15})
    Individual()
    Individual({8, 10, 13})
    Individual({7, 8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual({4})
    Individual()
    Individual()
    Individual({8})
    Individual({7, 8, 10, 13, 15})
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4, 8, 9, 10, 14, 15})
    Individual({8, 10, 4})
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({4, 8, 10, 12, 13, 14})
    Individual({4, 7, 8, 9, 10, 12, 13, 14, 16, 18})
    42 	91    	[  11.86        304.07117362]	[  12.03995017  211.20576324]	[ 0.  0.]                  	[  42.          637.31948207]
    Individual()
    Individual({8, 10, 13, 7})
    Individual({10})
    Individual({17})
    Individual({8, 10, 4, 13})
    Individual({10})
    Individual({0, 4, 8, 10, 13, 14, 15, 18})
    Individual()
    Individual()
    Individual({7, 8, 10, 13, 14, 15})
    Individual({4, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual({4, 7, 8, 10, 13, 14})
    Individual()
    Individual({4, 8, 10, 13, 15})
    Individual()
    Individual({4, 7, 8, 10, 13, 17})
    Individual()
    Individual({10})
    Individual({1})
    Individual()
    Individual({10})
    Individual({4, 8, 10, 13, 14})
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual({4, 7, 8, 9, 10, 12, 13, 14, 16, 18})
    Individual()
    Individual({6})
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 7})
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13, 14, 15})
    Individual({4, 8, 10, 11, 13})
    Individual({0, 4, 8, 10, 13, 14, 15})
    Individual({2, 8, 13, 18})
    Individual()
    Individual({10})
    Individual({8, 10, 13})
    Individual({2, 8, 10, 13, 16})
    Individual({8, 10, 13})
    Individual()
    Individual({4, 7, 8, 10, 13, 14})
    Individual()
    Individual({8, 10, 13, 14})
    Individual({0, 4, 8, 10, 13, 15})
    Individual({10})
    Individual({8, 10, 13})
    Individual({8, 10, 13, 15})
    Individual({4, 8, 9, 10, 14, 15})
    Individual()
    Individual({4, 8, 13, 14, 15})
    Individual({4, 6, 8, 10, 13, 14, 15})
    Individual({7, 8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual()
    Individual({7, 10, 13, 14})
    Individual({7, 8, 10, 13, 14})
    Individual()
    Individual({8, 10, 4, 13})
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual({0, 4, 7, 8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual({4, 7, 8, 10, 13})
    Individual()
    Individual({1, 2, 8, 10, 13})
    Individual()
    Individual({8, 10, 13})
    Individual({4, 8, 10, 13, 14})
    Individual({4, 8, 10, 13, 14, 15})
    Individual({4, 7, 8, 10})
    Individual({8, 10, 4, 13})
    Individual()
    Individual({3})
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    43 	93    	[  11.38       288.3292047]  	[  12.25706327  222.63153571]	[ 0.  0.]                  	[  42.          637.31948207]
    Individual({14})
    Individual()
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual({4, 8, 10, 13, 14, 15})
    Individual()
    Individual({8, 10, 13, 7})
    Individual()
    Individual({8, 10, 13})
    Individual({4, 7, 8, 10, 14})
    Individual({4, 6, 8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual({7, 8, 10, 15})
    Individual({8, 10, 4, 13})
    Individual({2, 4, 6, 8, 9, 10, 13, 14, 15})
    Individual({8, 10, 13})
    Individual({8, 10, 4, 13})
    Individual({7, 8, 10, 13})
    Individual({7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual()
    Individual({4, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17, 18})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({4, 7, 8, 9, 10, 13, 14, 15})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 10, 2})
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual({10})
    Individual()
    Individual({8, 10})
    Individual({8, 10, 13})
    Individual()
    Individual({4, 8, 10, 13, 14, 15, 18})
    Individual({10})
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 15})
    Individual({8, 10})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({7, 8, 10, 14})
    Individual()
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual({10})
    Individual({4, 8, 9, 10, 13, 14, 15})
    44 	87    	[  11.62      302.244937]    	[  12.38691245  217.55803025]	[ 0.  0.]                  	[  42.          637.31948207]
    Individual()
    Individual()
    Individual({7, 8, 10, 13})
    Individual({4, 8, 10, 13, 14})
    Individual({4, 8, 10, 13, 14, 15, 18})
    Individual()
    Individual({4, 8, 10, 12, 13, 14})
    Individual()
    Individual({8, 10, 4, 13})
    Individual({8, 10, 13, 7})
    Individual({4, 7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual()
    Individual({8, 10, 13})
    Individual({4, 8, 10, 13})
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual()
    Individual({10})
    Individual({10})
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual({10})
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13, 15})
    Individual()
    Individual({8, 10})
    Individual({8, 10, 13, 15})
    Individual({7, 8, 10, 14, 15})
    Individual()
    Individual({8, 10, 13, 15})
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 4, 13})
    Individual({8, 10, 4, 13})
    Individual({4, 8, 10, 13, 15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual({8, 10, 13, 7})
    Individual()
    Individual({8, 10, 13, 14, 15})
    Individual({8, 10, 4, 13})
    Individual({10})
    Individual({13})
    Individual({8, 10, 13})
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({4, 8, 10, 13, 14, 15})
    Individual({8, 10, 4, 13})
    Individual()
    Individual()
    Individual()
    Individual({4, 8, 10, 13, 15})
    Individual({8, 10, 4, 13})
    Individual()
    Individual({7, 8, 10, 13, 14, 15})
    Individual({8, 10, 13})
    Individual({7, 8, 10, 13, 15})
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual({7, 10, 13, 14})
    Individual()
    Individual()
    Individual({10})
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual()
    45 	87    	[  11.7         297.43893553]	[  12.64634335  223.48730523]	[ 0.  0.]                  	[  42.          637.31948207]
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual({8, 10})
    Individual({8, 10})
    Individual({2, 4, 7, 8, 10, 13})
    Individual({4, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18})
    Individual()
    Individual({4, 7, 8, 10, 13, 19})
    Individual()
    Individual({4, 8, 10, 13, 14, 15})
    Individual({4, 8, 10, 13, 14, 15, 18})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 4, 13})
    Individual({8, 10, 4, 13})
    Individual()
    Individual({8, 10})
    Individual({8, 10, 4, 13})
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual()
    Individual({7, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual({4, 7, 8, 10, 13})
    Individual({8, 10, 2, 13})
    Individual({8, 10, 4, 13})
    Individual({3, 4, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18})
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual()
    Individual({4, 7, 8, 10, 13})
    Individual({10})
    Individual({8})
    Individual({4, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18})
    Individual({8, 10})
    Individual()
    Individual({4, 8, 13, 15})
    Individual({4, 7, 8, 10})
    Individual({8, 10, 18, 13})
    Individual({8, 10, 13, 14})
    Individual()
    Individual({4, 7, 8, 10, 13})
    Individual({4, 7, 8, 9, 10, 13, 15})
    Individual()
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual({4, 8, 9, 13, 14, 15})
    Individual()
    Individual({8, 10, 13})
    Individual({4, 7, 8, 10, 13, 14, 15})
    Individual()
    Individual({4, 8, 10, 13, 15})
    Individual()
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({10})
    Individual({8, 10})
    Individual()
    Individual({8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({0, 4, 8, 10, 13})
    Individual()
    Individual({8, 10})
    Individual({4, 8, 13, 15})
    Individual({8, 10, 4, 13})
    Individual({4, 7, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual({4, 7, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual({8, 10, 13, 14, 15})
    Individual({8, 10})
    Individual({10})
    46 	91    	[  16.7         368.89821598]	[  14.88925787  230.18959314]	[ 0.  0.]                  	[  47.          707.73092646]
    Individual()
    Individual({13})
    Individual({7, 8, 10, 13, 14, 15})
    Individual()
    Individual({9})
    Individual({8, 10, 13})
    Individual({2, 6, 8, 9, 10, 13, 14, 15})
    Individual({4, 7, 8, 10, 12, 13, 14, 15, 16, 18})
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual({4, 8, 10, 13, 14, 15})
    Individual({8, 10, 4, 13})
    Individual({0, 4, 7, 8, 10, 13})
    Individual({8, 10, 4, 13})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual()
    Individual({8, 10, 4, 13})
    Individual({8, 10, 13, 14})
    Individual({4, 7, 8, 9, 10, 13, 15})
    Individual()
    Individual({8, 10})
    Individual({4, 7, 8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual({4, 7, 8, 10, 13, 14, 15})
    Individual({8, 10, 13})
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual({4, 8, 10, 13, 14, 15, 18})
    Individual({8, 10, 13, 14, 15})
    Individual({8, 10})
    Individual()
    Individual()
    Individual({4, 8, 10, 13, 14, 15, 18})
    Individual({10})
    Individual()
    Individual()
    Individual({4, 7, 8, 10, 13, 14, 16})
    Individual()
    Individual({9, 10})
    Individual()
    Individual()
    Individual({8, 10, 4, 13})
    Individual({3, 4, 6, 8, 9, 10, 13, 15})
    Individual({8, 10, 4, 13})
    Individual({3, 4, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 18})
    Individual({4, 7, 8, 10, 13})
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual({4, 8, 10, 13, 14, 15})
    Individual({4, 8, 10, 13, 14})
    Individual({8, 10, 4, 13})
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 2, 13})
    Individual({8, 10, 4, 13})
    Individual()
    Individual()
    Individual({8, 10, 13, 14})
    Individual({4, 8, 10, 13, 14})
    Individual({8, 10})
    Individual({4, 8, 10, 13, 14})
    Individual({8, 10, 4, 13})
    Individual()
    Individual({4, 8, 10, 13, 14, 15})
    Individual({1, 2, 4, 6, 8, 9, 10, 13, 14, 15})
    Individual({8, 10, 13, 14, 15})
    Individual({8, 10})
    Individual()
    Individual({8, 10})
    Individual({4, 8, 10, 13, 14})
    Individual({0})
    Individual()
    Individual({8, 10})
    Individual({4, 8, 10, 13, 14, 15})
    Individual()
    Individual({4, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18})
    47 	88    	[  15.02        344.98372128]	[  14.04776139  235.63811278]	[ 0.  0.]                  	[  47.          707.73092646]
    Individual({4, 8, 10, 13, 14, 15})
    Individual({8, 10, 13})
    Individual({4, 8, 10, 13, 14, 15})
    Individual({4, 8, 10, 13, 15})
    Individual({4, 8, 10, 13, 14, 15})
    Individual()
    Individual({4, 8, 10, 13, 14, 15, 18})
    Individual({7, 8, 10, 11, 13, 14, 15})
    Individual()
    Individual({4, 7, 8, 9, 10, 13, 14, 15})
    Individual({4, 7, 8, 13})
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual({8, 10, 13})
    Individual({7, 8, 10, 13, 14, 15})
    Individual()
    Individual({4, 8, 10, 15})
    Individual()
    Individual()
    Individual({0, 4, 8, 10, 13, 14, 18})
    Individual({8, 10})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4, 8, 10, 13, 15})
    Individual()
    Individual({7, 8, 10, 12, 13, 14, 15})
    Individual({3, 4, 7, 8, 9, 10, 12, 14, 15, 16, 18})
    Individual({4, 8, 10, 13, 14, 15})
    Individual({4, 8, 9, 10, 13, 14})
    Individual({0, 4, 8, 10, 13, 14, 15, 18})
    Individual({4, 8, 10, 13, 14, 15})
    Individual({4, 6, 8, 9, 10, 13, 14, 15})
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual({8, 10})
    Individual()
    Individual({8, 10, 13})
    Individual({4, 8, 10, 13, 15})
    Individual()
    Individual({8, 10})
    Individual()
    Individual({8, 10})
    Individual({8, 10})
    Individual({0, 4, 8, 13, 14, 15, 18})
    Individual()
    Individual({8, 10, 4, 13})
    Individual({4, 8, 10, 11, 13, 15})
    Individual()
    Individual({7, 8, 10, 13, 14, 15})
    Individual({4, 8, 10, 13, 15})
    Individual()
    Individual()
    Individual({4, 7, 8, 9, 10, 12, 13, 14, 15, 18})
    Individual({5})
    Individual()
    Individual({8, 10, 13, 14, 15})
    Individual({11})
    Individual({8, 10, 13})
    Individual({8, 10})
    Individual({8, 10, 13})
    Individual({8, 10})
    Individual({8, 10})
    Individual({4, 6, 8, 10, 13})
    Individual({4, 7, 8, 10, 13, 14, 15})
    Individual({14})
    Individual({2, 4, 8, 10, 13})
    Individual({7, 8, 10, 13, 14})
    Individual({8, 10, 4, 13})
    Individual()
    Individual()
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual({7, 8, 10, 13, 14, 15})
    Individual({0, 4, 8, 10, 13, 14, 15, 18})
    Individual({4, 7, 8, 10, 13, 14, 15})
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual({8, 2, 13})
    Individual({4, 7, 9, 10, 12, 13, 14, 15, 16, 18})
    Individual({4, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18})
    Individual()
    Individual()
    48 	86    	[  12.66        307.94602816]	[  13.67422393  237.85697412]	[ 0.  0.]                  	[  47.          707.73092646]
    Individual({4, 7, 8, 10, 13})
    Individual({4, 7, 8, 10, 13})
    Individual()
    Individual()
    Individual({10})
    Individual({4, 8, 10, 13, 14, 15})
    Individual({10})
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual({4, 8, 10, 13, 14, 15})
    Individual({8, 10, 2, 13})
    Individual()
    Individual({7})
    Individual({8, 10, 4, 13})
    Individual()
    Individual({4, 7, 8, 10, 13})
    Individual({8, 10, 13})
    Individual({8, 10})
    Individual({3, 4, 6, 8, 9, 10, 13, 15})
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual({8, 10})
    Individual({19})
    Individual()
    Individual({8, 10})
    Individual()
    Individual({1, 2, 4, 7, 8, 10, 13})
    Individual({8, 10, 4, 13})
    Individual({4, 7, 8, 10, 13})
    Individual({4, 8, 10, 13, 15})
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({4, 8, 10, 13, 14, 15, 18})
    Individual({0, 10})
    Individual({8, 10, 13})
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual({10})
    Individual({4, 8, 10, 14})
    Individual({8, 10, 13})
    Individual()
    Individual()
    Individual({10})
    Individual({10})
    Individual()
    Individual({8, 10})
    Individual({10})
    Individual({8, 10, 13})
    Individual({8, 10, 4, 13})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 4, 13})
    Individual({4, 6, 8, 10, 13})
    Individual({8, 10, 13, 14, 15})
    Individual({4, 7, 8, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual()
    Individual({3, 4, 6, 8, 9, 10, 13, 14})
    Individual({10})
    Individual({10})
    Individual({3, 4, 6, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual({4, 8, 10, 13})
    Individual({4, 7, 8, 9, 10, 13, 14, 15})
    Individual({5})
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual({4, 8, 10, 13})
    Individual({10})
    Individual({8, 10, 13, 14})
    Individual({4, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18})
    Individual()
    Individual({8, 10})
    Individual({10})
    Individual({4, 8, 10, 13, 14, 15})
    Individual({4, 6, 8, 9, 10, 13, 14, 15})
    Individual({8, 10})
    Individual({8, 10, 13})
    Individual({2, 4, 6, 8, 9, 10, 13, 14, 15, 17})
    Individual()
    Individual()
    Individual()
    Individual({4, 8, 10, 13, 14, 15})
    Individual({10})
    49 	90    	[  15.78        366.45278023]	[  14.21870599  222.95780236]	[ 0.  0.]                  	[  47.          707.73092646]
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual({8, 10})
    Individual({2, 4, 8, 10, 13})
    Individual({2, 4, 7, 8, 10, 13, 16})
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual({8, 10, 4, 13})
    Individual({4, 10, 13, 14, 15, 18})
    Individual({2, 4, 6, 8, 9, 10, 13, 14, 15})
    Individual({4, 7, 8, 10, 13})
    Individual({8, 10, 13})
    Individual({4, 6, 8, 9, 10, 13, 14, 15})
    Individual()
    Individual()
    Individual()
    Individual({2, 4, 8, 10, 13, 15})
    Individual()
    Individual({2, 4, 8, 10, 13, 14, 15, 18})
    Individual()
    Individual({8, 10, 13, 15})
    Individual({8, 10, 4, 13})
    Individual({8, 10, 4, 13})
    Individual()
    Individual()
    Individual()
    Individual({8, 10, 13})
    Individual({8, 10, 13})
    Individual()
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual({8, 10, 13})
    Individual({10})
    Individual({8, 13})
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual({4, 8, 10, 13, 15})
    Individual({4, 8, 10, 13, 15})
    Individual({8, 10, 13})
    Individual()
    Individual({4, 7, 8, 10, 13, 14, 15})
    Individual()
    Individual({8, 10})
    Individual({2, 4, 7, 8, 10, 13})
    Individual({4, 7, 8, 9, 10, 12, 13, 14, 15, 18})
    Individual({8, 10})
    Individual({4, 8, 10, 13, 14})
    Individual({8, 10, 4, 13})
    Individual({8, 10, 13})
    Individual({8, 10, 4, 13})
    Individual({8, 10, 4, 13})
    Individual({8, 10, 4, 13})
    Individual({8, 10, 4, 13})
    Individual({2, 8, 10, 13})
    Individual({4, 8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual({2, 4, 7, 8, 10, 13, 15})
    Individual({4, 8, 9, 10, 13, 14, 15})
    Individual({8, 10, 4, 13})
    Individual({4, 8, 10, 13, 14})
    Individual()
    Individual({0, 2, 8, 10, 13})
    Individual({4, 7, 8, 10, 13})
    Individual({4, 8, 10, 13, 15})
    Individual({8, 10})
    Individual({8, 10, 4, 13})
    Individual({4, 7, 8, 9, 10, 12, 13, 15, 16, 18})
    Individual({8, 10})
    Individual()
    Individual({7, 8, 9, 10, 13, 14, 15})
    Individual({8, 10, 4, 13})
    Individual({10})
    Individual({8, 10, 4, 13})
    Individual({2, 8, 10, 12, 13})
    Individual()
    Individual()
    Individual({4, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18})
    Individual({8, 10, 4, 13})
    Individual()
    Individual({4, 8, 10, 13, 14})
    Individual({4, 8, 10, 13, 14, 15, 18})
    Individual({2, 4, 6, 8, 9, 10, 14, 15})
    Individual({8, 10, 4, 13})
    Individual({4, 8, 10, 13, 14})
    Individual({8, 10, 13})
    Individual({4, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 18})
    Individual()
    Individual()
    50 	87    	[  16.92        386.82265016]	[  14.89005037  220.57801282]	[ 0.  0.]                  	[  47.          707.73092646]


GA v2 experiment
================

.. code:: python

    import random
    
    import numpy
    
    from deap import algorithms
    from deap import base
    from deap import creator
    from deap import tools

.. code:: python

    IND_INIT_SIZE = 5
    MAX_ITEM = 50
    MAX_WEIGHT = 50
    NBR_ITEMS = 20
    
    # To assure reproductibility, the RNG seed is set prior to the items
    # dict initialization. It is also seeded in main().
    random.seed(64)
    
    # Create the item dictionary: item name is an integer, and value is 
    # a (weight, value) 2-uple.
    items = {}
    # Create random items and store them in the items' dictionary.
    for i in range(NBR_ITEMS):
        items[i] = (random.randint(1, 10), random.uniform(0, 100))
        print(items[i])
    
    creator.create("Fitness", base.Fitness, weights=(-1.0, 1.0))
    creator.create("Individual", set, fitness=creator.Fitness)



.. parsed-literal::

    (8, 12.48509930835593)
    (10, 40.396096956847295)
    (1, 20.351113369770758)
    (5, 70.41144439033413)
    (4, 69.86574666158654)
    (3, 7.981206373372229)
    (7, 44.003630577221465)
    (1, 8.366591038506865)
    (1, 92.7646786450061)
    (3, 10.541265149933377)
    (1, 95.00359117156347)
    (5, 21.924990817330915)
    (4, 8.882167797500228)
    (1, 76.96797658382238)
    (7, 89.16384444665715)
    (5, 74.23923323846317)
    (9, 42.688343941286256)
    (9, 30.701913496381728)
    (6, 68.83604339265639)
    (9, 17.89634902035938)


.. code:: python

    import random
    from deap import tools
    
    IND_SIZE = 10
    
    toolbox = base.Toolbox()
    toolbox.register("attribute", random.random)
    toolbox.register("individual", tools.initRepeat, creator.Individual,
                     toolbox.attribute, n=IND_SIZE)
    toolbox.register("population", tools.initRepeat, list, toolbox.individual)
    
    pop = toolbox.population(n=50)
    print(len(pop[2]))


.. parsed-literal::

    10


doc link http://deap.gel.ulaval.ca/doc/dev/api/tools.html
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

    tools.initRepeat(list, random.random, 2) 




.. parsed-literal::

    [0.005308251575226408, 0.7484342341207556]



.. code:: python

    def initPopWithFixedSet():
        return [2,3,4]
    tools.initRepeat(list, initPopWithFixedSet, 10) 




.. parsed-literal::

    [[2, 3, 4],
     [2, 3, 4],
     [2, 3, 4],
     [2, 3, 4],
     [2, 3, 4],
     [2, 3, 4],
     [2, 3, 4],
     [2, 3, 4],
     [2, 3, 4],
     [2, 3, 4]]



.. code:: python

    
        
    def initPopWithFixedSet():
        return [2,3,4]
    tools.initRepeat(list, initPopWithFixedSet, 10) 

.. code:: python

    class MyClass:
        """A simple example class"""
        i = 12345
    
        def f(self):
            return 'hello world'

.. code:: python

    myClassList = [MyClass(), MyClass()]

.. code:: python

    names[0].i




.. parsed-literal::

    12345



.. code:: python

    def initListOfClass():
        return names
    
    tools.initRepeat(list, initListOfClass, 10) 




.. parsed-literal::

    [[<__main__.MyClass at 0x10992c208>, <__main__.MyClass at 0x10992c6a0>],
     [<__main__.MyClass at 0x10992c208>, <__main__.MyClass at 0x10992c6a0>],
     [<__main__.MyClass at 0x10992c208>, <__main__.MyClass at 0x10992c6a0>],
     [<__main__.MyClass at 0x10992c208>, <__main__.MyClass at 0x10992c6a0>],
     [<__main__.MyClass at 0x10992c208>, <__main__.MyClass at 0x10992c6a0>],
     [<__main__.MyClass at 0x10992c208>, <__main__.MyClass at 0x10992c6a0>],
     [<__main__.MyClass at 0x10992c208>, <__main__.MyClass at 0x10992c6a0>],
     [<__main__.MyClass at 0x10992c208>, <__main__.MyClass at 0x10992c6a0>],
     [<__main__.MyClass at 0x10992c208>, <__main__.MyClass at 0x10992c6a0>],
     [<__main__.MyClass at 0x10992c208>, <__main__.MyClass at 0x10992c6a0>]]



