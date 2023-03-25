import numpy as np
from typing import *
import itertools
import subprocess
from threading import Thread

class ThreadWithReturnValue(Thread):
    
    def __init__(self, group=None, target=None, name=None,
                 args=(), kwargs={}, Verbose=None):
        Thread.__init__(self, group, target, name, args, kwargs)
        self._return = None

    def run(self):
        if self._target is not None:
            self._return = self._target(*self._args,
                                                **self._kwargs)
    def join(self, *args):
        Thread.join(self, *args)
        return self._return




class Agent :
    def __init__(self):
        
        self.W1 = np.empty([64,64])
        self.W2 = np.empty([64,64])
        self.W3 = np.empty([1,64])
        self.b1 = np.empty([64,1])
        self.b2 = np.empty([64,1])
        self.b3 = np.empty([1,1])

    def initialize(self, W1, W2, W3, b1, b2, b3) :

        self.W1 = W1
        self.W2 = W2
        self.W3 = W3
        self.b1 = b1
        self.b2 = b2
        self.b3 = b3
    
    def parameters(self):
        return {"W1" : self.W1, "W2" : self.W2, "W3" : self.W3, "b1" : self.b1, "b2" : self.b2, "b3" : self.b3}
    
    def rand_mutations(self, standard_deviation = 0.1) :
        
        self.W1 = self.W1 + standard_deviation*np.random.randn(self.W1.shape[0], self.W1.shape[1])
        self.W2 = self.W2 + standard_deviation*np.random.randn(self.W2.shape[0], self.W2.shape[1])
        self.W3 = self.W3 + standard_deviation*np.random.randn(self.W3.shape[0], self.W3.shape[1])

        self.b1 = self.b1 + standard_deviation*np.random.randn(self.b1.shape[0], self.b1.shape[1])
        self.b2 = self.b2 + standard_deviation*np.random.randn(self.b2.shape[0], self.b2.shape[1])
        self.b3 = self.b3 + standard_deviation*np.random.randn(self.b3.shape[0], self.b3.shape[1])
    
    def rand_initialise(self, n0 = 64, n1 = 64, n2 = 64, n3 = 1) : # n0 nb d'entree | n1 nb de neurone dans couche 1 | n2 nb de neurone dans couche 2 | n3 nb de neurone dans couhe 3 = sortie
        
        self.W1 = np.random.randn(n1, n0)
        self.b1 = np.random.randn(n1, 1)
        self.W2 = np.random.randn(n2, n1)
        self.b2 = np.random.randn(n2, 1)
        self.W3 = np.random.randn(n3, n2)
        self.b3 = np.random.randn(n3, 1)

    def forward_propagation(self, X):

        # TODO check if we win or we lose return INT MAX resp INT MIN

        # couche 1
        Z1 = self.W1.dot(X) + self.b1
        A1 = sigmoid(Z1)

        # couche 2
        Z2 = self.W2.dot(A1) + self.b2
        A2 = sigmoid(Z2)

        # couche 3
        Z3 = self.W3.dot(A3) + self.b3
        A3 = sigmoid(Z3)

        return A3


# Utility functions
def sigmoid(X) :
    return 1/(1 + np.exp(-X))
#----------------------------------------#

def crossover(A1 : Agent, A2 : Agent) :

    p1 = A1.parameters()
    p2 = A2.parameters()

    nA1 = Agent() 
    nA2 = Agent()

    nA1.initialize(p1["W1"], p2["W2"], p1["W3"], p1["b1"], p2["b2"], p1["b3"])
    nA2.initialize(p2["W1"], p1["W2"], p2["W3"], p2["b1"], p1["b2"], p2["b3"])

    return [nA1,nA2]




def evaluate_against(model1: Agent, model2: Agent):
     
    first_text_input = "\n".join(map(str, model1.W1.flatten())) + "\n" +\
                 "\n".join(map(str, model1.W2.flatten())) + "\n" +\
                 "\n".join(map(str, model1.W3.flatten())) + "\n" +\
                 "\n".join(map(str, model1.b1.flatten())) + "\n" +\
                 "\n".join(map(str, model1.b2.flatten())) + "\n" +\
                 "\n".join(map(str, model1.b3.flatten())) + "\n"
    
    second_text_input = "\n".join(map(str, model2.W1.flatten())) + "\n" +\
                 "\n".join(map(str, model2.W2.flatten())) + "\n" +\
                 "\n".join(map(str, model2.W3.flatten())) + "\n" +\
                 "\n".join(map(str, model2.b1.flatten())) + "\n" +\
                 "\n".join(map(str, model2.b2.flatten())) + "\n" +\
                 "\n".join(map(str, model2.b3.flatten()))

    result = subprocess.run(["./main"], capture_output=True, input=(first_text_input + second_text_input).encode("UTF-8"))
    txt = result.stdout

    print(txt)
    


def evalue_fitness(nb_rounds: int, agents: List[Agent]):

    subprocess.run("gcc -o main main.c othello.c matrix.c -g -lm".split(), capture_output=True) # pour compiler une fois avant
    nb_agents = len(agents)

    scores = [0 for _ in agents]
    
    threads = []
    res = []

    for r in range(nb_rounds):
        print("Inner round #{0}".format(r))
        for i in range(nb_agents):
            for j in range(i + 1, nb_agents):
                t = ThreadWithReturnValue(target=evaluate_against, args=(agents[i], agents[j]))
                t.start()
                threads.append([i, j, t])
        
        for thread in threads:
            res.append([thread[0], thread[1], thread[2].join()])

        for element in res:
            if element[2]:
                scores[element[0]] += 1
            else:
                scores[element[1]] += 1


    return scores

def evolve_generation(gen : list[Agent], scores : List[int]) :

    gen_couple = [(gen[i], scores[i]) for i in range(len(scores))]
    gen_couple.sort(key=lambda x : x[1], reverse=True)

    new_gen = []

    # 32 AGENTS : 4 ( top agents ) + 4 ( random ) + 12 ( crossover ) + 12 ( mutation )

    # Get top 4
    new_gen.append(gen_couple[0][0])
    new_gen.append(gen_couple[1][0])
    new_gen.append(gen_couple[2][0])
    new_gen.append(gen_couple[3][0])

    # Add 4 random
    new_gen.extend([Agent() for _ in range(4)])
    for i in range(1,5) :
        new_gen[-i].rand_initialise()


    # 12 crossover
    for sub in itertools.combinations([0,1,2,3], 2):
        new_gen.extend(crossover(gen_couple[sub[0]][0], gen_couple[sub[1]][0]))

    # 12 mutation
    for j in range(4) :
        new_gen.append(gen_couple[j][0])
        new_gen.append(gen_couple[j][0])
        new_gen.append(gen_couple[j][0])
        for i in range(1,4) :
            new_gen[-i].rand_initialise()

    return new_gen

def first_generation(nb_agents) :
    n = [Agent() for _ in range(nb_agents)]
    for a in n :
        a.rand_initialise()
    return n

def get_best(gen: List[Agent]) -> Agent:

    scores = evalue_fitness(1, gen)
    gen_couple = [(gen[i], scores[i]) for i in range(len(scores))]
    gen_couple.sort(key=lambda x : x[1], reverse=True)

    print("Best score : ", gen_couple[0][1])
    return gen_couple[0][0]


def model(nb_gen: int, nb_agents: int) :
    generation  = first_generation(nb_agents)
    for r in range(nb_gen) :
        print("Playing round : ", r)
        score = evalue_fitness(1, generation)
        print("Tableau des scores : ", score)
        generation = evolve_generation(generation, score)
    return get_best(generation)



#-----------------------------------------------#

if __name__ == "__main__":

    A = model(1, 32)

    with open("sample.txt", "w") as f:
        t = "\n".join(map(str, A.W1.flatten()))
        t += "\n" + "\n".join(map(str, A.W2.flatten()))
        t += "\n" + "\n".join(map(str, A.W3.flatten()))
        t += "\n" + "\n".join(map(str, A.b1.flatten()))
        t += "\n" + "\n".join(map(str, A.b2.flatten()))
        t += "\n" + "\n".join(map(str, A.b3.flatten()))
        f.write(t)

