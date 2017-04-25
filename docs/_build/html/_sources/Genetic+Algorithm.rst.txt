
CAKE - Genetic Algorithm POC
============================

Simple Test case we have tested for memeory constraints.

.. raw:: html

   <pre>
   machine = {"m1" : {"ram" : 4, "HDD" : 500, "cores" : 3 },
        "m2" : {"ram" : 12, "HDD" : 1000, "cores" : 5 } 
       }
   jobs = {"j1" : {"ram" : 200, "HDD" : 100, "cores" : 3},
        "j2" : {"ram" : 500, "HDD" : 50, "cores" : 5}, 
        "j3" : {"ram" : 100, "HDD" : 30, "cores" : 3}, 
        "j4" : {"ram" : 1024, "HDD" : 1, "cores" : 5}
       }

   schedule_1 = {"m1" : ["j1", "j3"],
               "m2" : ["j2", "j4"]
              }
   schedule_2 = {"m1" : ["j1", "j2"],
               "m2
   </pre>


.. code:: python

    import random
    
    from deap import base
    from deap import creator
    from deap import tools
    import matplotlib.pyplot as plt
    %matplotlib inline 

.. code:: python

    m = {"m1" : {"ram" : 4, "HDD" : 500, "cores" : 3 },
         "m2" : {"ram" : 12, "HDD" : 1000, "cores" : 5 } 
        }
    j = {"j1" : {"ram" : 200, "HDD" : 100, "cores" : 3},
         "j2" : {"ram" : 500, "HDD" : 50, "cores" : 5}, 
         "j3" : {"ram" : 100, "HDD" : 30, "cores" : 3}, 
         "j4" : {"ram" : 1024, "HDD" : 1, "cores" : 5}
        }
    
    schedule_1 = {"m1" : ["j1", "j3"],
                "m2" : ["j2", "j4"]
               }
    schedule_2 = {"m1" : ["j1", "j2"],
                "m2" : ["j3", "j4"]
               }

.. code:: python

    def evaluate_schedule(schedule, m , j):
        l = len(schedule)
        fit = 0
        for key, values in schedule.items():
            max_core = 0
            t_r = 0
            t_h = 0
            m_c = m[key]
            for item in values:
                j_c = j[item]
                if j_c["cores"] > max_core:
                    max_core = j_c["cores"]
                t_r += j_c["ram"]
                t_h += j_c["HDD"]
            if (max_core <= m_c["cores"]) and (t_r <= m_c["ram"] * 1024) and (t_h <= m_c["HDD"]):
                fit += 1
        score = (fit/l)*100
        #print("Score : %f"%score)
        
        #check if this is a complete schedule
        allJobs = list(j.keys())
        allMachines = list(m.keys())
        scheduled_jobs = list()
        for machine in allMachines:
            jobs_on_machine = schedule[machine]
            for job in jobs_on_machine:
                if job not in scheduled_jobs:
                    scheduled_jobs.append(job)
        """
        for job in allJobs:
            valid = False
            for machine in allMachines:
                if job in m[machine]:
                    valid = True
                    break;
                else:
                    continue
            if valid == False:
                break
        """
        valid = False
        if len(allJobs) == len(scheduled_jobs):
            valid = True
        return (score, valid)
        #return score

.. code:: python

    import math as ma
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
                slice_point = ma.ceil(len(ith_machine_jobs_1)/2)
            else:
                slice_point = ma.ceil(len(ith_machine_jobs_2)/2)
                
            new_1 = [item for item in ith_machine_jobs_1[:slice_point]]
            for item in ith_machine_jobs_2[slice_point:]:
                new_1.append(item)
            n_s_1.update({"m"+str(i+1) : new_1 })
            
            new_2 = [item for item in ith_machine_jobs_2[:slice_point]]
            for item in ith_machine_jobs_1[slice_point:]:
                new_2.append(item)
            
            n_s_2.update({"m"+str(i+1) : new_2 })
        """
        print("old schedule_1 : ",schedule_1)
        print("old schedule_2 : ",schedule_2)
        print()
        print("new schedule_1 : ",n_s_1)
        print("new schedule_2 : ",n_s_2)
        """
        return n_s_1, n_s_2

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
        slice_point = ma.ceil(len(s_1)/2)
        for i in range(l):
            if i < slice_point:
                n_s_1.update({"m"+str(i+1) : s_1["m"+str(i+1)]})
                n_s_2.update({"m"+str(i+1) : s_2["m"+str(i+1)]})
            else:
                n_s_1.update({"m"+str(i+1) : s_2["m"+str(i+1)]})
                n_s_2.update({"m"+str(i+1) : s_1["m"+str(i+1)]})
        """
        print("old schedule_1 : ",s_1)
        print("old schedule_2 : ",s_2)
        print()
        print("new schedule_1 : ",n_s_1)
        print("new schedule_2 : ",n_s_2)
        """
        return n_s_1, n_s_2

.. code:: python

    import random
    def crossover(s_1, s_2):
        choice = random.randint(0,1)
        #print(choice)
        if choice == 0:
            return crossover_1(s_1, s_2)
        else:
            return crossover_2(s_1, s_2)

.. code:: python

    #generate the initial population
    def generateInitalPopulation_randomly(machine_details, job_details, numberOfSchedules):
        randomSchedules = list()
        available_machines = list(machine_details.keys())
        submitted_jobs = list(job_details.keys())
        
        jobs_per_machine = ma.ceil(len(submitted_jobs) / len(available_machines))
        
        #print(submitted_jobs)
        #print(available_machines)
        
        for i in range(numberOfSchedules):
            submitted_jobs = list(job_details.keys())
            schedule = {}
            for machine in available_machines:
                schedule.update({ machine : list()})
                for j in range(jobs_per_machine):
                    if(len(submitted_jobs) <= 0):
                        break
                    index = random.randint(0,len(submitted_jobs)-1)
                    job = submitted_jobs.pop(index)#removing scheduled item
                    schedule[machine].append(job)
            randomSchedules.append(schedule)
        return randomSchedules

.. code:: python

    #generates next population using crossover
    def generateNextPopulationUsingCrossOver(df, m, j, n):
        schedule = df['schedule']
        fitness_score = df['fitness score']
        new_Schedules = list()
        while(len(new_Schedules) < n):
            i1 = random.randint(0,n-1)
            i2 = random.randint(0,n-1)
            crossed_over_schedules = crossover(schedule[i1], schedule[i2])
            for item in crossed_over_schedules:
                if not len(new_Schedules) >= n:
                    new_Schedules.append(item)
        return new_Schedules

.. code:: python

    #print(random.random())
    def mutate(schedules, j, prob):
        print(schedules)
        allJobs = list(j.keys())
        for i in range(len(schedules)):
            #print(i)
            for key in schedules[i].keys():#length of each schedule i.e, number of machines
                #print(schedules[i])
                for k in range(len(schedules[i][key])):#number of jobs on each machine
                    #print("K : ",k)
                    if random.random() < prob:
                        possible_values = list(allJobs)
                        possible_values.remove(schedules[i][key][k])
                        index = random.randint(0,len(possible_values) - 1)
                        schedules[i][key][k] = possible_values[index]
        return schedules
    #mutate([schedule_1,schedule_2], j, 0.5)

.. code:: python

    import pandas as pd
    import sys
    def demoGA():
        #random.seed(64)
        CXPB, MUTPB, NGEN, POP_SIZE = 0.5, 0.2, 500, 2
        initialRandomSchedules = generateInitalPopulation_randomly(m, j, POP_SIZE)
        #print(initialRandomSchedules)
        fitnesses = list()
        for schedule in initialRandomSchedules:
            fit_score = evaluate_schedule(schedule, m , j)
            #fitnesses.append(fit_score)
            if fit_score[0] == 100 and (fit_score[1] == True):
                    print("Solution found : ", schedule)
                    return
            else:
                fitnesses.append(fit_score)
     
        #print(fitnesses)
        #for schedule, fitness in zip(intialRandomScehules, fitnesses):
        print("Initial Schedules : ")
        df = pd.DataFrame(data={'schedule' : initialRandomSchedules, 'fitness score' : fitnesses})
        df = df[['schedule', 'fitness score']]
        df = df.sort_values(axis=0,by='fitness score',ascending=False)
        print(df)
        
        for i in range(NGEN):
            print("\nGeneration : ",i)
            new_Schedule = generateNextPopulationUsingCrossOver(df, m, j, POP_SIZE)
            new_Schedule = mutate(new_Schedule, j, MUTPB)
            fitnesses = list()
            for schedule in new_Schedule:
                fit_score = evaluate_schedule(schedule, m , j)
                #print(fit_score)
                if fit_score[0] == 100 and (fit_score[1] == True):
                    print("Solution found : ", schedule)
                    return
                else:
                    fitnesses.append(fit_score)
            
            df = pd.DataFrame(data={'schedule' : new_Schedule, 'fitness score' : fitnesses})
            df = df[['schedule', 'fitness score']]
            df = df.sort_values(axis=0,by='fitness score',ascending=False)  
            print(df)
        
    if __name__ == "__main__":
        demoGA()


.. parsed-literal::

    Initial Schedules : 
                                       schedule fitness score
    0  {'m1': ['j3', 'j2'], 'm2': ['j1', 'j4']}  (50.0, True)
    1  {'m1': ['j3', 'j2'], 'm2': ['j1', 'j4']}  (50.0, True)
    
    Generation :  0
    [{'m1': ['j3', 'j2'], 'm2': ['j1', 'j4']}, {'m1': ['j3', 'j2'], 'm2': ['j1', 'j4']}]
                                       schedule fitness score
    0  {'m1': ['j3', 'j2'], 'm2': ['j1', 'j4']}  (50.0, True)
    1  {'m1': ['j3', 'j2'], 'm2': ['j1', 'j4']}  (50.0, True)
    
    Generation :  1
    [{'m1': ['j3', 'j2'], 'm2': ['j1', 'j4']}, {'m1': ['j3', 'j2'], 'm2': ['j1', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}  (50.0, False)
    
    Generation :  2
    [{'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j2'], 'm2': ['j1', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}  (50.0, False)
    
    Generation :  3
    [{'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j2'], 'm2': ['j1', 'j1']}  (50.0, False)
    
    Generation :  4
    [{'m1': ['j4', 'j2'], 'm2': ['j1', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j3', 'j4'], 'm2': ['j1', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j3', 'j1']}  (50.0, False)
    
    Generation :  5
    [{'m1': ['j4', 'j4'], 'm2': ['j3', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j2', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j2', 'j1']}  (50.0, False)
    
    Generation :  6
    [{'m1': ['j4', 'j4'], 'm2': ['j2', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j2', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  7
    [{'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  8
    [{'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  9
    [{'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  10
    [{'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  11
    [{'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  12
    [{'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j2', 'j1']}  (50.0, False)
    
    Generation :  13
    [{'m1': ['j4', 'j4'], 'm2': ['j2', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j2', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j2'], 'm2': ['j2', 'j1']}  (50.0, False)
    
    Generation :  14
    [{'m1': ['j4', 'j2'], 'm2': ['j2', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j2', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j2'], 'm2': ['j4', 'j1']}  (50.0, False)
    1  {'m1': ['j3', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  15
    [{'m1': ['j3', 'j4'], 'm2': ['j4', 'j1']}, {'m1': ['j4', 'j2'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    1  {'m1': ['j3', 'j2'], 'm2': ['j4', 'j1']}   (50.0, True)
    0  {'m1': ['j3', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  16
    [{'m1': ['j3', 'j4'], 'm2': ['j4', 'j1']}, {'m1': ['j3', 'j2'], 'm2': ['j4', 'j1']}]
                                       schedule   fitness score
    1  {'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}  (100.0, False)
    0  {'m1': ['j3', 'j4'], 'm2': ['j4', 'j1']}   (50.0, False)
    
    Generation :  17
    [{'m1': ['j3', 'j4'], 'm2': ['j4', 'j1']}, {'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}  (100.0, False)
    1  {'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}  (100.0, False)
    
    Generation :  18
    [{'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}, {'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}  (100.0, False)
    1  {'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}  (100.0, False)
    
    Generation :  19
    [{'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}, {'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j1'], 'm2': ['j4', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j1'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  20
    [{'m1': ['j4', 'j1'], 'm2': ['j4', 'j4']}, {'m1': ['j4', 'j1'], 'm2': ['j4', 'j4']}]
                                       schedule   fitness score
    1  {'m1': ['j3', 'j1'], 'm2': ['j4', 'j4']}  (100.0, False)
    0  {'m1': ['j4', 'j1'], 'm2': ['j3', 'j4']}   (50.0, False)
    
    Generation :  21
    [{'m1': ['j4', 'j1'], 'm2': ['j3', 'j4']}, {'m1': ['j4', 'j1'], 'm2': ['j3', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j1'], 'm2': ['j3', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j1'], 'm2': ['j3', 'j4']}  (50.0, False)
    
    Generation :  22
    [{'m1': ['j4', 'j1'], 'm2': ['j3', 'j4']}, {'m1': ['j4', 'j1'], 'm2': ['j3', 'j4']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j3', 'j4']}  (100.0, False)
    1  {'m1': ['j1', 'j3'], 'm2': ['j3', 'j4']}  (100.0, False)
    
    Generation :  23
    [{'m1': ['j1', 'j3'], 'm2': ['j3', 'j4']}, {'m1': ['j1', 'j3'], 'm2': ['j3', 'j4']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j3'], 'm2': ['j3', 'j4']}  (100.0, False)
    0  {'m1': ['j1', 'j4'], 'm2': ['j3', 'j1']}   (50.0, False)
    
    Generation :  24
    [{'m1': ['j1', 'j3'], 'm2': ['j3', 'j1']}, {'m1': ['j1', 'j4'], 'm2': ['j3', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j1', 'j4'], 'm2': ['j2', 'j4']}  (50.0, False)
    
    Generation :  25
    [{'m1': ['j1', 'j4'], 'm2': ['j2', 'j4']}, {'m1': ['j1', 'j4'], 'm2': ['j2', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j3', 'j4'], 'm2': ['j3', 'j4']}  (50.0, False)
    1  {'m1': ['j3', 'j4'], 'm2': ['j3', 'j4']}  (50.0, False)
    
    Generation :  26
    [{'m1': ['j3', 'j4'], 'm2': ['j3', 'j4']}, {'m1': ['j3', 'j4'], 'm2': ['j3', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j3', 'j4'], 'm2': ['j3', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j3', 'j4']}  (50.0, False)
    
    Generation :  27
    [{'m1': ['j3', 'j4'], 'm2': ['j3', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j3', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j3', 'j4'], 'm2': ['j3', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j3', 'j4']}  (50.0, False)
    
    Generation :  28
    [{'m1': ['j3', 'j4'], 'm2': ['j3', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j3', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j3', 'j4']}  (50.0, False)
    
    Generation :  29
    [{'m1': ['j1', 'j4'], 'm2': ['j1', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j3', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j4'], 'm2': ['j1', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j4']}  (50.0, False)
    
    Generation :  30
    [{'m1': ['j4', 'j3'], 'm2': ['j1', 'j1']}, {'m1': ['j1', 'j4'], 'm2': ['j3', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j1', 'j4'], 'm2': ['j3', 'j4']}  (50.0, False)
    
    Generation :  31
    [{'m1': ['j1', 'j3'], 'm2': ['j3', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j3', 'j4']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j3', 'j1']}  (100.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j3', 'j4']}   (50.0, False)
    
    Generation :  32
    [{'m1': ['j1', 'j3'], 'm2': ['j3', 'j1']}, {'m1': ['j1', 'j3'], 'm2': ['j3', 'j1']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j3'], 'm2': ['j2', 'j1']}  (100.0, False)
    0  {'m1': ['j1', 'j4'], 'm2': ['j3', 'j1']}   (50.0, False)
    
    Generation :  33
    [{'m1': ['j1', 'j4'], 'm2': ['j2', 'j1']}, {'m1': ['j1', 'j3'], 'm2': ['j3', 'j1']}]
                                       schedule   fitness score
    1  {'m1': ['j3', 'j3'], 'm2': ['j3', 'j1']}  (100.0, False)
    0  {'m1': ['j1', 'j2'], 'm2': ['j2', 'j1']}   (50.0, False)
    
    Generation :  34
    [{'m1': ['j1', 'j2'], 'm2': ['j2', 'j1']}, {'m1': ['j1', 'j2'], 'm2': ['j2', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j2', 'j2']}  (100.0, False)
    1  {'m1': ['j1', 'j3'], 'm2': ['j2', 'j2']}  (100.0, False)
    
    Generation :  35
    [{'m1': ['j1', 'j3'], 'm2': ['j2', 'j2']}, {'m1': ['j1', 'j3'], 'm2': ['j2', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j2'], 'm2': ['j1', 'j2']}  (50.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j1', 'j2']}  (50.0, False)
    
    Generation :  36
    [{'m1': ['j1', 'j2'], 'm2': ['j1', 'j2']}, {'m1': ['j1', 'j2'], 'm2': ['j1', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j2'], 'm2': ['j1', 'j3']}  (50.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j1', 'j2']}  (50.0, False)
    
    Generation :  37
    [{'m1': ['j1', 'j2'], 'm2': ['j1', 'j3']}, {'m1': ['j1', 'j2'], 'm2': ['j1', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}  (50.0, False)
    
    Generation :  38
    [{'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}, {'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j1', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}  (50.0, False)
    
    Generation :  39
    [{'m1': ['j2', 'j3'], 'm2': ['j1', 'j2']}, {'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j1', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}  (50.0, False)
    
    Generation :  40
    [{'m1': ['j2', 'j3'], 'm2': ['j1', 'j2']}, {'m1': ['j2', 'j3'], 'm2': ['j1', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j1', 'j2']}  (50.0, False)
    
    Generation :  41
    [{'m1': ['j2', 'j3'], 'm2': ['j3', 'j2']}, {'m1': ['j2', 'j3'], 'm2': ['j1', 'j1']}]
                                       schedule  fitness score
    1  {'m1': ['j2', 'j3'], 'm2': ['j4', 'j1']}   (50.0, True)
    0  {'m1': ['j2', 'j4'], 'm2': ['j3', 'j2']}  (50.0, False)
    
    Generation :  42
    [{'m1': ['j2', 'j3'], 'm2': ['j4', 'j1']}, {'m1': ['j2', 'j3'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j2'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j3', 'j1']}  (50.0, False)
    
    Generation :  43
    [{'m1': ['j2', 'j2'], 'm2': ['j3', 'j1']}, {'m1': ['j2', 'j2'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j2'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j3', 'j1']}  (50.0, False)
    
    Generation :  44
    [{'m1': ['j1', 'j2'], 'm2': ['j3', 'j1']}, {'m1': ['j1', 'j2'], 'm2': ['j3', 'j1']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j1'], 'm2': ['j3', 'j1']}  (100.0, False)
    0  {'m1': ['j3', 'j2'], 'm2': ['j3', 'j1']}   (50.0, False)
    
    Generation :  45
    [{'m1': ['j1', 'j1'], 'm2': ['j3', 'j1']}, {'m1': ['j1', 'j1'], 'm2': ['j3', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j3', 'j1']}  (100.0, False)
    1  {'m1': ['j1', 'j1'], 'm2': ['j3', 'j1']}  (100.0, False)
    
    Generation :  46
    [{'m1': ['j1', 'j1'], 'm2': ['j3', 'j1']}, {'m1': ['j1', 'j1'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j1'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j1'], 'm2': ['j3', 'j1']}  (50.0, False)
    
    Generation :  47
    [{'m1': ['j4', 'j1'], 'm2': ['j3', 'j1']}, {'m1': ['j4', 'j1'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j1'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j1'], 'm2': ['j3', 'j1']}  (50.0, False)
    
    Generation :  48
    [{'m1': ['j2', 'j1'], 'm2': ['j3', 'j1']}, {'m1': ['j2', 'j1'], 'm2': ['j3', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j3', 'j1']}  (100.0, False)
    1  {'m1': ['j1', 'j1'], 'm2': ['j3', 'j1']}  (100.0, False)
    
    Generation :  49
    [{'m1': ['j1', 'j1'], 'm2': ['j3', 'j1']}, {'m1': ['j1', 'j1'], 'm2': ['j3', 'j1']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j1'], 'm2': ['j3', 'j1']}  (100.0, False)
    0  {'m1': ['j2', 'j1'], 'm2': ['j3', 'j1']}   (50.0, False)
    
    Generation :  50
    [{'m1': ['j2', 'j1'], 'm2': ['j3', 'j1']}, {'m1': ['j2', 'j1'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j1'], 'm2': ['j3', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j1'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  51
    [{'m1': ['j2', 'j1'], 'm2': ['j4', 'j1']}, {'m1': ['j2', 'j1'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j1'], 'm2': ['j4', 'j4']}  (50.0, False)
    1  {'m1': ['j2', 'j1'], 'm2': ['j4', 'j4']}  (50.0, False)
    
    Generation :  52
    [{'m1': ['j2', 'j1'], 'm2': ['j4', 'j4']}, {'m1': ['j2', 'j1'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j4']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j4']}  (50.0, False)
    
    Generation :  53
    [{'m1': ['j2', 'j2'], 'm2': ['j2', 'j4']}, {'m1': ['j2', 'j2'], 'm2': ['j2', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j1'], 'm2': ['j2', 'j4']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j4']}  (50.0, False)
    
    Generation :  54
    [{'m1': ['j2', 'j1'], 'm2': ['j2', 'j4']}, {'m1': ['j2', 'j1'], 'm2': ['j2', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j1'], 'm2': ['j2', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j1'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  55
    [{'m1': ['j2', 'j1'], 'm2': ['j2', 'j3']}, {'m1': ['j2', 'j1'], 'm2': ['j2', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j1'], 'm2': ['j2', 'j3']}  (100.0, False)
    1  {'m1': ['j2', 'j4'], 'm2': ['j1', 'j3']}    (50.0, True)
    
    Generation :  56
    [{'m1': ['j2', 'j4'], 'm2': ['j1', 'j3']}, {'m1': ['j2', 'j4'], 'm2': ['j1', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}  (50.0, False)
    
    Generation :  57
    [{'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}, {'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j3']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j3']}  (50.0, False)
    
    Generation :  58
    [{'m1': ['j4', 'j3'], 'm2': ['j1', 'j3']}, {'m1': ['j4', 'j3'], 'm2': ['j1', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}  (50.0, False)
    
    Generation :  59
    [{'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}, {'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  60
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}  (50.0, False)
    
    Generation :  61
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j3', 'j2'], 'm2': ['j2', 'j4']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}  (50.0, False)
    
    Generation :  62
    [{'m1': ['j3', 'j2'], 'm2': ['j2', 'j4']}, {'m1': ['j3', 'j2'], 'm2': ['j2', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j3', 'j2'], 'm2': ['j1', 'j3']}  (50.0, False)
    1  {'m1': ['j3', 'j2'], 'm2': ['j1', 'j3']}  (50.0, False)
    
    Generation :  63
    [{'m1': ['j3', 'j2'], 'm2': ['j1', 'j3']}, {'m1': ['j3', 'j2'], 'm2': ['j1', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j3', 'j2'], 'm2': ['j3', 'j3']}  (50.0, False)
    1  {'m1': ['j3', 'j2'], 'm2': ['j1', 'j3']}  (50.0, False)
    
    Generation :  64
    [{'m1': ['j3', 'j2'], 'm2': ['j1', 'j3']}, {'m1': ['j3', 'j2'], 'm2': ['j1', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j3', 'j2'], 'm2': ['j1', 'j3']}  (50.0, False)
    1  {'m1': ['j3', 'j2'], 'm2': ['j1', 'j3']}  (50.0, False)
    
    Generation :  65
    [{'m1': ['j3', 'j2'], 'm2': ['j1', 'j3']}, {'m1': ['j3', 'j2'], 'm2': ['j1', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j3', 'j2'], 'm2': ['j1', 'j2']}  (50.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j1', 'j3']}  (50.0, False)
    
    Generation :  66
    [{'m1': ['j3', 'j2'], 'm2': ['j1', 'j2']}, {'m1': ['j3', 'j2'], 'm2': ['j1', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j2'], 'm2': ['j1', 'j2']}  (50.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j1', 'j2']}  (50.0, False)
    
    Generation :  67
    [{'m1': ['j1', 'j2'], 'm2': ['j1', 'j2']}, {'m1': ['j1', 'j2'], 'm2': ['j1', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j2']}   (50.0, True)
    1  {'m1': ['j1', 'j2'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  68
    [{'m1': ['j1', 'j3'], 'm2': ['j1', 'j2']}, {'m1': ['j4', 'j2'], 'm2': ['j1', 'j4']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j1', 'j2']}  (100.0, False)
    1  {'m1': ['j4', 'j2'], 'm2': ['j3', 'j4']}   (50.0, False)
    
    Generation :  69
    [{'m1': ['j1', 'j3'], 'm2': ['j1', 'j2']}, {'m1': ['j1', 'j3'], 'm2': ['j1', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j4', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j4', 'j2']}  (50.0, False)
    
    Generation :  70
    [{'m1': ['j2', 'j3'], 'm2': ['j4', 'j2']}, {'m1': ['j2', 'j3'], 'm2': ['j4', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  71
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  72
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}  (50.0, False)
    
    Generation :  73
    [{'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}, {'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}  (50.0, False)
    
    Generation :  74
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}, {'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}  (50.0, False)
    
    Generation :  75
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  76
    [{'m1': ['j2', 'j2'], 'm2': ['j2', 'j2']}, {'m1': ['j2', 'j2'], 'm2': ['j2', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j2']}  (50.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  77
    [{'m1': ['j2', 'j2'], 'm2': ['j2', 'j2']}, {'m1': ['j2', 'j2'], 'm2': ['j2', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  78
    [{'m1': ['j2', 'j2'], 'm2': ['j2', 'j1']}, {'m1': ['j2', 'j2'], 'm2': ['j2', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}  (50.0, False)
    
    Generation :  79
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}, {'m1': ['j2', 'j2'], 'm2': ['j2', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j1'], 'm2': ['j2', 'j1']}  (50.0, False)
    1  {'m1': ['j3', 'j2'], 'm2': ['j2', 'j1']}  (50.0, False)
    
    Generation :  80
    [{'m1': ['j3', 'j2'], 'm2': ['j2', 'j1']}, {'m1': ['j3', 'j2'], 'm2': ['j2', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j3', 'j2'], 'm2': ['j2', 'j2']}  (50.0, False)
    1  {'m1': ['j3', 'j2'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  81
    [{'m1': ['j3', 'j2'], 'm2': ['j2', 'j2']}, {'m1': ['j3', 'j2'], 'm2': ['j2', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j3', 'j2'], 'm2': ['j2', 'j2']}  (50.0, False)
    1  {'m1': ['j3', 'j2'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  82
    [{'m1': ['j3', 'j2'], 'm2': ['j2', 'j2']}, {'m1': ['j3', 'j2'], 'm2': ['j2', 'j2']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j3'], 'm2': ['j2', 'j2']}  (100.0, False)
    0  {'m1': ['j3', 'j2'], 'm2': ['j3', 'j2']}   (50.0, False)
    
    Generation :  83
    [{'m1': ['j1', 'j3'], 'm2': ['j2', 'j2']}, {'m1': ['j1', 'j3'], 'm2': ['j2', 'j2']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j2', 'j2']}  (100.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j2', 'j2']}   (50.0, False)
    
    Generation :  84
    [{'m1': ['j1', 'j3'], 'm2': ['j2', 'j2']}, {'m1': ['j1', 'j3'], 'm2': ['j2', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  85
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j3'], 'm2': ['j2', 'j3']}  (100.0, False)
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}   (50.0, False)
    
    Generation :  86
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}, {'m1': ['j1', 'j3'], 'm2': ['j2', 'j1']}]
                                       schedule   fitness score
    1  {'m1': ['j3', 'j3'], 'm2': ['j2', 'j4']}  (100.0, False)
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}   (50.0, False)
    
    Generation :  87
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}, {'m1': ['j3', 'j3'], 'm2': ['j2', 'j3']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j3'], 'm2': ['j2', 'j3']}  (100.0, False)
    0  {'m1': ['j2', 'j3'], 'm2': ['j1', 'j4']}    (50.0, True)
    
    Generation :  88
    [{'m1': ['j1', 'j3'], 'm2': ['j1', 'j4']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j1', 'j4']}  (100.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}   (50.0, False)
    
    Generation :  89
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j2', 'j3']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  90
    [{'m1': ['j4', 'j3'], 'm2': ['j2', 'j3']}, {'m1': ['j4', 'j3'], 'm2': ['j2', 'j3']}]
                                       schedule  fitness score
    1  {'m1': ['j4', 'j3'], 'm2': ['j2', 'j1']}   (50.0, True)
    0  {'m1': ['j4', 'j1'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  91
    [{'m1': ['j4', 'j3'], 'm2': ['j2', 'j2']}, {'m1': ['j4', 'j1'], 'm2': ['j2', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j2', 'j2']}  (50.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j2', 'j1']}  (50.0, False)
    
    Generation :  92
    [{'m1': ['j4', 'j3'], 'm2': ['j2', 'j2']}, {'m1': ['j4', 'j3'], 'm2': ['j2', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j2'], 'm2': ['j2', 'j2']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  93
    [{'m1': ['j4', 'j3'], 'm2': ['j2', 'j2']}, {'m1': ['j4', 'j2'], 'm2': ['j2', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j2', 'j3']}  (50.0, False)
    1  {'m1': ['j4', 'j2'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  94
    [{'m1': ['j4', 'j3'], 'm2': ['j2', 'j3']}, {'m1': ['j4', 'j3'], 'm2': ['j2', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j3']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j3']}  (50.0, False)
    
    Generation :  95
    [{'m1': ['j4', 'j3'], 'm2': ['j3', 'j3']}, {'m1': ['j4', 'j3'], 'm2': ['j3', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j3']}  (50.0, False)
    
    Generation :  96
    [{'m1': ['j4', 'j3'], 'm2': ['j1', 'j3']}, {'m1': ['j4', 'j3'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j1']}  (50.0, False)
    
    Generation :  97
    [{'m1': ['j4', 'j3'], 'm2': ['j3', 'j1']}, {'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j2'], 'm2': ['j3', 'j1']}   (50.0, True)
    1  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j1']}  (50.0, False)
    
    Generation :  98
    [{'m1': ['j4', 'j2'], 'm2': ['j1', 'j1']}, {'m1': ['j4', 'j3'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j2'], 'm2': ['j1', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j3', 'j2']}  (50.0, False)
    
    Generation :  99
    [{'m1': ['j4', 'j2'], 'm2': ['j1', 'j1']}, {'m1': ['j4', 'j2'], 'm2': ['j1', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j2'], 'm2': ['j1', 'j2']}  (50.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j1', 'j2']}  (50.0, False)
    
    Generation :  100
    [{'m1': ['j1', 'j2'], 'm2': ['j1', 'j2']}, {'m1': ['j1', 'j2'], 'm2': ['j1', 'j2']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j1'], 'm2': ['j1', 'j2']}  (100.0, False)
    0  {'m1': ['j1', 'j2'], 'm2': ['j1', 'j2']}   (50.0, False)
    
    Generation :  101
    [{'m1': ['j1', 'j2'], 'm2': ['j1', 'j2']}, {'m1': ['j1', 'j1'], 'm2': ['j1', 'j2']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j1'], 'm2': ['j1', 'j1']}  (100.0, False)
    0  {'m1': ['j1', 'j2'], 'm2': ['j3', 'j2']}   (50.0, False)
    
    Generation :  102
    [{'m1': ['j1', 'j1'], 'm2': ['j3', 'j2']}, {'m1': ['j1', 'j2'], 'm2': ['j1', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j3', 'j4']}  (100.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j4', 'j1']}   (50.0, False)
    
    Generation :  103
    [{'m1': ['j1', 'j2'], 'm2': ['j3', 'j1']}, {'m1': ['j1', 'j1'], 'm2': ['j4', 'j4']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j4']}  (100.0, False)
    0  {'m1': ['j3', 'j2'], 'm2': ['j4', 'j1']}    (50.0, True)
    
    Generation :  104
    [{'m1': ['j3', 'j2'], 'm2': ['j4', 'j1']}, {'m1': ['j3', 'j2'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    1  {'m1': ['j3', 'j2'], 'm2': ['j4', 'j1']}   (50.0, True)
    0  {'m1': ['j2', 'j2'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  105
    [{'m1': ['j2', 'j2'], 'm2': ['j4', 'j1']}, {'m1': ['j3', 'j2'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    1  {'m1': ['j3', 'j2'], 'm2': ['j4', 'j1']}   (50.0, True)
    0  {'m1': ['j2', 'j2'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  106
    [{'m1': ['j3', 'j2'], 'm2': ['j4', 'j1']}, {'m1': ['j2', 'j2'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j2'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  107
    [{'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j3'], 'm2': ['j4', 'j1']}  (100.0, False)
    1  {'m1': ['j3', 'j3'], 'm2': ['j4', 'j1']}  (100.0, False)
    
    Generation :  108
    [{'m1': ['j3', 'j3'], 'm2': ['j4', 'j1']}, {'m1': ['j3', 'j3'], 'm2': ['j4', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j3'], 'm2': ['j4', 'j1']}  (100.0, False)
    1  {'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}  (100.0, False)
    
    Generation :  109
    [{'m1': ['j3', 'j3'], 'm2': ['j4', 'j1']}, {'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}]
                                       schedule   fitness score
    1  {'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}  (100.0, False)
    0  {'m1': ['j3', 'j2'], 'm2': ['j4', 'j1']}    (50.0, True)
    
    Generation :  110
    [{'m1': ['j3', 'j2'], 'm2': ['j4', 'j1']}, {'m1': ['j3', 'j2'], 'm2': ['j4', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}  (100.0, False)
    1  {'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}  (100.0, False)
    
    Generation :  111
    [{'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}, {'m1': ['j3', 'j1'], 'm2': ['j4', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j1'], 'm2': ['j1', 'j1']}  (100.0, False)
    1  {'m1': ['j3', 'j1'], 'm2': ['j1', 'j1']}  (100.0, False)
    
    Generation :  112
    [{'m1': ['j3', 'j1'], 'm2': ['j1', 'j1']}, {'m1': ['j3', 'j1'], 'm2': ['j1', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j1'], 'm2': ['j1', 'j2']}  (50.0, False)
    1  {'m1': ['j3', 'j4'], 'm2': ['j1', 'j1']}  (50.0, False)
    
    Generation :  113
    [{'m1': ['j4', 'j1'], 'm2': ['j1', 'j2']}, {'m1': ['j4', 'j1'], 'm2': ['j1', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j1'], 'm2': ['j1', 'j2']}  (50.0, False)
    1  {'m1': ['j4', 'j1'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  114
    [{'m1': ['j4', 'j1'], 'm2': ['j1', 'j4']}, {'m1': ['j4', 'j1'], 'm2': ['j1', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j1'], 'm2': ['j1', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j1'], 'm2': ['j1', 'j3']}  (50.0, False)
    
    Generation :  115
    [{'m1': ['j4', 'j1'], 'm2': ['j1', 'j4']}, {'m1': ['j4', 'j1'], 'm2': ['j1', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j1'], 'm2': ['j1', 'j3']}  (50.0, False)
    
    Generation :  116
    [{'m1': ['j4', 'j3'], 'm2': ['j4', 'j4']}, {'m1': ['j4', 'j3'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j1'], 'm2': ['j4', 'j4']}  (50.0, False)
    
    Generation :  117
    [{'m1': ['j4', 'j1'], 'm2': ['j4', 'j4']}, {'m1': ['j4', 'j1'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j1'], 'm2': ['j4', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j1'], 'm2': ['j3', 'j1']}  (50.0, False)
    
    Generation :  118
    [{'m1': ['j4', 'j1'], 'm2': ['j3', 'j1']}, {'m1': ['j4', 'j1'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  119
    [{'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}, {'m1': ['j4', 'j3'], 'm2': ['j3', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j1']}  (50.0, False)
    
    Generation :  120
    [{'m1': ['j4', 'j4'], 'm2': ['j3', 'j1']}, {'m1': ['j4', 'j3'], 'm2': ['j1', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j1']}  (50.0, False)
    
    Generation :  121
    [{'m1': ['j4', 'j4'], 'm2': ['j3', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    1  {'m1': ['j4', 'j2'], 'm2': ['j3', 'j1']}   (50.0, True)
    0  {'m1': ['j2', 'j4'], 'm2': ['j2', 'j1']}  (50.0, False)
    
    Generation :  122
    [{'m1': ['j4', 'j2'], 'm2': ['j2', 'j1']}, {'m1': ['j2', 'j4'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j1'], 'm2': ['j2', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j3', 'j1']}  (50.0, False)
    
    Generation :  123
    [{'m1': ['j2', 'j2'], 'm2': ['j3', 'j1']}, {'m1': ['j2', 'j2'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j2'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j1', 'j1']}  (50.0, False)
    
    Generation :  124
    [{'m1': ['j2', 'j2'], 'm2': ['j3', 'j1']}, {'m1': ['j2', 'j2'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}  (50.0, False)
    1  {'m1': ['j4', 'j2'], 'm2': ['j3', 'j3']}  (50.0, False)
    
    Generation :  125
    [{'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}, {'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j4'], 'm2': ['j2', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j4'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  126
    [{'m1': ['j2', 'j4'], 'm2': ['j2', 'j3']}, {'m1': ['j2', 'j4'], 'm2': ['j2', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j3', 'j2'], 'm2': ['j2', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j4'], 'm2': ['j3', 'j3']}  (50.0, False)
    
    Generation :  127
    [{'m1': ['j2', 'j4'], 'm2': ['j2', 'j3']}, {'m1': ['j3', 'j2'], 'm2': ['j3', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j4'], 'm2': ['j3', 'j1']}   (50.0, True)
    1  {'m1': ['j3', 'j2'], 'm2': ['j3', 'j3']}  (50.0, False)
    
    Generation :  128
    [{'m1': ['j2', 'j4'], 'm2': ['j3', 'j1']}, {'m1': ['j2', 'j4'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    1  {'m1': ['j2', 'j4'], 'm2': ['j3', 'j1']}   (50.0, True)
    0  {'m1': ['j1', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  129
    [{'m1': ['j1', 'j4'], 'm2': ['j3', 'j1']}, {'m1': ['j2', 'j4'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j3', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j1'], 'm2': ['j4', 'j4']}  (50.0, False)
    
    Generation :  130
    [{'m1': ['j2', 'j1'], 'm2': ['j3', 'j2']}, {'m1': ['j4', 'j4'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j1'], 'm2': ['j3', 'j3']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j3']}  (50.0, False)
    
    Generation :  131
    [{'m1': ['j4', 'j4'], 'm2': ['j3', 'j3']}, {'m1': ['j2', 'j1'], 'm2': ['j4', 'j3']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}  (100.0, False)
    0  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j1']}   (50.0, False)
    
    Generation :  132
    [{'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}, {'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}]
                                       schedule  fitness score
    1  {'m1': ['j2', 'j1'], 'm2': ['j4', 'j3']}   (50.0, True)
    0  {'m1': ['j4', 'j1'], 'm2': ['j4', 'j3']}  (50.0, False)
    
    Generation :  133
    [{'m1': ['j2', 'j1'], 'm2': ['j4', 'j3']}, {'m1': ['j4', 'j1'], 'm2': ['j4', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}  (100.0, False)
    1  {'m1': ['j4', 'j1'], 'm2': ['j4', 'j3']}   (50.0, False)
    
    Generation :  134
    [{'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}, {'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}  (100.0, False)
    1  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j4']}  (100.0, False)
    
    Generation :  135
    [{'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}, {'m1': ['j1', 'j1'], 'm2': ['j4', 'j4']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}  (100.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j4', 'j1']}   (50.0, False)
    
    Generation :  136
    [{'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}, {'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}  (100.0, False)
    1  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}  (100.0, False)
    
    Generation :  137
    [{'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}, {'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}  (100.0, False)
    0  {'m1': ['j1', 'j2'], 'm2': ['j4', 'j3']}    (50.0, True)
    
    Generation :  138
    [{'m1': ['j1', 'j2'], 'm2': ['j4', 'j3']}, {'m1': ['j1', 'j2'], 'm2': ['j4', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j2'], 'm2': ['j3', 'j3']}  (50.0, False)
    1  {'m1': ['j4', 'j2'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  139
    [{'m1': ['j4', 'j2'], 'm2': ['j3', 'j3']}, {'m1': ['j1', 'j2'], 'm2': ['j2', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j2'], 'm2': ['j3', 'j4']}  (50.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  140
    [{'m1': ['j1', 'j2'], 'm2': ['j2', 'j4']}, {'m1': ['j4', 'j2'], 'm2': ['j3', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j2'], 'm2': ['j2', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j3', 'j3']}  (50.0, False)
    
    Generation :  141
    [{'m1': ['j1', 'j2'], 'm2': ['j2', 'j1']}, {'m1': ['j1', 'j2'], 'm2': ['j2', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j3'], 'm2': ['j4', 'j1']}  (100.0, False)
    1  {'m1': ['j1', 'j3'], 'm2': ['j2', 'j1']}  (100.0, False)
    
    Generation :  142
    [{'m1': ['j3', 'j3'], 'm2': ['j4', 'j1']}, {'m1': ['j1', 'j3'], 'm2': ['j2', 'j1']}]
                                       schedule   fitness score
    1  {'m1': ['j3', 'j3'], 'm2': ['j2', 'j1']}  (100.0, False)
    0  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j1']}   (50.0, False)
    
    Generation :  143
    [{'m1': ['j4', 'j3'], 'm2': ['j4', 'j1']}, {'m1': ['j4', 'j3'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  144
    [{'m1': ['j4', 'j3'], 'm2': ['j4', 'j1']}, {'m1': ['j4', 'j3'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j1']}  (50.0, False)
    
    Generation :  145
    [{'m1': ['j4', 'j3'], 'm2': ['j3', 'j1']}, {'m1': ['j4', 'j3'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}  (50.0, False)
    
    Generation :  146
    [{'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}, {'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j3', 'j1']}  (100.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}   (50.0, False)
    
    Generation :  147
    [{'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}, {'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}  (50.0, False)
    
    Generation :  148
    [{'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}, {'m1': ['j2', 'j3'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  149
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j2', 'j1']}  (100.0, False)
    1  {'m1': ['j1', 'j3'], 'm2': ['j2', 'j1']}  (100.0, False)
    
    Generation :  150
    [{'m1': ['j1', 'j3'], 'm2': ['j2', 'j1']}, {'m1': ['j1', 'j3'], 'm2': ['j2', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j2', 'j1']}  (100.0, False)
    1  {'m1': ['j1', 'j1'], 'm2': ['j2', 'j1']}  (100.0, False)
    
    Generation :  151
    [{'m1': ['j1', 'j1'], 'm2': ['j2', 'j1']}, {'m1': ['j1', 'j1'], 'm2': ['j2', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j2', 'j1']}  (100.0, False)
    1  {'m1': ['j1', 'j1'], 'm2': ['j2', 'j3']}  (100.0, False)
    
    Generation :  152
    [{'m1': ['j1', 'j1'], 'm2': ['j2', 'j1']}, {'m1': ['j1', 'j1'], 'm2': ['j2', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j2', 'j1']}  (100.0, False)
    1  {'m1': ['j1', 'j1'], 'm2': ['j2', 'j3']}  (100.0, False)
    
    Generation :  153
    [{'m1': ['j1', 'j1'], 'm2': ['j2', 'j1']}, {'m1': ['j1', 'j1'], 'm2': ['j2', 'j3']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j1'], 'm2': ['j1', 'j3']}  (100.0, False)
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}   (50.0, False)
    
    Generation :  154
    [{'m1': ['j2', 'j3'], 'm2': ['j1', 'j3']}, {'m1': ['j1', 'j1'], 'm2': ['j2', 'j1']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j1']}  (100.0, False)
    0  {'m1': ['j2', 'j4'], 'm2': ['j1', 'j3']}    (50.0, True)
    
    Generation :  155
    [{'m1': ['j1', 'j1'], 'm2': ['j4', 'j1']}, {'m1': ['j1', 'j1'], 'm2': ['j4', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j1']}  (100.0, False)
    1  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j1']}  (100.0, False)
    
    Generation :  156
    [{'m1': ['j1', 'j1'], 'm2': ['j4', 'j1']}, {'m1': ['j1', 'j1'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j2'], 'm2': ['j4', 'j1']}  (50.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  157
    [{'m1': ['j1', 'j2'], 'm2': ['j4', 'j1']}, {'m1': ['j1', 'j2'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j2'], 'm2': ['j4', 'j1']}  (50.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  158
    [{'m1': ['j1', 'j2'], 'm2': ['j4', 'j1']}, {'m1': ['j1', 'j2'], 'm2': ['j4', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j1']}  (100.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j4', 'j1']}   (50.0, False)
    
    Generation :  159
    [{'m1': ['j1', 'j1'], 'm2': ['j4', 'j1']}, {'m1': ['j1', 'j2'], 'm2': ['j4', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j1']}  (100.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j4', 'j4']}   (50.0, False)
    
    Generation :  160
    [{'m1': ['j1', 'j2'], 'm2': ['j4', 'j4']}, {'m1': ['j1', 'j1'], 'm2': ['j4', 'j1']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j1'], 'm2': ['j2', 'j1']}  (100.0, False)
    0  {'m1': ['j1', 'j2'], 'm2': ['j4', 'j4']}   (50.0, False)
    
    Generation :  161
    [{'m1': ['j1', 'j2'], 'm2': ['j2', 'j1']}, {'m1': ['j1', 'j1'], 'm2': ['j4', 'j4']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j4']}  (100.0, False)
    0  {'m1': ['j1', 'j2'], 'm2': ['j2', 'j1']}   (50.0, False)
    
    Generation :  162
    [{'m1': ['j1', 'j1'], 'm2': ['j2', 'j4']}, {'m1': ['j1', 'j2'], 'm2': ['j4', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j2', 'j4']}  (100.0, False)
    1  {'m1': ['j1', 'j2'], 'm2': ['j4', 'j3']}    (50.0, True)
    
    Generation :  163
    [{'m1': ['j1', 'j2'], 'm2': ['j4', 'j3']}, {'m1': ['j1', 'j2'], 'm2': ['j4', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j4'], 'm2': ['j4', 'j3']}  (50.0, False)
    1  {'m1': ['j1', 'j4'], 'm2': ['j4', 'j3']}  (50.0, False)
    
    Generation :  164
    [{'m1': ['j1', 'j4'], 'm2': ['j4', 'j3']}, {'m1': ['j1', 'j4'], 'm2': ['j4', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j4'], 'm2': ['j4', 'j3']}  (50.0, False)
    1  {'m1': ['j1', 'j4'], 'm2': ['j4', 'j3']}  (50.0, False)
    
    Generation :  165
    [{'m1': ['j1', 'j4'], 'm2': ['j4', 'j3']}, {'m1': ['j1', 'j4'], 'm2': ['j4', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j3']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j3']}  (50.0, False)
    
    Generation :  166
    [{'m1': ['j4', 'j4'], 'm2': ['j4', 'j3']}, {'m1': ['j4', 'j4'], 'm2': ['j4', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j2']}  (50.0, False)
    1  {'m1': ['j1', 'j4'], 'm2': ['j4', 'j3']}  (50.0, False)
    
    Generation :  167
    [{'m1': ['j1', 'j4'], 'm2': ['j4', 'j3']}, {'m1': ['j1', 'j4'], 'm2': ['j4', 'j3']}]
                                       schedule fitness score
    0  {'m1': ['j1', 'j4'], 'm2': ['j2', 'j3']}  (50.0, True)
    1  {'m1': ['j1', 'j4'], 'm2': ['j2', 'j3']}  (50.0, True)
    
    Generation :  168
    [{'m1': ['j1', 'j4'], 'm2': ['j2', 'j3']}, {'m1': ['j1', 'j4'], 'm2': ['j2', 'j3']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j1'], 'm2': ['j2', 'j3']}  (100.0, False)
    0  {'m1': ['j1', 'j4'], 'm2': ['j2', 'j3']}    (50.0, True)
    
    Generation :  169
    [{'m1': ['j1', 'j1'], 'm2': ['j2', 'j3']}, {'m1': ['j1', 'j1'], 'm2': ['j2', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j3', 'j3']}  (100.0, False)
    1  {'m1': ['j1', 'j1'], 'm2': ['j3', 'j3']}  (100.0, False)
    
    Generation :  170
    [{'m1': ['j1', 'j1'], 'm2': ['j3', 'j3']}, {'m1': ['j1', 'j1'], 'm2': ['j3', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j2', 'j3']}  (100.0, False)
    1  {'m1': ['j1', 'j1'], 'm2': ['j3', 'j1']}  (100.0, False)
    
    Generation :  171
    [{'m1': ['j1', 'j1'], 'm2': ['j3', 'j3']}, {'m1': ['j1', 'j1'], 'm2': ['j2', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j3', 'j3']}  (100.0, False)
    1  {'m1': ['j1', 'j1'], 'm2': ['j2', 'j1']}  (100.0, False)
    
    Generation :  172
    [{'m1': ['j1', 'j1'], 'm2': ['j2', 'j1']}, {'m1': ['j1', 'j1'], 'm2': ['j2', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j4'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j1', 'j4'], 'm2': ['j3', 'j1']}  (50.0, False)
    
    Generation :  173
    [{'m1': ['j1', 'j4'], 'm2': ['j3', 'j1']}, {'m1': ['j1', 'j4'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j4'], 'm2': ['j3', 'j1']}  (50.0, False)
    1  {'m1': ['j1', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  174
    [{'m1': ['j1', 'j4'], 'm2': ['j4', 'j1']}, {'m1': ['j1', 'j4'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    1  {'m1': ['j1', 'j4'], 'm2': ['j3', 'j1']}  (50.0, False)
    
    Generation :  175
    [{'m1': ['j1', 'j4'], 'm2': ['j3', 'j1']}, {'m1': ['j1', 'j4'], 'm2': ['j3', 'j1']}]
                                       schedule  fitness score
    1  {'m1': ['j1', 'j4'], 'm2': ['j3', 'j2']}   (50.0, True)
    0  {'m1': ['j1', 'j4'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  176
    [{'m1': ['j1', 'j4'], 'm2': ['j3', 'j2']}, {'m1': ['j1', 'j4'], 'm2': ['j2', 'j2']}]
                                       schedule fitness score
    0  {'m1': ['j1', 'j4'], 'm2': ['j3', 'j2']}  (50.0, True)
    1  {'m1': ['j1', 'j4'], 'm2': ['j2', 'j3']}  (50.0, True)
    
    Generation :  177
    [{'m1': ['j1', 'j4'], 'm2': ['j3', 'j2']}, {'m1': ['j1', 'j4'], 'm2': ['j3', 'j2']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j3'], 'm2': ['j4', 'j2']}  (100.0, False)
    1  {'m1': ['j3', 'j3'], 'm2': ['j4', 'j2']}  (100.0, False)
    
    Generation :  178
    [{'m1': ['j3', 'j3'], 'm2': ['j4', 'j2']}, {'m1': ['j3', 'j3'], 'm2': ['j4', 'j2']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j3'], 'm2': ['j3', 'j2']}  (100.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j4', 'j2']}   (50.0, False)
    
    Generation :  179
    [{'m1': ['j3', 'j3'], 'm2': ['j4', 'j2']}, {'m1': ['j2', 'j3'], 'm2': ['j3', 'j2']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j3'], 'm2': ['j4', 'j2']}  (100.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}   (50.0, False)
    
    Generation :  180
    [{'m1': ['j3', 'j3'], 'm2': ['j2', 'j2']}, {'m1': ['j2', 'j3'], 'm2': ['j4', 'j2']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j3'], 'm2': ['j2', 'j2']}  (100.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j4', 'j2']}   (50.0, False)
    
    Generation :  181
    [{'m1': ['j3', 'j3'], 'm2': ['j2', 'j2']}, {'m1': ['j3', 'j3'], 'm2': ['j2', 'j2']}]
                                       schedule   fitness score
    1  {'m1': ['j3', 'j3'], 'm2': ['j2', 'j1']}  (100.0, False)
    0  {'m1': ['j4', 'j2'], 'm2': ['j2', 'j2']}   (50.0, False)
    
    Generation :  182
    [{'m1': ['j3', 'j3'], 'm2': ['j2', 'j1']}, {'m1': ['j3', 'j3'], 'm2': ['j2', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j3'], 'm2': ['j2', 'j1']}  (100.0, False)
    1  {'m1': ['j3', 'j3'], 'm2': ['j2', 'j1']}  (100.0, False)
    
    Generation :  183
    [{'m1': ['j3', 'j3'], 'm2': ['j2', 'j1']}, {'m1': ['j3', 'j3'], 'm2': ['j2', 'j1']}]
                                       schedule   fitness score
    1  {'m1': ['j3', 'j1'], 'm2': ['j2', 'j1']}  (100.0, False)
    0  {'m1': ['j3', 'j4'], 'm2': ['j4', 'j1']}   (50.0, False)
    
    Generation :  184
    [{'m1': ['j3', 'j4'], 'm2': ['j4', 'j1']}, {'m1': ['j3', 'j4'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j3', 'j4'], 'm2': ['j4', 'j3']}  (50.0, False)
    1  {'m1': ['j3', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  185
    [{'m1': ['j3', 'j4'], 'm2': ['j4', 'j1']}, {'m1': ['j3', 'j4'], 'm2': ['j4', 'j3']}]
                                       schedule   fitness score
    1  {'m1': ['j3', 'j3'], 'm2': ['j2', 'j3']}  (100.0, False)
    0  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}   (50.0, False)
    
    Generation :  186
    [{'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}  (50.0, False)
    
    Generation :  187
    [{'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}, {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  188
    [{'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j3']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j1']}  (50.0, False)
    
    Generation :  189
    [{'m1': ['j4', 'j4'], 'm2': ['j1', 'j3']}, {'m1': ['j4', 'j4'], 'm2': ['j1', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j3']}  (50.0, False)
    1  {'m1': ['j1', 'j4'], 'm2': ['j1', 'j3']}  (50.0, False)
    
    Generation :  190
    [{'m1': ['j1', 'j4'], 'm2': ['j1', 'j3']}, {'m1': ['j1', 'j4'], 'm2': ['j1', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j1', 'j4']}  (100.0, False)
    1  {'m1': ['j1', 'j3'], 'm2': ['j1', 'j4']}  (100.0, False)
    
    Generation :  191
    [{'m1': ['j1', 'j3'], 'm2': ['j1', 'j4']}, {'m1': ['j1', 'j3'], 'm2': ['j1', 'j4']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j1', 'j4']}  (100.0, False)
    1  {'m1': ['j1', 'j3'], 'm2': ['j1', 'j4']}  (100.0, False)
    
    Generation :  192
    [{'m1': ['j1', 'j3'], 'm2': ['j1', 'j4']}, {'m1': ['j1', 'j3'], 'm2': ['j1', 'j4']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j2', 'j2']}  (100.0, False)
    1  {'m1': ['j1', 'j1'], 'm2': ['j2', 'j2']}  (100.0, False)
    
    Generation :  193
    [{'m1': ['j1', 'j1'], 'm2': ['j2', 'j2']}, {'m1': ['j1', 'j1'], 'm2': ['j2', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j1'], 'm2': ['j2', 'j4']}  (50.0, False)
    1  {'m1': ['j2', 'j1'], 'm2': ['j2', 'j4']}  (50.0, False)
    
    Generation :  194
    [{'m1': ['j2', 'j1'], 'm2': ['j2', 'j4']}, {'m1': ['j2', 'j1'], 'm2': ['j2', 'j4']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j1'], 'm2': ['j2', 'j1']}  (100.0, False)
    0  {'m1': ['j4', 'j1'], 'm2': ['j4', 'j4']}   (50.0, False)
    
    Generation :  195
    [{'m1': ['j1', 'j1'], 'm2': ['j2', 'j4']}, {'m1': ['j4', 'j1'], 'm2': ['j4', 'j1']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j4']}  (100.0, False)
    1  {'m1': ['j4', 'j1'], 'm2': ['j4', 'j1']}   (50.0, False)
    
    Generation :  196
    [{'m1': ['j1', 'j1'], 'm2': ['j4', 'j4']}, {'m1': ['j1', 'j1'], 'm2': ['j4', 'j4']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j4']}  (100.0, False)
    1  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j4']}  (100.0, False)
    
    Generation :  197
    [{'m1': ['j1', 'j1'], 'm2': ['j4', 'j4']}, {'m1': ['j1', 'j1'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j2'], 'm2': ['j4', 'j4']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j4', 'j4']}  (50.0, False)
    
    Generation :  198
    [{'m1': ['j2', 'j2'], 'm2': ['j4', 'j4']}, {'m1': ['j2', 'j2'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j2'], 'm2': ['j4', 'j4']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  199
    [{'m1': ['j2', 'j2'], 'm2': ['j4', 'j4']}, {'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j2'], 'm2': ['j3', 'j4']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  200
    [{'m1': ['j2', 'j2'], 'm2': ['j3', 'j4']}, {'m1': ['j2', 'j2'], 'm2': ['j3', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j1'], 'm2': ['j3', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j1'], 'm2': ['j3', 'j2']}  (50.0, False)
    
    Generation :  201
    [{'m1': ['j2', 'j1'], 'm2': ['j3', 'j2']}, {'m1': ['j2', 'j1'], 'm2': ['j3', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}  (50.0, False)
    
    Generation :  202
    [{'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}, {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}  (50.0, False)
    
    Generation :  203
    [{'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}, {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}  (50.0, False)
    
    Generation :  204
    [{'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}, {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}  (50.0, False)
    
    Generation :  205
    [{'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}, {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}  (50.0, False)
    
    Generation :  206
    [{'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}, {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}  (50.0, False)
    
    Generation :  207
    [{'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}, {'m1': ['j2', 'j4'], 'm2': ['j4', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j4'], 'm2': ['j4', 'j4']}  (50.0, False)
    1  {'m1': ['j2', 'j4'], 'm2': ['j4', 'j4']}  (50.0, False)
    
    Generation :  208
    [{'m1': ['j2', 'j4'], 'm2': ['j4', 'j4']}, {'m1': ['j2', 'j4'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    1  {'m1': ['j2', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  209
    [{'m1': ['j2', 'j4'], 'm2': ['j1', 'j4']}, {'m1': ['j2', 'j4'], 'm2': ['j1', 'j4']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j1', 'j4']}  (100.0, False)
    1  {'m1': ['j1', 'j3'], 'm2': ['j1', 'j4']}  (100.0, False)
    
    Generation :  210
    [{'m1': ['j1', 'j3'], 'm2': ['j1', 'j4']}, {'m1': ['j1', 'j3'], 'm2': ['j1', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    1  {'m1': ['j2', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  211
    [{'m1': ['j2', 'j4'], 'm2': ['j1', 'j4']}, {'m1': ['j2', 'j4'], 'm2': ['j1', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  212
    [{'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}, {'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  213
    [{'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}, {'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  214
    [{'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}, {'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j1', 'j4']}   (50.0, True)
    1  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  215
    [{'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}, {'m1': ['j2', 'j3'], 'm2': ['j1', 'j4']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j3'], 'm2': ['j1', 'j4']}  (100.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j3', 'j4']}   (50.0, False)
    
    Generation :  216
    [{'m1': ['j3', 'j3'], 'm2': ['j3', 'j4']}, {'m1': ['j2', 'j3'], 'm2': ['j1', 'j4']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j3'], 'm2': ['j3', 'j4']}  (100.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}   (50.0, False)
    
    Generation :  217
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}  (50.0, False)
    
    Generation :  218
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  219
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  220
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  221
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}]
                                       schedule   fitness score
    1  {'m1': ['j3', 'j3'], 'm2': ['j2', 'j3']}  (100.0, False)
    0  {'m1': ['j2', 'j3'], 'm2': ['j3', 'j3']}   (50.0, False)
    
    Generation :  222
    [{'m1': ['j3', 'j3'], 'm2': ['j3', 'j3']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j3'], 'm2': ['j3', 'j3']}  (100.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}   (50.0, False)
    
    Generation :  223
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j3'], 'm2': ['j2', 'j3']}  (100.0, False)
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}   (50.0, False)
    
    Generation :  224
    [{'m1': ['j1', 'j3'], 'm2': ['j2', 'j3']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j2', 'j3']}  (100.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}   (50.0, False)
    
    Generation :  225
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  226
    [{'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}, {'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  227
    [{'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}, {'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j4', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  228
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}, {'m1': ['j2', 'j2'], 'm2': ['j4', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j4', 'j2']}  (50.0, False)
    
    Generation :  229
    [{'m1': ['j2', 'j3'], 'm2': ['j4', 'j2']}, {'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j4', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  230
    [{'m1': ['j2', 'j2'], 'm2': ['j4', 'j3']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j4', 'j3']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  231
    [{'m1': ['j2', 'j3'], 'm2': ['j4', 'j3']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j1', 'j2']}  (100.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}   (50.0, False)
    
    Generation :  232
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  233
    [{'m1': ['j2', 'j2'], 'm2': ['j2', 'j2']}, {'m1': ['j2', 'j2'], 'm2': ['j2', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j4'], 'm2': ['j2', 'j2']}  (50.0, False)
    1  {'m1': ['j2', 'j4'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  234
    [{'m1': ['j2', 'j4'], 'm2': ['j2', 'j2']}, {'m1': ['j2', 'j4'], 'm2': ['j2', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j2'], 'm2': ['j2', 'j1']}  (50.0, False)
    
    Generation :  235
    [{'m1': ['j2', 'j2'], 'm2': ['j2', 'j1']}, {'m1': ['j2', 'j2'], 'm2': ['j2', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}  (50.0, False)
    
    Generation :  236
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  237
    [{'m1': ['j4', 'j3'], 'm2': ['j4', 'j1']}, {'m1': ['j4', 'j3'], 'm2': ['j4', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j1']}  (50.0, False)
    
    Generation :  238
    [{'m1': ['j4', 'j3'], 'm2': ['j4', 'j1']}, {'m1': ['j4', 'j3'], 'm2': ['j4', 'j1']}]
                                       schedule fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j2', 'j1']}  (50.0, True)
    1  {'m1': ['j4', 'j3'], 'm2': ['j2', 'j1']}  (50.0, True)
    
    Generation :  239
    [{'m1': ['j4', 'j3'], 'm2': ['j2', 'j1']}, {'m1': ['j4', 'j3'], 'm2': ['j2', 'j1']}]
                                       schedule  fitness score
    1  {'m1': ['j4', 'j3'], 'm2': ['j2', 'j1']}   (50.0, True)
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}  (50.0, False)
    
    Generation :  240
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}, {'m1': ['j4', 'j3'], 'm2': ['j2', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  241
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j1']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}  (50.0, False)
    1  {'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}  (50.0, False)
    
    Generation :  242
    [{'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}, {'m1': ['j2', 'j3'], 'm2': ['j2', 'j4']}]
                                       schedule   fitness score
    1  {'m1': ['j3', 'j3'], 'm2': ['j2', 'j4']}  (100.0, False)
    0  {'m1': ['j1', 'j2'], 'm2': ['j2', 'j4']}   (50.0, False)
    
    Generation :  243
    [{'m1': ['j1', 'j3'], 'm2': ['j2', 'j4']}, {'m1': ['j3', 'j2'], 'm2': ['j2', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j2', 'j4']}  (50.0, False)
    1  {'m1': ['j3', 'j2'], 'm2': ['j2', 'j4']}  (50.0, False)
    
    Generation :  244
    [{'m1': ['j4', 'j4'], 'm2': ['j2', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j2', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j2', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j2', 'j3']}  (50.0, False)
    
    Generation :  245
    [{'m1': ['j4', 'j4'], 'm2': ['j2', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j2', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j2', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j2', 'j4']}  (50.0, False)
    
    Generation :  246
    [{'m1': ['j4', 'j4'], 'm2': ['j2', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j2', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j2', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j2', 'j4']}  (50.0, False)
    
    Generation :  247
    [{'m1': ['j4', 'j4'], 'm2': ['j2', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j2', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j2', 'j4']}  (50.0, False)
    
    Generation :  248
    [{'m1': ['j4', 'j4'], 'm2': ['j2', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j4'], 'm2': ['j2', 'j4']}  (50.0, False)
    1  {'m1': ['j3', 'j4'], 'm2': ['j4', 'j4']}  (50.0, False)
    
    Generation :  249
    [{'m1': ['j3', 'j4'], 'm2': ['j4', 'j4']}, {'m1': ['j3', 'j4'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j3', 'j4'], 'm2': ['j4', 'j4']}  (50.0, False)
    1  {'m1': ['j3', 'j4'], 'm2': ['j4', 'j4']}  (50.0, False)
    
    Generation :  250
    [{'m1': ['j3', 'j4'], 'm2': ['j4', 'j4']}, {'m1': ['j3', 'j4'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  251
    [{'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  252
    [{'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  253
    [{'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  254
    [{'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  255
    [{'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}  (50.0, False)
    
    Generation :  256
    [{'m1': ['j4', 'j3'], 'm2': ['j1', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j1', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j3']}  (50.0, False)
    
    Generation :  257
    [{'m1': ['j4', 'j3'], 'm2': ['j4', 'j4']}, {'m1': ['j4', 'j3'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j4']}  (50.0, False)
    
    Generation :  258
    [{'m1': ['j4', 'j3'], 'm2': ['j4', 'j4']}, {'m1': ['j4', 'j3'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j4']}  (50.0, False)
    
    Generation :  259
    [{'m1': ['j4', 'j4'], 'm2': ['j4', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j4']}  (50.0, False)
    
    Generation :  260
    [{'m1': ['j4', 'j4'], 'm2': ['j4', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j4'], 'm2': ['j4', 'j4']}  (50.0, False)
    
    Generation :  261
    [{'m1': ['j4', 'j4'], 'm2': ['j4', 'j4']}, {'m1': ['j4', 'j4'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j1', 'j4'], 'm2': ['j4', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j4']}  (50.0, False)
    
    Generation :  262
    [{'m1': ['j4', 'j3'], 'm2': ['j4', 'j4']}, {'m1': ['j4', 'j3'], 'm2': ['j4', 'j4']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j2', 'j4']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j2', 'j4']}  (50.0, False)
    
    Generation :  263
    [{'m1': ['j4', 'j3'], 'm2': ['j2', 'j4']}, {'m1': ['j4', 'j3'], 'm2': ['j2', 'j4']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j3'], 'm2': ['j2', 'j4']}  (100.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j4']}   (50.0, False)
    
    Generation :  264
    [{'m1': ['j3', 'j3'], 'm2': ['j2', 'j4']}, {'m1': ['j4', 'j3'], 'm2': ['j3', 'j4']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j3'], 'm2': ['j2', 'j3']}  (100.0, False)
    1  {'m1': ['j1', 'j3'], 'm2': ['j3', 'j2']}  (100.0, False)
    
    Generation :  265
    [{'m1': ['j1', 'j3'], 'm2': ['j3', 'j3']}, {'m1': ['j3', 'j3'], 'm2': ['j2', 'j2']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j4', 'j3']}  (100.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j2']}   (50.0, False)
    
    Generation :  266
    [{'m1': ['j1', 'j3'], 'm2': ['j4', 'j2']}, {'m1': ['j4', 'j3'], 'm2': ['j4', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j2', 'j3'], 'm2': ['j4', 'j2']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j3']}  (50.0, False)
    
    Generation :  267
    [{'m1': ['j2', 'j3'], 'm2': ['j4', 'j3']}, {'m1': ['j4', 'j3'], 'm2': ['j4', 'j2']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j3']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j4', 'j2']}  (50.0, False)
    
    Generation :  268
    [{'m1': ['j4', 'j3'], 'm2': ['j4', 'j3']}, {'m1': ['j4', 'j3'], 'm2': ['j4', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j3']}  (50.0, False)
    1  {'m1': ['j4', 'j3'], 'm2': ['j3', 'j3']}  (50.0, False)
    
    Generation :  269
    [{'m1': ['j4', 'j3'], 'm2': ['j3', 'j3']}, {'m1': ['j4', 'j3'], 'm2': ['j3', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j3'], 'm2': ['j3', 'j3']}  (100.0, False)
    1  {'m1': ['j3', 'j3'], 'm2': ['j3', 'j3']}  (100.0, False)
    
    Generation :  270
    [{'m1': ['j3', 'j3'], 'm2': ['j3', 'j3']}, {'m1': ['j3', 'j3'], 'm2': ['j3', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j3', 'j3'], 'm2': ['j3', 'j3']}  (100.0, False)
    1  {'m1': ['j3', 'j3'], 'm2': ['j1', 'j3']}  (100.0, False)
    
    Generation :  271
    [{'m1': ['j3', 'j3'], 'm2': ['j3', 'j3']}, {'m1': ['j3', 'j3'], 'm2': ['j3', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j3', 'j3']}  (100.0, False)
    1  {'m1': ['j1', 'j3'], 'm2': ['j3', 'j3']}  (100.0, False)
    
    Generation :  272
    [{'m1': ['j1', 'j3'], 'm2': ['j3', 'j3']}, {'m1': ['j1', 'j3'], 'm2': ['j3', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j4', 'j3']}  (100.0, False)
    1  {'m1': ['j1', 'j3'], 'm2': ['j3', 'j3']}  (100.0, False)
    
    Generation :  273
    [{'m1': ['j1', 'j3'], 'm2': ['j3', 'j3']}, {'m1': ['j1', 'j3'], 'm2': ['j4', 'j3']}]
                                       schedule   fitness score
    0  {'m1': ['j1', 'j3'], 'm2': ['j3', 'j3']}  (100.0, False)
    1  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}  (100.0, False)
    
    Generation :  274
    [{'m1': ['j1', 'j3'], 'm2': ['j4', 'j3']}, {'m1': ['j1', 'j1'], 'm2': ['j3', 'j3']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j1'], 'm2': ['j3', 'j3']}  (100.0, False)
    0  {'m1': ['j1', 'j2'], 'm2': ['j1', 'j4']}   (50.0, False)
    
    Generation :  275
    [{'m1': ['j1', 'j1'], 'm2': ['j3', 'j3']}, {'m1': ['j1', 'j1'], 'm2': ['j3', 'j3']}]
                                       schedule   fitness score
    1  {'m1': ['j1', 'j1'], 'm2': ['j4', 'j3']}  (100.0, False)
    0  {'m1': ['j1', 'j4'], 'm2': ['j1', 'j3']}   (50.0, False)
    
    Generation :  276
    [{'m1': ['j1', 'j4'], 'm2': ['j1', 'j3']}, {'m1': ['j1', 'j4'], 'm2': ['j1', 'j3']}]
                                       schedule  fitness score
    0  {'m1': ['j4', 'j4'], 'm2': ['j1', 'j3']}  (50.0, False)
    1  {'m1': ['j1', 'j4'], 'm2': ['j4', 'j3']}  (50.0, False)
    
    Generation :  277
    [{'m1': ['j1', 'j4'], 'm2': ['j4', 'j3']}, {'m1': ['j4', 'j4'], 'm2': ['j1', 'j3']}]
                                       schedule  fitness score
    1  {'m1': ['j2', 'j3'], 'm2': ['j1', 'j4']}   (50.0, True)
    0  {'m1': ['j3', 'j4'], 'm2': ['j2', 'j2']}  (50.0, False)
    
    Generation :  278
    [{'m1': ['j2', 'j4'], 'm2': ['j1', 'j2']}, {'m1': ['j3', 'j3'], 'm2': ['j2', 'j4']}]
    Solution found :  {'m1': ['j3', 'j1'], 'm2': ['j2', 'j4']}

