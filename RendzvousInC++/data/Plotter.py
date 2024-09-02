import numpy as np
import matplotlib.pyplot as plt
import csv

def plot_file(data, num_generations):#making a plotter func is easier here 
    j = 0;#counter for elements being used
    for i in data:  
        gen_name = i[0]
        num_elements = len(i) -2 #total population number, you have to minus the element containing the name off of it
        start_counter = j*5
        end_counter = start_counter +5
        #print(i)
        data_array = np.array(i[1:num_elements+1])
        #print(data_array)
        element_list = np.arange(start_counter, end_counter)
        #print(element_list)
        plt.plot(element_list,data_array, label = gen_name, marker='x')
        j =j +1
    
    plt.legend()
    return()


file_name='./fitness_vals/datafile_2024-09-02_14-15-06.csv'



with open(file_name) as csvfile:
    generations =[]#define new generation list
    reader = csv.reader(csvfile, delimiter=',')
    for row in reader:
        generations.append(row)
        #print(generations)
            
num_generations = len(generations)
#print(num_generations)

plot_file(generations,num_generations)
plt.show