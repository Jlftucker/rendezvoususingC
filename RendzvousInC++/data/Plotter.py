import numpy as np
import matplotlib.pyplot as plt
import csv



def setup_data_arrays_all(csv_data, num_generations,num_elements):
    
    generation_name =np.array([])
    total_num_elements = num_generations*(num_elements-2)
    elements = np.array([])##minus two to take into account the name of the gen and the , at the end
    #csv_data is a 2d list where each row represents a generation
    for i in csv_data:#grab each row
        gen_name=i[0]#name is the first element
        j = 1#initialize counter at one to ignore the name
        np.append(generation_name, gen_name)
        while j<(num_elements-1):#we want it to be less than the total number because their is a space at the end of the csv file
            value = float(i[j])
            elements = np.append(elements, value)
            j =j+1
    
   
    element_list = np.arange(0,total_num_elements)
    
    return(elements, element_list)


def setup_data_arrays_maxval(csv_data, num_generations,num_elements):
    
    generation_name =np.array([])
    total_num_elements = num_generations*(num_elements-2)
    elements = np.array([])
    #csv_data is a 2d list where each row represents a generation
    for i in csv_data:#grab each row
        element_row = np.array([])
        gen_name=i[0]#name is the first element
        j = 1#initialize counter at one to ignore the name
        np.append(generation_name, gen_name)
        while j<(num_elements-1):#we want it to be less than the total number because their is a space at the end of the csv file
            value = float(i[j])
            element_row = np.append(element_row, value)
            j =j+1   
            
        element = max(element_row)
        elements =np.append(elements,element)
        
   
    element_list = np.arange(0,num_generations)
    
    return(elements, element_list)


