# Report

The solution is provided in the code files. 

In the Linux Parser, the appropriate functions were completed by reading specific files extract the relevant information. 

The most challenging task was the completion of the Process class and consequently the part of the System task that deals with the processes. 

In the System class, at each update the active processes are added to a vector and then sorted, in this case by CPU utilisation. Before adding the processes, the vector is cleared. This poses a limitation as is is not straightforward to have history and calculate the utilization at each step. In the implementation the CPU utilization of each process is taken from the power up. 

On the contrary for the CPU class itself, the deltas of the jiffies are extracted at each update step and the total CPU utilization is accurate. 

A screenshot from the running program is shown below:

![](images/process_monitor_dm.png)

An issue seems to be that when there is an update and the new value is shorter in characters, the characters from previous updates are not cleared. 



# Project Specification



## Process Monitor

Basic Requirements

| Criteria                                                     | Meets Specifications                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| The student will be able to organize code in a project structure. | The program must build an executable system monitor.         |
| The student will be able to write warning-free code.         | The program must build without generating compiler warnings. |
| The student will be able to create a working project.        | The system monitor must run continuously without error, until the user terminates the program. |
| The student will be able to organize code using object oriented programming principles. | The project should be organized into appropriate classes.    |

The above criteria are fulfilled to the best of my knowledge



## System Requirements

| Criteria                                                     | Meets Specifications                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| The student will be able to extract and display basic data about the system. | The system monitor program should list at least the operating system,  kernel version, total number of processes, number of running processes,  and up time. |
| The student will be able to use composition.                 | The System class should be composed of at least one other class. |

The above criteria are fulfilled to the best of my knowledge

## Processor Requirements

| Criteria                                                     | Meets Specifications                                   |
| ------------------------------------------------------------ | ------------------------------------------------------ |
| The student will be able to read and display data about the CPU. | The system monitor should display the CPU utilization. |

The above criteria are fulfilled to the best of my knowledge

## Process Requirements

| Criteria                                                     | Meets Specifications                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| The student will be able to read and display the processes on the system. | The system monitor should display a partial list of processes running on the system. |
| The student will be able to display data about individual processes. | The system monitor should display the PID, user, CPU utilization, memory utilization, up time, and command for each process. |

The above criteria are fulfilled to the best of my knowledge

## Suggestions to Make Your Project Stand Out!

Calculate CPU utilization dynamically, based on recent utilization
Sort processes based on CPU or memory utilization
Make the display interactive
Restructure the program to use abstract classes (interfaces) and pure virtual functions
Port the program to another operating system