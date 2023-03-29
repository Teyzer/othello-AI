import os
import shutil

if __name__ == "__main__":
    start_directory = "temp_agents"
    end_directory = "sample_agents"
    nb_agents = len(list(os.listdir(end_directory)))
    for i in range(32):
        shutil.copy2(start_directory + "/agent{0}.txt".format(i), end_directory + "/agent{0}.txt".format(i + nb_agents))