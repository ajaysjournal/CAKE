
Class Machine
=============

::

    Machine {
        attribute-1 : value-1
        attribute-2 : value-2
        attribute-3 : value-3
        attribute-4 : value-4
        attribute-5 : value-5
        
        Jobs = [
        
            {
               name = J1
               attribute1 = a1
        
            },
            {
               name = J1
               attribute1 = a1
        
            },
            {
               name = J1
               attribute1 = a1
        
            },
            {
               name = J1
               attribute1 = a1
        
            },
            .
            .
            .
            .
            
        
        
        ]
        
        
    }

Population initialization
-------------------------

.. code:: python

    import random
    from deap import tools

.. code:: python

    class Machine:
    
         def __init__(self, type):
            self.type = type 
            self.id = random.uniform(1, 10)
         
         def __str__(self):
            return "type = %s, id = %s "% (self.type, self.id)
            
            

.. code:: python

    machine1 = Machine("ec2-instance1")
    machine2 = Machine("ec2-instance2")
    print(machine1)
    print(machine2)
    lst = [machine1,machine2]


.. parsed-literal::

    type = ec2-instance1, id = 6.291858862473603 
    type = ec2-instance2, id = 1.442505876067043 


.. code:: python

    def initListOfClass():
        """ Here we need to  create the Population 
            Time being we are returing the list of classes 
        """ 
        # todo 
        machine1 = Machine("ec2-instance1")
        machine2 = Machine("ec2-instance2")
        lst = [machine1,machine2]
        ## todo generate uniform population
        return lst


.. code:: python

    for individual in population:
        for item in individual:  # individual is a collection of machines 
            print(item)


.. parsed-literal::

    type = ec2-instance1, id = 6.230752951433176 
    type = ec2-instance2, id = 1.8950821391601065 
    type = ec2-instance1, id = 2.3484315611478768 
    type = ec2-instance2, id = 2.1475370653604244 
    type = ec2-instance1, id = 8.279094279717839 
    type = ec2-instance2, id = 9.804635828490593 
    type = ec2-instance1, id = 1.3223420434931321 
    type = ec2-instance2, id = 6.183984408046574 
    type = ec2-instance1, id = 3.0609305506918476 
    type = ec2-instance2, id = 5.458811013352265 
    type = ec2-instance1, id = 8.705676331582456 
    type = ec2-instance2, id = 8.010944710194709 
    type = ec2-instance1, id = 3.8891137766405657 
    type = ec2-instance2, id = 5.366030094959112 
    type = ec2-instance1, id = 7.979732275542519 
    type = ec2-instance2, id = 2.679470202148593 
    type = ec2-instance1, id = 1.6638579797283306 
    type = ec2-instance2, id = 6.339781416213646 
    type = ec2-instance1, id = 2.2476084002523873 
    type = ec2-instance2, id = 2.879658073719627 



