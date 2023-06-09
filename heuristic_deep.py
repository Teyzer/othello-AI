import numpy as np
from typing import *
import subprocess
from threading import Thread
import concurrent.futures
import itertools
import random
import os

#--------------------CLASS-----------------------#

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

#---------------------FUNC-----------------------#
def out_random_mutation(ag: Agent, standard_deviation = 0.3):
    
    result = Agent()
    
    result.W1 = ag.W1 + standard_deviation*np.random.randn(ag.W1.shape[0], ag.W1.shape[1])
    result.W2 = ag.W2 + standard_deviation*np.random.randn(ag.W2.shape[0], ag.W2.shape[1])
    result.W3 = ag.W3 + standard_deviation*np.random.randn(ag.W3.shape[0], ag.W3.shape[1])

    result.b1 = ag.b1 + standard_deviation*np.random.randn(ag.b1.shape[0], ag.b1.shape[1])
    result.b2 = ag.b2 + standard_deviation*np.random.randn(ag.b2.shape[0], ag.b2.shape[1])
    result.b3 = ag.b3 + standard_deviation*np.random.randn(ag.b3.shape[0], ag.b3.shape[1])
    
    return result

#-----------------------------------------------#
# Utility functions
#-----------------------------------------------#

def sigmoid(X) :
    return 1/(1 + np.exp(-X))
#-----------------------------------------------#
def write(A : Agent, index : int) :
    with open("temp_agents/agent" + str(index) + ".txt", "w") as f :
        f.write("\n".join(map(str, A.W1.flatten())) + "\n" +\
                 "\n".join(map(str, A.W2.flatten())) + "\n" +\
                 "\n".join(map(str, A.W3.flatten())) + "\n" +\
                 "\n".join(map(str, A.b1.flatten())) + "\n" +\
                 "\n".join(map(str, A.b2.flatten())) + "\n" +\
                 "\n".join(map(str, A.b3.flatten())))
        
#-----------------------------------------------#
#-----------------------------------------------#
def crossover(A1 : Agent, A2 : Agent) :

    p1 = A1.parameters()
    p2 = A2.parameters()

    nA1 = Agent() 
    nA2 = Agent()

    nA1.initialize(p1["W1"], p2["W2"], p1["W3"], p1["b1"], p2["b2"], p1["b3"])
    nA2.initialize(p2["W1"], p1["W2"], p2["W3"], p2["b1"], p1["b2"], p2["b3"])

    return [nA1,nA2]


#-----------------------------------------------#
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
    txt = result.stdout.decode("utf-8")
    # print("STD-OUT : \n", txt) # PK ca imprime pas ca ? <-- necesitamos vever el stdout
    i = txt.index("RESULT:") # psque je voulais voir les barres juste le temps de voir les [32, 32, 32, 32, ..., ..., ..., ...]
    result = txt[i + len("RESULT:")]

    return int(result) == 1
    
#-----------------------------------------------#
def evalue_fitness_multi_threading(nb_rounds: int, agents: List[Agent]):

    
    nb_agents = len(agents)

    scores = [0 for _ in agents]
    
    res = []

    current_done = 0
    bar_length = 20

    for r in range(nb_rounds):
        print("Inner round #{0}".format(r+1))

        for i in range(nb_agents):
            
            current_done = 0

            print("\r[{0}] {1}/{2}".format(" "*bar_length, i + 1, nb_agents), end="")

            with concurrent.futures.ThreadPoolExecutor() as executor:
                futures = []
                for j in range(nb_agents):
                    futures.append(executor.submit(evaluate_against, model1=agents[i], model2=agents[j]))
                for future in concurrent.futures.as_completed(futures):
                    res = future.result()
                    #print(f"{i}: Fight #")
                    current_done += 1
                    bar_progress = int((100 * current_done / nb_agents) * bar_length / 100)
                    print("\r[{0}{1}] {2}/{3}".format("="*bar_progress, " "*(bar_length-bar_progress), i + 1, nb_agents), end="")
                    

                    if res:
                        scores[i] += 1
                    # print('\r>> [] %d%%' % i),
                    # sys.stdout.flush()

                    # else:
                    #     scores[j] += 1

            print()
            # for j in range(i + 1, nb_agents):


                
            #     res = evaluate_against(agents[i], agents[j])
            #     # print(i, j)
            #     # t = ThreadWithReturnValue(target=evaluate_against, args=(agents[i], agents[j]))
            #     # t.start()
            #     # threads.append([i, j, t])
                # if res:
                #     scores[i] += 1
                # else:
                #     scores[j] += 1

    return scores

#-----------------------------------------------#
def evalue_fitness(nb_rounds: int, gen: List[Agent]):

    for i in range(len(gen)) :
        write(gen[i], i)

    result = subprocess.run(["./main"], capture_output=True) # maybe input nb_rounds
    txt = result.stdout.decode("utf-8")

    i = txt.index("RESULT:")
    result = list(map(int, txt[i + len("RESULT:"):].split(";")))

    return result
    
#-----------------------------------------------#
def evolve_generation(gen : list[Agent], scores : List[int]) :

    gen_couple = [(gen[i], scores[i]) for i in range(len(scores))]
    gen_couple.sort(key=lambda x : x[1], reverse=True)

    f = open("best_agent.txt", "w")
    f.write("\n".join(map(str, gen_couple[0][0].W1.flatten())) + "\n" +\
                 "\n".join(map(str, gen_couple[0][0].W2.flatten())) + "\n" +\
                 "\n".join(map(str, gen_couple[0][0].W3.flatten())) + "\n" +\
                 "\n".join(map(str, gen_couple[0][0].b1.flatten())) + "\n" +\
                 "\n".join(map(str, gen_couple[0][0].b2.flatten())) + "\n" +\
                 "\n".join(map(str, gen_couple[0][0].b3.flatten())) + "\n")
    f.close()
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
        new_gen[-1] = out_random_mutation(new_gen[-1], 0.01)
        new_gen[-2] = out_random_mutation(new_gen[-2], 0.01)

    # 12 mutation
    for j in range(4) :
        new_gen.append(gen_couple[j][0])
        new_gen.append(gen_couple[j][0])
        new_gen.append(gen_couple[j][0])
        
        new_gen[-1] = out_random_mutation(new_gen[-1], 0.5)
        new_gen[-2] = out_random_mutation(new_gen[-2], 0.1)
        new_gen[-3] = out_random_mutation(new_gen[-3], 0.05)

    return new_gen

#-----------------------------------------------#
def first_generation(nb_agents) :
    n = [Agent() for _ in range(nb_agents)]
    for a in n :
        a.rand_initialise()
        a.rand_mutations(random.randint(-3,3))
    return n

#-----------------------------------------------#
def get_best(gen: List[Agent]) -> Agent:

    scores = evalue_fitness(1, gen)
    gen_couple = [(gen[i], scores[i]) for i in range(len(scores))]
    gen_couple.sort(key=lambda x : x[1], reverse=True)

    print("Best score:", gen_couple[0][1])
    return gen_couple[0][0]

#-----------------------------------------------#
def model(nb_gen: int, nb_agents: int) :
    generation  = first_generation(nb_agents)
    for r in range(nb_gen) :
        print("Playing round:", r+1, "/", nb_gen)
        score = evalue_fitness(1, generation)
        print("Tableau des scores:", score)
        generation = evolve_generation(generation, score)
        if r % 10 == 0:
            eval_performance()
    return get_best(generation)




#-----------------------------------------------#

def read() :
    n = []
    for i in range(32) :
        with open("temp_agents/agent" + str(i) + ".txt", "r" ) as f :
            a = Agent()
            txt = list(map(float,f.read().split("\n")))

            a.initialize(np.reshape(txt[0:64*64],(64,64)), 
                         np.reshape(txt[64*64:2*64*64], (64,64)), 
                         np.reshape(txt[2*64*64:2*64*64 + 64], (1,64)), 
                         np.reshape(txt[2*64*64 + 64:2*64*64 + 2*64],(64,1)), 
                         np.reshape(txt[2*64*64 + 2*64:2*64*64 + 3*64],(64,1)), 
                         np.reshape(txt[2*64*64 + 3*64: 2*64*64 + 3*64 + 1],(1,1)))
            n.append(a)
    return n

#-----------------------------------------------#
def model_continue(nb_gen: int, nb_agents: int) :
    generation = read()
    for r in range(nb_gen) :
        print("Playing round:", r+1, "/", nb_gen)
        score = evalue_fitness(1, generation)
        print("Tableau des scores : ", score)
        generation = evolve_generation(generation, score)
        if r % 10 == 0:
            eval_performance()
    return get_best(generation)

#-----------------------------------------------#

def eval_performance():
    nb_agents = len(list(os.listdir("sample_agents")))
    wins = 0
    for i in range(nb_agents):
        z = subprocess.run(["./main", "best_agent.txt", "sample_agents/agent{0}.txt".format(i)], capture_output=True)
        u = z.stdout.decode()
        if int(u[u.index("RESULT:") + len("RESULT:")]) == 1:
            wins += 1
    print("#-- Score: {0}/{1}".format(wins, nb_agents))
    with open("result.txt", "a") as f:
        f.write(str(float(wins)/float(nb_agents)) + ",")
    
    
    
#-----------------------------------------------#
if __name__ == "__main__":

    subprocess.run("gcc -o main main.c othello.c matrix.c -g -lm".split(), capture_output=True) # pour compiler une fois avant
    A = model_continue(3000, 32)




